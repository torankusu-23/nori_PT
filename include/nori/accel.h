/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#pragma once

#include <nori/mesh.h>

NORI_NAMESPACE_BEGIN

static constexpr uint32_t MAX_TRIANGLES_PER_NODE = 15;  //单位节点最多三角形数
static constexpr uint32_t MAX_RECURSION_DEPTH = 10;     //最大递归深度
static constexpr uint32_t MAX_NUM_MESHES = 32;          //场景最多mesh数

struct Node {
    std::vector<Node*> child;//子节点数组
    std::vector<uint32_t> mesh_index;//这里用两个数组来存mesh和tri的indices
    std::vector<uint32_t> triangles_index;
    BoundingBox3f bbox;

    ~Node() {

    }
};

/**
 * \brief Acceleration data structure for ray intersection queries
 *
 * The current implementation falls back to a brute force loop
 * through the geometry.
 */
class Accel {
public:
    /**
     * \brief Register a triangle mesh for inclusion in the acceleration
     * data structure
     *
     * This function can only be used before \ref build() is called
     */
    void addMesh(Mesh *mesh);

    /// Build the acceleration data structure (currently a no-op)
    void build();

    /// Return an axis-aligned box that bounds the scene
    const BoundingBox3f &getBoundingBox() const { return m_bbox; }


    //递归建树
    Node* recursion(BoundingBox3f box, std::vector<uint32_t>& triangles, std::vector<uint32_t>& mesh_indices, uint32_t depth);
    //交点判定
    bool intersect(Ray3f &ray, Intersection &its, bool shadowRay, Node* node, uint32_t& id) const;


    /**
     * \brief Intersect a ray against all triangles stored in the scene and
     * return detailed intersection information
     *
     * \param ray
     *    A 3-dimensional ray data structure with minimum/maximum extent
     *    information
     *
     * \param its
     *    A detailed intersection record, which will be filled by the
     *    intersection query
     *
     * \param shadowRay
     *    \c true if this is a shadow ray query, i.e. a query that only aims to
     *    find out whether the ray is blocked or not without returning detailed
     *    intersection information.
     *
     * \return \c true if an intersection was found
     */
    bool rayIntersect(const Ray3f &ray, Intersection &its, bool shadowRay) const;


private:
    BoundingBox3f m_bbox;           ///< Bounding box of the entire scene

    Mesh*       m_meshes[MAX_NUM_MESHES];   //多mesh
    uint32_t    m_num_meshes = 0;           //mesh数量
    Node         *tree = nullptr;           //OctTree根节点

};

NORI_NAMESPACE_END
