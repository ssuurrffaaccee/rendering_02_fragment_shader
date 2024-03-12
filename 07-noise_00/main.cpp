#include "driver.hpp"

float plot(const Vec2 &st, float x_min, float x_max, float y_min, float y_max,
           const std::function<float(float)> &f) {
  float x = mix(x_min, x_max, st[0]);
  float y = f(x);
  float normlized_y = 1.0f - smooth_step(y_min, y_max, y);
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}
float random(float t) { return fract(sin(t * 8.233f) * 43758.5453123); }
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
    // st *= 10.0f;           // Scale the coordinate system by 10
    // Vec2 ipos = floor(st); // get the integer coords
    // Vec2 fpos = fract(st); // get the fractional coords
    Color r_color = Color{1.0f, 0.0f, 0.0f};
    Color g_color = Color{0.0f, 1.0f, 0.0f};
    Color b_color = Color{0.0f, 0.0f, 1.0f};
    Color color = Color{0.0f, 0.0f, 0.0f};
    color += r_color * plot(st, -5.0f, 5.0f, -5.0f, 5.00f, [](float x) {
               float i = floor(x);  // integer
               float f = fract(x);  // fraction
               return random(i);
             });
    color += g_color * plot(st, -5.0f, 5.0f, -5.0f, 5.00f, [](float x) {
               float i = floor(x);  // integer
               float f = fract(x);  // fraction
               return mix(random(i), random(i + 1.0), 1.0f - f)+2.0f;
             });
    color +=
        b_color * plot(st, -5.0f, 5.0f, -5.0f, 5.00f, [](float x) {
          float i = floor(x);  // integer
          float f = fract(x);  // fraction
          return mix(random(i), random(i + 1.0), 1.0 - smooth_step(0., 1., f))-2.0f;
        });
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}