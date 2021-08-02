/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#pragma once

#include <nori/object.h>

NORI_NAMESPACE_BEGIN

struct EmitterQueryRecord {
    Point3f x;//着色点
    Point3f y;//光源点
    Normal3f n;//y法线
    Vector3f wi;//着色点到光源点的单位方向
    float pdf;//采样点p的pdf，这里是均匀的（面光源基本都是均匀的才能做），所以每个点都一样

    /// Create a new record for sampling the Emitter 采样的
    EmitterQueryRecord(const Point3f &x): x(x) {}

    /// Create a new record for querying the Emitter 记录的
    EmitterQueryRecord(const Point3f &x, const Point3f &y, const Normal3f& n): x(x), y(y), n(n) 
    {
        wi = (y - x).normalized();
    }
};


/**
 * \brief Superclass of all emitters
 */
class Emitter : public NoriObject {
public:

    //生成采样点
    virtual Color3f sample(const Mesh* mesh, EmitterQueryRecord &eRec, Sampler* &sample) const = 0;

    //计算pdf
    virtual float pdf(const Mesh* mesh, const EmitterQueryRecord &eRec) const = 0;

    //返回emitted radiance，存在一个Color3f通道中
    virtual Color3f eval(const EmitterQueryRecord &eRec) const = 0;

    /**
     * \brief Return the type of object (i.e. Mesh/Emitter/etc.) 
     * provided by this instance
     * */
    EClassType getClassType() const { return EEmitter; }

};

NORI_NAMESPACE_END
