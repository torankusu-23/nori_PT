#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include <nori/emitter.h>
#include <nori/bsdf.h>

NORI_NAMESPACE_BEGIN

class path_ems : public Integrator
{
public:
  path_ems(const PropertyList &props) {}

  Color3f Li(const Scene *scene, Sampler *sampler, const Ray3f &ray) const
  {
    Color3f result(0.f);
    Intersection its;
    //无交点 返回黑色 程序有偏移量bug
    if (!scene->rayIntersect(ray, its))
    {
      return result;
    }

    //统计光源
    std::vector<Mesh *> Meshes = scene->getMeshes();
    std::vector<Mesh *> light_source;
    float totalArea = 0.f;
    for (auto x : Meshes)
    {
      if (x->isEmitter())
      {
        light_source.emplace_back(x);
        totalArea += x->getpdf().getSum();
      }
    }
    uint32_t ls_nums = light_source.size();

    //否则就是正常着色点
    float total_eta = 1.f;
    Intersection x(its);
    Ray3f r(ray);
    Color3f wait_albedo(1.f);
    uint32_t least_recursion = 0; //如果不设置最少递归次数，玻璃球的光出不去
    uint32_t MAX_DEPTH = 5;
    float maxComp = 1.f;
    bool lastSpecular = false;

    while (least_recursion < MAX_DEPTH || (sampler->next1D() < fmin(maxComp * total_eta * total_eta, 0.99)))
    {
      Color3f dir_light(0.f);
      //Le
      if (x.mesh->isEmitter())
      {
        EmitterQueryRecord eRec(r.o, x.p, x.shFrame.n);
        if (least_recursion == 0 || lastSpecular)
          result += x.mesh->getEmitter()->eval(eRec) * wait_albedo;
      }
      if(!x.mesh->getBSDF()->isDiffuse()) lastSpecular = true;
      else lastSpecular = false;

      //------------------------直接光 direct light---------------------------
      //均匀随机采样光源
      uint32_t index = sampler->next1D() * ls_nums;
      Mesh *area_light = light_source[index];

      //光源离散采样三角形
      EmitterQueryRecord eRec(x.p);
      Color3f Le = area_light->getEmitter()->sample(area_light, eRec, sampler) * ls_nums; //radiance/pdf，pdf要考虑光源个数

      Intersection y;
      Ray3f rayo(x.p, eRec.wi);

      if (scene->rayIntersect(rayo, y) && y.mesh == area_light)
      {
        if (eRec.n.dot(-eRec.wi) > 0.f)
        {
          BSDFQueryRecord bRec(x.shFrame.toLocal(-r.d), x.shFrame.toLocal(eRec.wi), ESolidAngle);

          eRec.pdf /= ls_nums; //考虑光源个数

          Color3f fr = x.mesh->getBSDF()->eval(bRec);
          float G = abs(x.shFrame.n.dot(eRec.wi)) * abs(eRec.n.dot(-eRec.wi)) / (eRec.y - eRec.x).squaredNorm();

          Color3f dir = fr * G * Le;
          dir_light += wait_albedo * dir;
        }
      }

      //------------------------间接光 indirect light---------------------------

      BSDFQueryRecord bRec(x.shFrame.toLocal(-r.d.normalized()));
      Color3f albedo = x.mesh->getBSDF()->sample(bRec, sampler->next2D());
      Ray3f ro(x.p, x.shFrame.toWorld(bRec.wo)); //反射光线
      Intersection next_x;

      if (!scene->rayIntersect(ro, next_x))
      {
        break;
      }

      x = next_x;
      r.o = ro.o;
      r.d = ro.d;

      if (least_recursion >= MAX_DEPTH)
      {
        total_eta *= bRec.eta;
        maxComp = wait_albedo.maxCoeff(); //太小了就没必要继续弹射了
      }
      else
      {
        least_recursion++;
      }

      float q = total_eta * total_eta * maxComp;
      wait_albedo *= albedo / q;

      result += dir_light / q;
    }

    return result;
  }

  std::string toString() const
  {
    return "path_ems[]";
  }
};

NORI_REGISTER_CLASS(path_ems, "path_ems");
NORI_NAMESPACE_END