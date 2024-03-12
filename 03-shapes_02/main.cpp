#include "driver.hpp"
float circle(Vec2 &_st, float _radius) {
  Vec2 dist = _st - Vec2{0.5f, 0.5};
  return 1.0f - smooth_step(_radius - (_radius * 0.01), _radius + (_radius * 0.01),
                         dot(dist, dist) * 4.0);
}
class MyShader : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Vec3 color = gray(circle(st,0.9));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}