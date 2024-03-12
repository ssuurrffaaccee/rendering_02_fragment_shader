#include "driver.hpp"

float plot(const Vec2 &st, float x_min, float x_max, float y_min, float y_max,
           const std::function<float(float)> &f) {
  float x = mix(x_min, x_max, st[0]);
  float y = f(x);
  float normlized_y = 1.0f - smooth_step(y_min, y_max, y);
  return step(normlized_y, st[1]) - step(normlized_y + 0.005f, st[1]);
}

float random(Vec2 st) {
  return fract(sin(dot(st, Vec2{12.9898f, 78.233f})) * 43758.5453123);
}

Vec2 truchet_pattern(Vec2 st, float index){
    index = fract(((index-0.5)*2.0));//[0,1] ot [-1,1]
    if (index > 0.75) {
        st = Vec2{1.0f,1.0f} - st; // flip x and y, 0.25 probability
    } else if (index > 0.5) {
        st = Vec2{1.0f-st[0],st[1]}; // flip x, 0.25 probability
    } else if (index > 0.25) {
        st = 1.0-Vec2{1.0f-st[0],st[1]};  // flip y,0.25 probability
    } 
    return st;  //no flip 0.25 probability
}

class MyShader : public BaseShader {
public:
  std::tuple<Vec<int, 2>, Color, float>
  process_fragment(const FragmentStageData &frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t *)get_global_data(4)),
                            float(*(size_t *)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    st *= 10.0f;           // Scale the coordinate system by 10
    Vec2 ipos = floor(st); // get the integer coords
    Vec2 fpos = fract(st); // get the fractional coords
    Vec2 tile = truchet_pattern(fpos, random( ipos ));
    float gray_color = step(tile[0],tile[1]);
    Color color = gray(gray_color);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}