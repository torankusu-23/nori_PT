/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include <nori/mesh.h>
#include <nori/bbox.h>
#include <nori/bsdf.h>
#include <nori/emitter.h>
#include <nori/warp.h>
#include <Eigen/Geometry>

NORI_NAMESPACE_BEGIN

Mesh::Mesh() { }

Mesh::~Mesh() {
    delete m_bsdf;
    delete m_emitter;
}

void Mesh::activate() {
    if (!m_bsdf) {
        /* If no material was assigned, instantiate a diffuse BRDF */
        m_bsdf = static_cast<BSDF *>(//默认一个diffuse
            NoriObjectFactory::createInstance("diffuse", PropertyList()));
    }

    //统计整个mesh的pdf和cdf
    mesh_area = 0.0f;
    mesh_cdf.reserve(getTriangleCount());//初始化cdf的vector容量大小
    for(uint32_t i = 0; i < getTriangleCount(); ++i) {//遍历三角形
        auto area = surfaceArea(i);
        mesh_area += area;//累加三角形面积
        mesh_cdf.append(area);//累加pdf到cdf中，正比于三角形面积
    }
    mesh_cdf.normalize();//归一化cdf
}


//获取采样点
SampleMeshResult Mesh::sampleSurfaceUniform(Sampler* sampler) const {
    SampleMeshResult res;
    uint32_t idx = mesh_cdf.sample(sampler->next1D());//网格中用inversion method采样一个三角形
    Point2f zeta = sampler->next2D();//三角形中采样随机点，用重心坐标方法
    float alpha = 1 - sqrt(1 - zeta.x());
    float beta = zeta.y() * sqrt(1 - zeta.x());
    Point3f v0 = m_V.col(m_F(0, idx));
    Point3f v1 = m_V.col(m_F(1, idx));
    Point3f v2 = m_V.col(m_F(2, idx));
    res.p = v0 * alpha + v1 * beta + v2 * (1 - alpha - beta);

    if (m_N.size() != 0) {//如果有顶点法线，用重心坐标插值
        Point3f n0 = m_N.col(m_F(0, idx));
        Point3f n1 = m_N.col(m_F(1, idx));
        Point3f n2 = m_N.col(m_F(2, idx));
        res.n = (alpha * n0 + beta * n1 + (1 - alpha - beta) * n2).normalized();
    } else {//否则用表面插乘，此时假设三角形面完全平坦，所以精度低
        Vector3f e1 = v1 - v0;
        Vector3f e2 = v2 - v0;
        res.n = e1.cross(e2).normalized();
    }

    res.pdf = (float)1 / mesh_area;//面积分之一，如果用dpdf的归一化因子也是一样的，但看起来不太直观
    
    return res;
}






//index三角形的面积
float Mesh::surfaceArea(uint32_t index) const {
    //根据面index获取顶点index
    uint32_t i0 = m_F(0, index), i1 = m_F(1, index), i2 = m_F(2, index);
    //顶点坐标
    const Point3f p0 = m_V.col(i0), p1 = m_V.col(i1), p2 = m_V.col(i2);

    return 0.5f * Vector3f((p1 - p0).cross(p2 - p0)).norm();
}

bool Mesh::rayIntersect(uint32_t index, const Ray3f &ray, float &u, float &v, float &t) const {
    uint32_t i0 = m_F(0, index), i1 = m_F(1, index), i2 = m_F(2, index);
    const Point3f p0 = m_V.col(i0), p1 = m_V.col(i1), p2 = m_V.col(i2);

    /* Find vectors for two edges sharing v[0] */
    Vector3f edge1 = p1 - p0, edge2 = p2 - p0;

    /* Begin calculating determinant - also used to calculate U parameter */
    Vector3f pvec = ray.d.cross(edge2);

    /* If determinant is near zero, ray lies in plane of triangle */
    float det = edge1.dot(pvec);

    if (det > -1e-8f && det < 1e-8f)
        return false;
    float inv_det = 1.0f / det;

    /* Calculate distance from v[0] to ray origin */
    Vector3f tvec = ray.o - p0;

    /* Calculate U parameter and test bounds */
    u = tvec.dot(pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return false;

    /* Prepare to test V parameter */
    Vector3f qvec = tvec.cross(edge1);

    /* Calculate V parameter and test bounds */
    v = ray.d.dot(qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0)
        return false;

    /* Ray intersects triangle -> compute t */
    t = edge2.dot(qvec) * inv_det;//计算光线时间（长度)

    return t >= ray.mint && t <= ray.maxt;
}

BoundingBox3f Mesh::getBoundingBox(uint32_t index) const {
    BoundingBox3f result(m_V.col(m_F(0, index)));
    result.expandBy(m_V.col(m_F(1, index)));
    result.expandBy(m_V.col(m_F(2, index)));
    return result;
}

Point3f Mesh::getCentroid(uint32_t index) const {
    return (1.0f / 3.0f) *
        (m_V.col(m_F(0, index)) +
         m_V.col(m_F(1, index)) +
         m_V.col(m_F(2, index)));
}

void Mesh::addChild(NoriObject *obj) {
    switch (obj->getClassType()) {
        case EBSDF:
            if (m_bsdf)
                throw NoriException(
                    "Mesh: tried to register multiple BSDF instances!");
            m_bsdf = static_cast<BSDF *>(obj);
            break;

        case EEmitter: {
                Emitter *emitter = static_cast<Emitter *>(obj);
                if (m_emitter)
                    throw NoriException(
                        "Mesh: tried to register multiple Emitter instances!");
                m_emitter = emitter;
            }
            break;

        default:
            throw NoriException("Mesh::addChild(<%s>) is not supported!",
                                classTypeName(obj->getClassType()));
    }
}

std::string Mesh::toString() const {
    return tfm::format(
        "Mesh[\n"
        "  name = \"%s\",\n"
        "  vertexCount = %i,\n"
        "  triangleCount = %i,\n"
        "  bsdf = %s,\n"
        "  emitter = %s\n"
        "]",
        m_name,
        m_V.cols(),
        m_F.cols(),
        m_bsdf ? indent(m_bsdf->toString()) : std::string("null"),
        m_emitter ? indent(m_emitter->toString()) : std::string("null")
    );
}

std::string Intersection::toString() const {
    if (!mesh)
        return "Intersection[invalid]";

    return tfm::format(
        "Intersection[\n"
        "  p = %s,\n"
        "  t = %f,\n"
        "  uv = %s,\n"
        "  shFrame = %s,\n"
        "  geoFrame = %s,\n"
        "  mesh = %s\n"
        "]",
        p.toString(),
        t,
        uv.toString(),
        indent(shFrame.toString()),
        indent(geoFrame.toString()),
        mesh ? mesh->toString() : std::string("null")
    );
}

NORI_NAMESPACE_END
