
#include "driver.hpp"

float random(Vec2 st) {
  return fract(sin(dot(st, Vec2{12.9898f, 78.233f})) * 43758.5453123);
}
float noise(Vec2 st) {
  Vec2 i = floor(st);  // get the integer coords
  Vec2 f = fract(st);  // get the fractional coords
  // Four corners in 2D of a tile
  float a = random(i);
  float b = random(i + Vec2{1.0f, 0.0f});
  float c = random(i + Vec2{0.0f, 1.0f});
  float d = random(i + Vec2{1.0f, 1.0f});
  Vec2 u = f * f * (3.0f - 2.0f * f);
  return mix(mix(a, b, 1.0f - u[0]), mix(c, d, 1.0 - u[0]), 1.0f - u[1]);
}
Square<float,2> rotate2d(float angle){
    return Square<float,2>{cosf(angle),-sinf(angle),
                sinf(angle),cosf(angle)};
}

float lines(Vec2 pos, float b){
    float scale = 10.0;
    pos *= scale;
    return smooth_step(0.0,0.5f+b*0.5f,abs((sin(pos[0]*3.1415)+b*2.0))*.5);
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
    // st.y *= u_resolution.y/u_resolution.x;
    Vec2 pos = st*Vec2{3.0f,10.0f};
    float pattern = pos[0];
    // Add noise
    pos = mvdot<float,2>(rotate2d( noise(pos)),pos);
    // pos = mvdot<float,2>(rotate2d(0.3f),pos);
    // Draw lines
    pattern = lines(pos,0.5);
    Color color = Color{pattern,pattern,pattern};
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}