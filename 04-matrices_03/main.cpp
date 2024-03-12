#include "driver.hpp"

// YUV to RGB matrix
Square3 yuv2rgb = Square3{1.0f, 0.0f, 1.13983f,
                          1.0f, -0.39465f, -0.58060f,
                          1.0f, 2.03211f, 0.0f};

// RGB to YUV matrix
Square3 rgb2yuv = Square3{0.2126f, 0.7152f, 0.0722f,
                          -0.09991f, -0.33609f,0.43600f,
                          0.615f, -0.5586f, -0.05639f};
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
    Vec3 color = gray(0.0f);
    // UV values goes from -1 to 1
    // So we need to remap st (0.0 to 1.0)
    st -= 0.5;  // becomes -0.5 to 0.5
    st *= 2.0;  // becomes -1.0 to 1.0
    // we pass st as the y & z values of
    // a three dimensional vector to be
    // properly multiply by a 3x3 matrix
    color = mvdot<float, 3>(yuv2rgb, Vec3{0.5f, st[0], st[1]});
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}