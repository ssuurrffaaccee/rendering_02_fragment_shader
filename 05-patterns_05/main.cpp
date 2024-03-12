#include "driver.hpp"
Vec2 brick_tile(Vec2 st, float zoom){
    st *= zoom;
    // Here is where the offset is happening
    // st[1]%2==0 ? 0 : 1
    st[0] += step(1.0f, std::fmod(st[1],2.0)) * 0.5;

    return fract(st);
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
    st= brick_tile(st,10.0f);
    Color color = Color{st[0],st[1],0.0f};
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}