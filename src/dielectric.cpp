/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include <nori/bsdf.h>
#include <nori/frame.h>

NORI_NAMESPACE_BEGIN

/// Ideal dielectric BSDF
class Dielectric : public BSDF {
public:
    Dielectric(const PropertyList &propList) {
        /* Interior IOR (default: BK7 borosilicate optical glass) */
        m_intIOR = propList.getFloat("intIOR", 1.5046f);

        /* Exterior IOR (default: air) */
        m_extIOR = propList.getFloat("extIOR", 1.000277f);
    }

    Color3f eval(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return Color3f(0.0f);
    }

    float pdf(const BSDFQueryRecord &) const {
        /* Discrete BRDFs always evaluate to zero in Nori */
        return 0.0f;
    }

    Color3f sample(BSDFQueryRecord &bRec, const Point2f &sample) const {
        bRec.measure = EDiscrete;
        //入射角余弦
        float cosThetaI = Frame::cosTheta(bRec.wi);
        //nori自带的fresenl函数,固定角度下的菲涅尔系数，且自带了完全反射判定
        float kr = fresnel(cosThetaI, m_extIOR, m_intIOR);
        //完全反射
        if (sample.x() < kr) {
            bRec.wo = Vector3f(-bRec.wi.x(), -bRec.wi.y(), bRec.wi.z());
            bRec.eta = 1.f;
            return Color3f(1.f);
        } else {//折射
            //考虑了内部射出情况
            bRec.eta = cosThetaI >= 0 ? m_extIOR / m_intIOR : m_intIOR / m_extIOR;
            //翻转法线，统一方向
            Normal3f n = cosThetaI < 0 ? Normal3f(0.f, 0.f, -1.f) : Normal3f(0.f, 0.f, 1.f);
            cosThetaI = abs(cosThetaI);

            //出射角余弦，利用了Snell law：eta = sinThetaI / sinThetaO
            float cosThetaO = sqrt(1 - bRec.eta * bRec.eta * fmax(0.f, 1.f - cosThetaI * cosThetaI));
            //数学方法
            bRec.wo = bRec.eta * -bRec.wi + (bRec.eta * cosThetaI - cosThetaO) * n;
            bRec.wo.normalize();
            return Color3f(1.f);
        }
    }

    std::string toString() const {
        return tfm::format(
            "Dielectric[\n"
            "  intIOR = %f,\n"
            "  extIOR = %f\n"
            "]",
            m_intIOR, m_extIOR);
    }
private:
    float m_intIOR, m_extIOR;
};

NORI_REGISTER_CLASS(Dielectric, "dielectric");
NORI_NAMESPACE_END
