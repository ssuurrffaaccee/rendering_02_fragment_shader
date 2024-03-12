#include "driver.hpp"

float plot(const Vec2 &st, float x_min, float x_max, float y_min, float y_max,
           const std::function<float(float)> &f) {
  float x = mix(x_min, x_max, st[0]);
  float y = f(x);
  float normlized_y = 1.0f - smooth_step(y_min, y_max, y);
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}
float amplitude = 1.f;
float frequency = 1.f;
float line(float x) {
  float y = sin(x * frequency);
  y += sin(x * frequency * 2.1) * 4.5;
  y += sin(x * frequency * 1.72) * 4.0;
  y += sin(x * frequency * 2.221) * 5.0;
  y += sin(x * frequency * 3.1122) * 2.5;
  y *= amplitude * 0.06;
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
    color = color * plot(st, -5.0f, 5.0f, -1.02f, 1.0f,
                         &line);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}