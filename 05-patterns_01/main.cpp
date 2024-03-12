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
    Vec2 grid = tile(st, 7.f);
    color = gray(circle(grid,0.25));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}