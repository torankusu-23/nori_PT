/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/


#include <nori/accel.h>
#include <Eigen/Geometry>

NORI_NAMESPACE_BEGIN

void Accel::addMesh(Mesh *mesh) {
    if (m_num_meshes >= MAX_NUM_MESHES)
        throw NoriException("Accel: only %d mesh is supported!", MAX_NUM_MESHES);
    m_meshes[m_num_meshes++] = mesh;
    m_bbox.expandBy(mesh->getBoundingBox());
}



Node* Accel::recursion(BoundingBox3f box, std::vector<uint32_t>& triangles, std::vector<uint32_t>& mesh_indices, uint32_t depth) {
    //空节点
    if(triangles.size() == 0) {
        return nullptr;
    }
    //叶子
    if(triangles.size() <= MAX_TRIANGLES_PER_NODE || depth == 0) {
        Node* leaf = new Node();
        leaf->bbox=box;
        leaf->mesh_index = mesh_indices;
        leaf->triangles_index = triangles;
        return leaf;
    }
    
    //否则，必然要分叉
    //划分8块bbox
    std::vector<BoundingBox3f> divide_box;
    Point3f lb = box.min;
    Point3f rt = box.max;
    Point3f c = box.getCenter();
    divide_box.push_back(BoundingBox3f(lb, c));
    divide_box.push_back(BoundingBox3f(Point3f(c.x(), lb.y(), lb.z()), Point3f(rt.x(), c.y(), c.z())));
    divide_box.push_back(BoundingBox3f(Point3f(lb.x(), lb.y(), c.z()), Point3f(c.x(), c.y(), rt.z())));
    divide_box.push_back(BoundingBox3f(Point3f(c.x(), lb.y(), c.z()), Point3f(rt.x(), c.y(), rt.z())));
    divide_box.push_back(BoundingBox3f(Point3f(lb.x(), c.y(), lb.z()), Point3f(c.x(), rt.y(), c.z())));
    divide_box.push_back(BoundingBox3f(Point3f(c.x(), c.y(), lb.z()), Point3f(rt.x(), rt.y(), c.z())));
    divide_box.push_back(BoundingBox3f(Point3f(lb.x(), c.y(), c.z()), Point3f(c.x(), rt.y(), rt.z())));
    divide_box.push_back(BoundingBox3f(c, rt));

    //划分三角形
    std::vector<std::vector<uint32_t> > divide_tri(8, std::vector<uint32_t>());
    //三角形对应mesh
    std::vector<std::vector<uint32_t> > divide_mesh(8, std::vector<uint32_t>());

    for(uint32_t i = 0; i < triangles.size(); ++i) {//for all triangles

        uint32_t mesh_idx = mesh_indices[i];
        uint32_t tri_idx = triangles[i];

        for(uint16_t j = 0; j < 8; ++j) {           //for all division
            if(divide_box[j].overlaps(m_meshes[mesh_idx]->getBoundingBox(tri_idx))) {//重叠了，将三角形下标及其mesh下标填入(一一对应)
                divide_tri[j].push_back(tri_idx);
                divide_mesh[j].push_back(mesh_idx);
            }
        }
    }

    //构建八叉树
    Node* root = new Node();
    root->bbox = box;

    for(uint32_t i = 0; i < 8; ++i) {
        root->child.push_back(recursion(divide_box[i], divide_tri[i], divide_mesh[i], depth - 1));
    }

    return root;
}


void Accel::build() {
    if(m_num_meshes == 0)
        throw NoriException("No mesh found, could not build acceleration structure");

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    uint32_t num_triangles = 0;
    for(uint32_t mesh_idx = 0; mesh_idx < m_num_meshes; ++mesh_idx) {//
        num_triangles += m_meshes[mesh_idx]->getTriangleCount();//统计all mesh的三角形数量
    }

    std::vector<uint32_t> triangles(num_triangles);//三角形下标
    std::vector<uint32_t> mesh_indices(num_triangles);//mesh下标
    uint32_t offset = 0;//
    
    //利用offset，统计上面的三角形下标和mesh下标，通过这俩值可以提取任意mesh的任意三角形
    for(uint32_t current_mesh_idx = 0; current_mesh_idx < m_num_meshes; ++current_mesh_idx) {

        uint32_t current_tri_num = m_meshes[current_mesh_idx]->getTriangleCount();//当前网格三角形数量
        
        for(uint32_t i = 0; i < current_tri_num; ++i) {
            triangles[offset + i] = i;                  //每个mesh中三角形的下标
            mesh_indices[offset + i] = current_mesh_idx;//mesh的下标
        }
        offset += current_tri_num;
    }

    tree = recursion(m_bbox, triangles, mesh_indices, MAX_RECURSION_DEPTH);//递归建树

    auto end = high_resolution_clock::now();
    std::cout<<"\nOctTree build finish"
    <<"\ncost: "<<duration_cast<milliseconds>(end - start).count()<<" ms"
    <<std::endl;
}


