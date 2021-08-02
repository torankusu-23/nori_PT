#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include <nori/warp.h>

NORI_NAMESPACE_BEGIN

class AOIntegrator : public Integrator {
public:
    AOIntegrator(const PropertyList &props) {
    }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        /* Find the surface that is visible in the requested direction */
        Intersection its;//碰撞点（可见点
        if (!scene->rayIntersect(ray, its))//光线判定场景交点
            return Color3f(0.0f);//无交点返回黑色
        
        Point3f x = its.p;
        uint32_t sample_count = 2;//ao采样点个数
        float visibility = 0.f;

        for(uint32_t i = 0; i < sample_count; i++) {
            Point3f sample_local = Warp::squareToCosineHemisphere(sampler->next2D());
            Point3f sample_world = its.shFrame.toWorld(sample_local).normalized();//采样方向的世界空间
            float vis = 0.0f;
            if(!scene->rayIntersect(Ray3f(its.p + sample_world * 0.00001f, sample_world))) {
                vis = 1.0f;
            }
            visibility += vis;
        }

        return Color3f(visibility / sample_count);
    }

    std::string toString() const {
        return "AOIntegrator[]";
    }

};

NORI_REGISTER_CLASS(AOIntegrator, "ao");
NORI_NAMESPACE_END