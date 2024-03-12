#include "driver.hpp"

Vec2 skew (Vec2 st) {
    Vec2 r = Vec2{0.0f,0.0f};
    r[0] = 1.1547*st[0];
    r[1] = st[1]+0.5*r[0];
    return r;
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
    st *= 10.0f;           // Scale the coordinate system by 10
    Vec2 ipos = floor(st); // get the integer coords
    //Vec2 fpos = fract(st); // get the fractional coords
    Vec2 fpos = fract(skew(st));
    Color color = Color{fpos[0],fpos[1],0.0f};
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}