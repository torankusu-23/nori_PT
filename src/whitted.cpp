#include <nori/integrator.h>
#include <nori/scene.h>
#include <nori/sampler.h>
#include <nori/emitter.h>
#include <nori/bsdf.h>
NORI_NAMESPACE_BEGIN

class whitted : public Integrator {
public:

  whitted(const PropertyList& props) {}

  Color3f Li(const Scene* scene, Sampler* sampler, const Ray3f& ray) const {

    Intersection its;
    //无交点 返回黑色
    if (!scene->rayIntersect(ray, its)) {
      return Color3f(0.0f);
    }

    //交点是光源
    Color3f lightAreaPoint(0.0f);
    if (its.mesh->isEmitter()) {
      EmitterQueryRecord eRec(ray.o, its.p, its.shFrame.n);
      lightAreaPoint = its.mesh->getEmitter()->eval(eRec);
    }

    //否则就是正常着色点
    Color3f result(0.0f);

    //1: 对光源随机采样,不适用dielectirc和mirror材质-----------------------------------------------------------------------
    // uint32_t sample_count = 2;

    // std::vector<Mesh*> m_meshes = scene->getMeshes();//先要获取光源mesh
    // Mesh* Arealight = nullptr;
    // for(auto x: m_meshes) {//单mesh光源情况
    //   if(x->isEmitter())
    //   Arealight = x;
    // }
    
    // for(uint32_t i = 0; i < sample_count; ++i) {

    //   //构造emitter的记录
    //   EmitterQueryRecord eRec(its.p);

    //   //采样光源点，返回的albedo = Le / pdf
    //   Color3f albedo = Arealight->getEmitter()->sample(Arealight, eRec, sampler);

    //   Intersection y_its;
    //   Ray3f rayo(its.p, eRec.wi, Epsilon, __FLT_MAX__);//反射光

    //   if(scene->rayIntersect(rayo, y_its) && y_its.mesh != Arealight) {//可见性测试
    //     continue;
    //   }
      
    //   //算BSDF
    //   BSDFQueryRecord bRec(its.shFrame.toLocal(-ray.d), its.shFrame.toLocal(eRec.wi), ESolidAngle);

    //   //算fr
    //   Color3f fr = its.mesh->getBSDF()->eval(bRec);
    //   //算G项,后两项包含了dA到dw的转换
    //   float G = abs(its.shFrame.n.dot(eRec.wi)) * abs(eRec.n.dot(-eRec.wi)) / (eRec.y - eRec.x).squaredNorm();

    //   result += fr * albedo * G;
    // }

    // return result / sample_count  + lightAreaPoint;


    //2: 完整的whitted-style光追算法(加上递归)-----------------------------------------------------------------------
    //打到了diffuse，就对光源采样并返回，和1一样
    if(its.mesh->getBSDF()->isDiffuse()) {
      uint32_t sample_count = 2;

      std::vector<Mesh*> m_meshes = scene->getMeshes();//先要获取光源mesh
      Mesh* Arealight = nullptr;
      for(auto x: m_meshes) {//单mesh光源情况
        if(x->isEmitter())
        Arealight = x;
      }
      
      for(uint32_t i = 0; i < sample_count; ++i) {

        //构造emitter的记录
        EmitterQueryRecord eRec(its.p);

        //采样光源点，返回albedo = Le / pdf
        Color3f albedo = Arealight->getEmitter()->sample(Arealight, eRec, sampler);

        Intersection y_its;
        Ray3f rayo(its.p, eRec.wi, Epsilon, __FLT_MAX__);//反射光

        if(scene->rayIntersect(rayo, y_its) && y_its.mesh != Arealight) {//可见性测试
          continue;
        }
        
        
        //算BSDF
        BSDFQueryRecord bRec(its.shFrame.toLocal(-ray.d), its.shFrame.toLocal(eRec.wi), ESolidAngle);
        
        //算fr
        Color3f fr = its.mesh->getBSDF()->eval(bRec);
        //算G项
        float G = its.shFrame.n.dot(eRec.wi) * eRec.n.dot(-eRec.wi) / (eRec.y - eRec.x).squaredNorm();

        result += fr * albedo * G;
      }
      
      return result / sample_count  + lightAreaPoint;

    } else if(!its.mesh->isEmitter()){ //打到非diffuse，就要算反射折射，并且递归计算了
      if(sampler->next1D() >= 0.95) {  //俄罗斯轮盘赌

        return Color3f(0.0f);
      } else {

        BSDFQueryRecord bRec(its.shFrame.toLocal(-ray.d));
        Color3f c = its.mesh->getBSDF()->sample(bRec, sampler->next2D());

        Ray3f ro(its.p, its.shFrame.toWorld(bRec.wo), Epsilon, __FLT_MAX__);
        Color3f recursion = Li(scene, sampler, ro);
        return recursion * c / 0.95;

      }
    }
   
    return Color3f(0.0f);
  }


  std::string toString() const {
    return "whitted[]";
  }
};

NORI_REGISTER_CLASS(whitted, "whitted");
NORI_NAMESPACE_END