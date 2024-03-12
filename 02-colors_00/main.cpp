#include "driver.hpp"

float plot(const Vec2 &st, float x_min, float x_max, float y_min, float y_max,
           const std::function<float(float)> &f) {
  float x = mix(x_min, x_max, st[0]);
  float y = f(x);
  float normlized_y = 1.0f - smooth_step(y_min, y_max, y);
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}
float plot(const Vec2 &st, float y) {
  float normlized_y = 1.0f - y;
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}


Vec3 color_A = Vec3{0.149f, 0.141f, 0.912f};
Vec3 color_B = Vec3{1.000f, 0.833f, 0.224f};
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
    Color color = Color{0.0f, 0.0f, 0.0f};

    Vec3 pct = gray(st[0]);
    pct[0] = smooth_step(0.0, 1.0, st[0]);
    pct[1] = sin(st[0] * M_PI);
    pct[2] = pow(st[0], 0.5);

    color = mix(color_A, color_B, 1.0f - pct);

    // Plot transition lines for each channel
    color = mix(color, Vec3{1.0, 0.0, 0.0}, 1.0f - plot(st, pct[0]));
    color = mix(color, Vec3{0.0, 1.0, 0.0}, 1.0f - plot(st, pct[1]));
    color = mix(color, Vec3{0.0, 0.0, 1.0}, 1.0f - plot(st, pct[2]));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}