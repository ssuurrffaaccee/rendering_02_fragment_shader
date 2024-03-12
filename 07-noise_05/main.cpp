#include "driver.hpp"

Vec2 random2(Vec2 st) {
    st = Vec2{dot(st,Vec2{127.1,311.7}),
              dot(st,Vec2{269.5,183.3}) };
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}
// Gradient Noise by Inigo Quilez - iq/2013
// https://www.shadertoy.com/view/XdXGW8
float noise(Vec2 st) {
    Vec2 i = floor(st);
    Vec2 f = fract(st);
    Vec2 u = f*f*(3.0-2.0*f);
    Vec2 a = random2(i + Vec2{0.0f,0.0f}); 
    Vec2 b = random2(i + Vec2{1.0f,0.0f}); 
    Vec2 c = random2(i + Vec2{0.0f,1.0f}); 
    Vec2 d = random2(i + Vec2{1.0f,1.0f}); 
    Vec2 cur_to_a = f - Vec2{0.0f,0.0f};
    Vec2 cur_to_b = f - Vec2{1.0f,0.0f};
    Vec2 cur_to_c = f - Vec2{0.0f,1.0f};
    Vec2 cur_to_d = f - Vec2{1.0f,1.0f};
    return mix(mix(dot(a,cur_to_a),dot(b,cur_to_b),1.0f-u[0]),mix(dot(c,cur_to_c),dot(d,cur_to_d),1.0f-u[0]),1.0-u[1]);
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
    Vec3 color = Vec3{0.0f,0.0f,0.0f};
    float random_value = noise(st)*0.5f+0.5f;
    color = Vec3{random_value,random_value,random_value};
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}