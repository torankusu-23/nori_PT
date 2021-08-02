#include <nori/integrator.h>
#include <nori/scene.h>

NORI_NAMESPACE_BEGIN

class SimpleIntegrator : public Integrator {
public:
    SimpleIntegrator(const PropertyList &props) {
        lightPos = props.getPoint("position");
        lightEnergy = props.getColor("energy");
    }

    Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const {
        /* Find the surface that is visible in the requested direction */
        Intersection its;//碰撞点（可见点
        if (!scene->rayIntersect(ray, its))//光线判定场景交点
            return Color3f(0.0f);//无交点返回黑色
        
        Point3f x = its.p;
        Point3f p = lightPos;
        auto vec = p - x;
        auto dir = vec.normalized();
        //可见性项
        if(scene->rayIntersect(Ray3f(x + vec * 0.00001f, vec))) {//只有一个ray参数，是shadow ray

            return Color3f(0.0f);//光源不可见，则ray颜色为0
        }
        float visibility = 1;//保留一下，虽然此处没什么意义
        const auto& frame = its.shFrame;//法线图
        auto cosTheta = frame.cosTheta(frame.toLocal(dir));//costheta,其实就是z坐标,z = r * costheta，是切线空间
        auto s = std::pow((x - p).norm(), 2.0f);
        auto li = (lightEnergy.x() / (4 * M_PI * M_PI)) * ((std::max(0.0f, cosTheta)) / s) * visibility;
        return li;
    }

    std::string toString() const {
        return "SimpleIntegrator[]";
    }

private:
    Point3f lightPos;
    Color3f lightEnergy;
};

NORI_REGISTER_CLASS(SimpleIntegrator, "simple");
NORI_NAMESPACE_END