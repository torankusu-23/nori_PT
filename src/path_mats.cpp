#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include <nori/emitter.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class path_mats : public Integrator
{
public:
  path_mats(const PropertyList &props) {}

  Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const
  {
    Color3f result(0.f);
    Intersection its;
    //无交点 返回黑色
    if (!scene->rayIntersect(ray, its))
    {
      return result;
    }

    //交点是光源
    if (its.mesh->isEmitter())
    {
      EmitterQueryRecord eRec(ray.o, its.p, its.shFrame.n);
      result += its.mesh->getEmitter()->eval(eRec);
    }

    float total_eta = 1.f;
    Intersection x(its);
    Ray3f r(ray);
    Color3f wait_albedo(1.f); //暂存fr * cosTheta / pdf的结果，等待后续的Li
    int least_recursion = 4;    //如果不设置最少递归次数，玻璃球显示会有问题
    float maxComp = 1.f;             //最大通量，通量越小贡献越小，计算概率越小，1spp情况比较简单


    //完全随机采样BSDF，迭代方法
    while (least_recursion > 0 || (sampler->next1D() < fmin(maxComp * total_eta * total_eta, 0.99)))
    {

      BSDFQueryRecord bRec(x.shFrame.toLocal(-r.d));
      
      //albedo = sample() = fr * cosTheta / pdf;
      Color3f albedo = x.mesh->getBSDF()->sample(bRec, sampler->next2D());
  
      wait_albedo *= albedo;

      Ray3f ro(x.p, x.shFrame.toWorld(bRec.wo)); //反射光线
      Intersection next_x;

      if (!scene->rayIntersect(ro, next_x))
      { //打空了
        return result;
      }
      else if (next_x.mesh->isEmitter())
      { //打到光源了
        EmitterQueryRecord eRec(x.p, next_x.p, next_x.shFrame.n);

        Color3f radiance = next_x.mesh->getEmitter()->eval(eRec);

        result += radiance * wait_albedo;
      }

      x = next_x;
      r.o = ro.o;
      r.d = ro.d;

      if (least_recursion <= 0)
      {
        maxComp = wait_albedo.maxCoeff();
        total_eta *= bRec.eta;
      } else {
        least_recursion--;
      }
    }

    return result;
  }

  std::string toString() const
  {
    return "path_mats[]";
  }
};

NORI_REGISTER_CLASS(path_mats, "path_mats");
NORI_NAMESPACE_END