#include "driver.hpp"
class MyShader : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec3 color = gray(0.0f);
    Vec2 st = canvas_postiion / canvas_size;
    // bottom-left
    Vec2 bl;
    bl[0] = step(0.1f, st[0]);
    bl[1] = step(0.1f, st[1]);
    float pct = bl[0] * bl[1];
    // top-right
    Vec2 tr;
    tr[0] = step(0.1f, 1.0f - st[0]);
    tr[1] = step(0.1f, 1.0f - st[1]);
    pct *= tr[0] * tr[1];
    color = gray(pct);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}