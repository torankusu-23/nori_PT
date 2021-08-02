/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob
*/

#include <nori/bsdf.h>
#include <nori/frame.h>
#include <nori/warp.h>
#include <nori/dpdf.h>

NORI_NAMESPACE_BEGIN

class Microfacet : public BSDF {
public:
    Microfacet(const PropertyList &propList) {
        /* RMS surface roughness */
        m_alpha = propList.getFloat("alpha", 0.1f);

        /* Interior IOR (default: BK7 borosilicate optical glass) */
        m_intIOR = propList.getFloat("intIOR", 1.5046f);

        /* Exterior IOR (default: air) */
        m_extIOR = propList.getFloat("extIOR", 1.000277f);

        /* Albedo of the diffuse base material (a.k.a "kd") */
        m_kd = propList.getColor("kd", Color3f(0.5f));

        /* To ensure energy conservation, we must scale the 
           specular component by 1-kd. 

           While that is not a particularly realistic model of what 
           happens in reality, this will greatly simplify the 
           implementation. Please see the course staff if you're 
           interested in implementing a more realistic version 
           of this BRDF. */
        m_ks = 1 - m_kd.maxCoeff();
    }

    /// Evaluate the BRDF for the given pair of directions
    Color3f eval(const BSDFQueryRecord &bRec) const {
        if(bRec.wi.z() <= 0 || bRec.wo.z() <= 0.f) return 0.f;
    	//照着公示一步一步来
        //半程向量
        Vector3f wh = (bRec.wi + bRec.wo) / (bRec.wi + bRec.wo).norm();
        //向量余弦
        float cosThetaI = Frame::cosTheta(bRec.wi);
        float cosThetaO = Frame::cosTheta(bRec.wo);
        float cosThetaH = Frame::cosTheta(wh);

        //NDF，角度转换避免反三角函数带来的开销
        float azimutal = INV_PI;
        float longitudinal = exp(- (1 - cosThetaH * cosThetaH) / (cosThetaH * cosThetaH * m_alpha * m_alpha))
                            / (m_alpha * m_alpha * cosThetaH * cosThetaH * cosThetaH); 
        float D = azimutal * longitudinal;

        //Fresnel
        float F = fresnel(wh.dot(bRec.wi), m_extIOR, m_intIOR);
        
        //G
        float G1 = 0.f, G2 = 0.f;
        if(bRec.wi.dot(wh) / cosThetaI > 0) {
            float b =  cosThetaI / (sqrt(1 - cosThetaI * cosThetaI) * m_alpha);
            if(b < 1.6) {
                G1 = (3.535 * b + 2.181 * b * b) / (1 + 2.276 * b + 2.577 * b * b);
            } else {
                G1 = 1.f;
            }
        }
        if(bRec.wo.dot(wh) / cosThetaO > 0) {
            float b =  cosThetaO / (sqrt(1 - cosThetaO * cosThetaO) * m_alpha);
            if(b < 1.6) {
                G2 = (3.535 * b + 2.181 * b * b) / (1 + 2.276 * b + 2.577 * b * b);
            } else {
                G2 = 1.f;
            }
        }
        float G = G1 * G2;

        Color3f fr = m_kd * INV_PI + m_ks * D * F * G / (4 * cosThetaI * cosThetaO * cosThetaH);

        return fr;
    }

    /// Evaluate the sampling density of \ref sample() wrt. solid angles
    float pdf(const BSDFQueryRecord &bRec) const {
        if(bRec.wi.z() <= 0 || bRec.wo.z() <= 0.f) return 0.f;
        //if(bRec.wi.z() <= 0.f) std::cout<<"???"<<std::endl;
        //D
        Vector3f wh = (bRec.wi + bRec.wo) / (bRec.wi + bRec.wo).norm();

        float cosThetaH = Frame::cosTheta(wh);
        float cosThetaO = Frame::cosTheta(bRec.wo);
        float azimutal = INV_PI;
        float longitudinal = exp(- (1 - cosThetaH * cosThetaH) / (cosThetaH * cosThetaH * m_alpha * m_alpha))
                            / (m_alpha * m_alpha * cosThetaH * cosThetaH * cosThetaH); 

        float D = azimutal * longitudinal;

        float J = 4 * wh.dot(bRec.wo);
        //跟公式来
        
        float pdf =  m_ks * D / J + (1 - m_ks) * cosThetaO * INV_PI;

        return pdf;
    }

    /// Sample the BRDF
    Color3f sample(BSDFQueryRecord &bRec, const Point2f &_sample) const {
        DiscretePDF m_cdf;
        if (Frame::cosTheta(bRec.wi) <= 0) //背刺
            return Color3f(0.0f);

        if(_sample.x() > m_ks) { 
            Point2f reuse((_sample.x() - m_ks) / (1 - m_ks), _sample.y());

            //diffuse
            bRec.measure = ESolidAngle;

            bRec.wo = Warp::squareToCosineHemisphere(reuse);

            bRec.eta = 1.f;
        } else { 
            Point2f reuse((m_ks - _sample.x()) / m_ks, _sample.y());
            //specular
            //根据Beckmann模型采样微表面法线

            Vector3f n = Warp::squareToBeckmann(reuse, m_alpha).normalized();

            //根据微表面法线算反射方向，用镜面反射原理
            float nor = bRec.wi.dot(n);//wi在n的投影长度
            bRec.wo = -bRec.wi + 2 * nor * n;

            if(bRec.wo.z() <= 0.f) return Color3f(0.f);
            bRec.measure = ESolidAngle;

            bRec.eta = 1.f; 
        }
        // Note: Once you have implemented the part that computes the scattered
        // direction, the last part of this function should simply return the
        // BRDF value divided by the solid angle density and multiplied by the
        // cosine factor from the reflection equation, i.e.

        Color3f albedo = eval(bRec) * Frame::cosTheta(bRec.wo) / pdf(bRec);


        return albedo;
    }

    bool isDiffuse() const {
        /* While microfacet BRDFs are not perfectly diffuse, they can be
           handled by sampling techniques for diffuse/non-specular materials,
           hence we return true here */
        return true;
    }

    std::string toString() const {
        return tfm::format(
            "Microfacet[\n"
            "  alpha = %f,\n"
            "  intIOR = %f,\n"
            "  extIOR = %f,\n"
            "  kd = %s,\n"
            "  ks = %f\n"
            "]",
            m_alpha,
            m_intIOR,
            m_extIOR,
            m_kd.toString(),
            m_ks
        );
    }
private:
    float m_alpha;
    float m_intIOR, m_extIOR;
    float m_ks;
    Color3f m_kd;
};

NORI_REGISTER_CLASS(Microfacet, "microfacet");
NORI_NAMESPACE_END
