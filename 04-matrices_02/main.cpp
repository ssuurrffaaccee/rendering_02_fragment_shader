#include "driver.hpp"
float box(Vec2 &st, float dx, float dy) {
  float u = step(-dx, st[0]) - step(dx, st[0]);
  float v = step(-dy, st[1]) - step(dy, st[1]);
  return u * v;
}

float cross(Vec2 &st, float dx, float dy) {
  return box(st, dx, dy) + box(st, dy, dx);
}
Square2 rotate2d(float _angle) {
  return Square2{float(cos(_angle)), float(-sin(_angle)), float(sin(_angle)),
                 float(cos(_angle))};
}

Square2 scale(const Vec2 &_scale) {
  return Square2{_scale[0], 0.0f, 0.0f, _scale[1]};
}
class MyShader : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvassize = Vec2{float(*(size_t *)get_global_data(4)),
                           float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvassize;
    Vec2 to_centor = st - Vec2{0.5f, 0.5f};
    to_centor = mvdot<float, 2>(scale(Vec2{2.0f, 0.5f}), to_centor);
    auto color = Vec3{std::abs(to_centor[0]), std::abs(to_centor[1]), 0.0f};
    color += gray(cross(to_centor, 0.025, 0.1));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}