bool Accel::intersect(Ray3f &ray, Intersection &its, bool shadowRay, Node* root, uint32_t& id) const {
    
    if(!root) return false;
    if(!root->bbox.rayIntersect(ray)) return false;//box和ray无交点

    bool foundIntersection = false;

    //找到节点
    if(root->child.size() == 0) {//叶子结点

        for(uint32_t idx = 0; idx < root->triangles_index.size(); ++idx) {
            float u, v, t;
            //对应的mesh对应的triagnles的idx
            uint32_t mesh_idx = root->mesh_index[idx];
            uint32_t tri_idx = root->triangles_index[idx];
            if(m_meshes[mesh_idx]->rayIntersect(tri_idx, ray, u, v, t) && t < ray.maxt) {
                if(shadowRay) {//如果是shadowRay追踪，就没必要找交点位置了，判定有无交点即可返回ture or false
                    return true;
                }
                ray.maxt = t;
                its.t = t;
                its.uv = Point2f(u, v);
                its.mesh = m_meshes[mesh_idx];
                id = tri_idx;
                foundIntersection = true;
            }
        }
    } else { //否则就是找到了有交点的中间节点
        //给box到光源的距离排个序,每一组box对光源距离不同
        std::pair<uint32_t, float> index_node[8] = {};
        for(uint32_t i = 0; i < 8; ++i) {
            if(root->child[i] == nullptr) {
                index_node[i] = std::make_pair(i, __FLT_MAX__);
            } else {
                index_node[i] = std::make_pair(i, root->child[i]->bbox.distanceTo(ray.o));
            }
        }
        std::sort(index_node, index_node + 8, [](const auto& l, const auto& r) {return l.second < r.second;});
        
        for(uint32_t ch = 0; ch < 8; ++ch) {

            foundIntersection |= intersect(ray, its, shadowRay, root->child[index_node[ch].first], id);
            if(shadowRay && foundIntersection) return true;//shadowRay仅追踪碰撞，找到即可停
            //if(foundIntersection) break;//找到即可停，因为排过序，找到交点之后的块不必再计算
            //后续发现，这里不能停，如果停了，会在之后的光追中出现透视的情况。
        }
    }

    return foundIntersection;
}


bool Accel::rayIntersect(const Ray3f &ray_, Intersection &its, bool shadowRay) const {
    bool foundIntersection = false;  // Was an intersection found so far?
    uint32_t f = (uint32_t) -1;      // Triangle index of the closest intersection

    Ray3f ray(ray_); /// Make a copy of the ray (we will need to update its '.maxt' value)

    foundIntersection = intersect(ray, its, shadowRay, tree, f);

    if(foundIntersection && shadowRay) return true;//shadowRay提前结束

    if (foundIntersection) {
        /* At this point, we now know that there is an intersection,
           and we know the triangle index of the closest such intersection.

           The following computes a number of additional properties which
           characterize the intersection (normals, texture coordinates, etc..)
        */

        /* Find the barycentric coordinates */
        Vector3f bary;
        bary << 1-its.uv.sum(), its.uv;

        /* References to all relevant mesh buffers */
        const Mesh *mesh   = its.mesh;
        const MatrixXf &V  = mesh->getVertexPositions();
        const MatrixXf &N  = mesh->getVertexNormals();
        const MatrixXf &UV = mesh->getVertexTexCoords();
        const MatrixXu &F  = mesh->getIndices();

        /* Vertex indices of the triangle */
        uint32_t idx0 = F(0, f), idx1 = F(1, f), idx2 = F(2, f);//获得顶点下标

        Point3f p0 = V.col(idx0), p1 = V.col(idx1), p2 = V.col(idx2);//通过下标得到坐标

        /* Compute the intersection positon accurately
           using barycentric coordinates */
        its.p = bary.x() * p0 + bary.y() * p1 + bary.z() * p2;//重心插值坐标

        /* Compute proper texture coordinates if provided by the mesh */
        if (UV.size() > 0)
            its.uv = bary.x() * UV.col(idx0) +//重心插值uv
                bary.y() * UV.col(idx1) +
                bary.z() * UV.col(idx2);

        /* Compute the geometry frame */
        its.geoFrame = Frame((p1-p0).cross(p2-p0).normalized());//geoFrame = Frame(面法线？)

        if (N.size() > 0) {
            /* Compute the shading frame. Note that for simplicity,
               the current implementation doesn't attempt to provide
               tangents that are continuous across the surface. That
               means that this code will need to be modified to be able
               use anisotropic BRDFs, which need tangent continuity */

            its.shFrame = Frame(
                (bary.x() * N.col(idx0) +
                 bary.y() * N.col(idx1) +
                 bary.z() * N.col(idx2)).normalized());//顶点法线插值
        } else {
            its.shFrame = its.geoFrame;//面法线直接给
        }
    }

    return foundIntersection;
}

NORI_NAMESPACE_END

