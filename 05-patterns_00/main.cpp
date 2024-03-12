#include "driver.hpp"
Vec2 tile(const Vec2& st, float zoom) {
  auto new_st = st * zoom;
  return fract(new_st);
}

float circle(const Vec2& st, float radius) {
  Vec2 pos = st - 0.5f;
  radius *= 0.75f;
  return 1.f - smooth_step(radius - (radius * 0.05f), radius + (radius * 0.05f),
                           dot(pos, pos) * 3.14f);
}

float circlePattern(const Vec2& st, float radius) {
  return circle(st + Vec2{0.f, -.5f}, radius) +
         circle(st + Vec2{0.f, .5f}, radius) +
         circle(st + Vec2{-.5f, 0.f}, radius) +
         circle(st + Vec2{.5f, 0.f}, radius);
}
class MyShader : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData& frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t*)get_global_data(4)),
                            float(*(size_t*)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Color color = gray(0.0f);
    Vec2 grid1 = tile(st, 7.f);
    color += mix(Vec3{0.075, 0.114, 0.329}, Vec3{0.973, 0.843, 0.675},
                 circlePattern(grid1, 0.23) - circlePattern(grid1, 0.01));

    Vec2 grid2 = tile(st, 3.0f);
    color = mix(Vec3{0.761, 0.247, 0.102}, color, circlePattern(grid2, 0.2)) -
            circlePattern(grid2, 0.05);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}