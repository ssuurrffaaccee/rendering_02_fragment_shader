#include "driver.hpp"
class MyShader0 : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec2 pos = st * 2.f - 1.f;
    float r = length(pos) * 2.0;
    float a = atan2(pos[1], pos[0]);

    float f = cos(a * 3.f);
    // f = abs(cos(a*3.f));
    // f = abs(cos(a*2.5))*.5+.3;
    // f = abs(cos(a*12.)*sin(a*3.))*.8+.1;
    // f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;

    auto color = gray(1. - smooth_step(f, f + 0.02f, r));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my0, MyShader0)
class MyShader1 : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec2 pos = st * 2.f - 1.f;
    float r = length(pos) * 2.0;
    float a = atan2(pos[1], pos[0]);
    float f = fabs(cos(a * 3.0f));
    auto color = gray(1.f - smooth_step(f, f + 0.02f, r));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my1, MyShader1)
class MyShader2 : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec2 pos = st * 2.f - 1.f;
    float r = length(pos) * 2.0f;
    float a = atan2(pos[1], pos[0]);

    float f = fabs(cos(a * 2.5f)) * .5f + .3f;
    // float f = abs(cos(a*12.)*sin(a*3.))*.8+.1;
    // float f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;

    auto color = gray(1. - smooth_step(f, f + 0.02f, r));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my2, MyShader2)
class MyShader3 : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec2 pos = st * 2.f - 1.f;
    float r = length(pos) * 2.0f;
    float a = atan2(pos[1], pos[0]);

    float f = fabs(cos(a * 12.f) * sin(a * 3.f)) * .8f + .1f;
    // float f = smoothstep(-.5,1., cos(a*10.))*0.2+0.5;

    auto color = gray(1. - smooth_step(f, f + 0.02f, r));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my3, MyShader3)
class MyShader4 : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec2 pos = st * 2.f - 1.f;
    float r = length(pos) * 2.0f;
    float a = atan2(pos[1], pos[0]);

    float f = smooth_step(-.5f, 1.f, cos(a * 10.f)) * 0.2f + 0.5f;

    auto color = gray(1. - smooth_step(f, f + 0.02f, r));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my4, MyShader4)
int main() {
  run("my0", "xxxx0");
  run("my1", "xxxx1");
  run("my2", "xxxx2");
  run("my3", "xxxx3");
  run("my4", "xxxx4");
  return 0;
}