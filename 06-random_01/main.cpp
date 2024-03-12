#include "driver.hpp"


float random (Vec2 st) {
    return fract(sin(dot(st,Vec2{12.9898f,78.233f}))* 43758.5453123);
}

class MyShader : public BaseShader {
public:
  std::tuple<Vec<int, 2>, Color, float>
  process_fragment(const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t*)get_global_data(4)),float(*(size_t*)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion/canvas_size;
    Color color = gray(random( st ));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}