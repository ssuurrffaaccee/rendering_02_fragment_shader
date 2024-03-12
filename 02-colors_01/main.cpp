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

Vec3 rgb2hsb(Vec3 c) {
  // r0g1b2 x0y1z2w3
  Vec4 K = Vec4{0.0f, -1.0 / 3.0f, 2.0 / 3.0f, -1.0f};
  Vec4 p = mix(Vec4{c[2], c[1], K[3], K[2]}, Vec4{c[1], c[2], K[0], K[1]},
               step(c[2], c[1]));
  Vec4 q = mix(Vec4{p[0], p[1], p[3], c[0]}, Vec4{c[0], p[1], p[2], p[0]},
               step(p[0], c[0]));
  float d = q[0] - std::min(q[3], q[1]);
  float e = 1.0e-10f;
  return Vec3{std::abs(float(q[2] + (q[3] - q[1]) / (6.0 * d + e))),
              d / (q[0] + e), q[0]};
}

//  Function from Iñigo Quiles
//  https://www.shadertoy.com/view/MsS3Wc
Vec3 hsb2rgb(Vec3 c) {
  Vec3 rgb =
      clamp(abs(mod(c[0] * 6.0 + Vec3{0.0f, 4.0f, 2.0f}, 6.0f) - 3.0f) - 1.0f,
            0.0, 1.0);
  rgb = rgb * rgb * (3.0 - 2.0 * rgb);
  return c[2] * mix(gray(1.0), rgb, 1.0f - c[1]);
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
    Color color = Color{0.0f, 0.0f, 0.0f};

    // We map x (0.0 - 1.0) to the hue (0.0 - 1.0)
    // And the y (0.0 - 1.0) to the brightness
    color = hsb2rgb(Vec3{st[0],1.0f,st[1]});
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}