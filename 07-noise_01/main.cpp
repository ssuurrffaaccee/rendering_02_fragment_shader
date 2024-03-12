#include "driver.hpp"

float random(Vec2 st) {
  return fract(sin(dot(st, Vec2{12.9898f, 78.233f})) * 43758.5453123);
}
float noise(Vec2 st) {
  Vec2 i = floor(st);  // get the integer coords
  Vec2 f = fract(st);  // get the fractional coords
  // Four corners in 2D of a tile
  float a = random(i);
  float b = random(i + Vec2{1.0f, 0.0f});
  float c = random(i + Vec2{0.0f, 1.0f});
  float d = random(i + Vec2{1.0f, 1.0f});
  Vec2 u = f * f * (3.0f - 2.0f * f);
  return mix(mix(a, b, 1.0f - u[0]), mix(c, d, 1.0 - u[0]), 1.0f - u[1]);
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
    st *= 5.0f;  // Scale the coordinate system by 10
    float n = noise(st);
    Color color = gray(n);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}