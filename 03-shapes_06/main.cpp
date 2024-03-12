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
    Vec2 st = canvas_postiion / canvas_size;
    // Remap the space to -1. to 1.
    st = st * 2. - 1.;
    // Number of sides of your shape
    int N = 3;

    // Angle and radius from the current pixel
    float a = atan2(st[0], st[1]) + M_PI;
    float r = 2.0f * M_PI / float(N);

    // Shaping function that modulate the distance
    float d = cos(floor(.5f + a / r) * r - a) * length(st);

    auto color = gray(1.0 - smooth_step(.4f, .41f, d));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}