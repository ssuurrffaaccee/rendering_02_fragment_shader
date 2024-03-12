#include "driver.hpp"

float plot(const Vec2 &st, float x_min, float x_max, float y_min, float y_max,
           const std::function<float(float)> &f) {
  float x = mix(x_min, x_max, st[0]);
  float y = f(x);
  float normlized_y = 1.0f - smooth_step(y_min, y_max, y);
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}
float random(float t) { return fract(sin(t * 8.233f) * 43758.5453123); }
// Properties
const int octaves = 4;
float lacunarity = 2.0f;
float gain = 0.5f;
float line(float x) {
  // Initial values
  float amplitude = 0.5f;
  float frequency = 1.0f;
  float y = 0.0f;
  // Loop of octaves
  for (int i = 0; i < octaves; i++) {
    y += amplitude * sin(frequency * x);
    frequency *= lacunarity;
    amplitude *= gain;
  }
  return y;
}

class MyShader : public BaseShader {
public:
  std::tuple<Vec<int, 2>, Color, float>
  process_fragment(const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Color color = Color{1.0f, 0.0f, 0.0f};
    color = color * plot(st, -10.0f, 10.0f, -1.0f, 1.0f,
                         [](float x) { return line(x); });
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}