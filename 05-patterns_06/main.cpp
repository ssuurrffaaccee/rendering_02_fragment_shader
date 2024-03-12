#include "driver.hpp"
Vec2 tile(const Vec2& st, float zoom) {
  auto new_st = st * zoom;
  return fract(new_st);
}

float circle(const Vec2& st, float radius) {
  Vec2 pos = st - 0.5f;
  radius *= 0.75f;
  return 1.f - smooth_step(radius - (radius * 0.05f), radius + (radius * 0.05f),
                           dot(pos, pos) * 3.14f);
}
Square2 rotate2d(float angle) {
  return Square2{float(cos(angle)), float(-sin(angle)), float(sin(angle)),
                 float(cos(angle))};
}
Vec2 rotate2D(const Vec2& st, float angle) {
  auto _st = st;
  _st -= 0.5;
  _st = mvdot<float, 2>(Square2{float(cos(angle)), float(-sin(angle)),
                                float(sin(angle)), float(cos(angle))},
                        _st);
  _st += 0.5;
  return _st;
}
float framework(const Vec2& st) {
  static float base{0.5f};
  float u = step(base - 0.01, st[0]) - step(base + 0.01, st[0]);
  float v = step(base - 0.01, st[1]) - step(base + 0.01, st[1]);
  return u + v;
}

float smooth_range(float l, float r, float u) {
  return smooth_step(l, l + 0.01, u) - smooth_step(r, r + 0.01, u);
}

// float framework1(const Vec2& st) {
//     auto _st = st;
//     _st -= 0.5;
//   static float base{0.0f};
//   float u = step(base, _st[0]) - step(base + 0.05, _st[0]);
//   float v = step(base, _st[1]) - step(base + 0.05, _st[1]);
//   return u+v;
// }

// float framework1(const Vec2& st) {
//   auto _st = st;
//   _st -= 0.5;
//   static float base{0.0f};
//   static float half_width{0.05};
//   float u = smooth_range(base - half_width, base + half_width, _st[0]);
//   float v = smooth_range(base - half_width, base + half_width, _st[1]);
//   return u + v;
// }

float box(const Vec2& st) {
  auto _st = st;
  _st -= 0.5;
  float u = (step(0.0f, _st[0]) - 0.5f) * 2.0f;
  float v = (step(0.0f, _st[1]) - 0.5f) * 2.0f;
  return u * v;
}
// float box(Vec2& st, float dx, float dy) {
//   float u = step(-dx, st[0]) - step(dx, st[0]);
//   float v = step(-dy, st[1]) - step(dy, st[1]);
//   return u * v;
// }
Vec2 rotate_tile_pattern(Vec2 st) {
  auto _st = st;
  //  Scale the coordinate system by 2x2
  _st *= 2.0f;

  //  Give each cell an index number
  //  according to its position
  float index = 0.0f;
  index += step(1.0f, std::fmod(_st[0], 2.0f));
  index += step(1.0f, std::fmod(_st[1], 2.0f)) * 2.0f;

  //      |
  //  2   |   3
  //      |
  //--------------
  //      |
  //  0   |   1
  //      |

  // Make each cell between 0.0 - 1.0
  _st = fract(_st);

  // Rotate each cell according to the index
  if (index == 1.0) {
    //  Rotate cell 1 by 90 degrees
    _st = rotate2D(_st, M_PI * -0.5);
  } else if (index == 2.0) {
    //  Rotate cell 2 by -90 degrees
    _st = rotate2D(_st, M_PI * 0.5);
  } else if (index == 3.0) {
    //  Rotate cell 3 by 180 degrees
    _st = rotate2D(_st, M_PI);
  }

  return _st;
}

class MyShader : public BaseShader {
 public:
  std::tuple<Vec<int, 2>, Color, float> process_fragment(
      const FragmentStageData& frag) override {
    float depth = frag.second[2];
    Vec2 canvas_postiion = to_float(frag.first);
    Vec2 canvas_size = Vec2{float(*(size_t*)get_global_data(4)),
                            float(*(size_t*)get_global_data(4))};
    ///////////////////////////////////
    Vec2 st = canvas_postiion / canvas_size;
    Color color = gray(0.0f);
    st = tile(st, 3.f);
    st = tile(st,2.0);
    st = rotate_tile_pattern(st);
    color = gray(step(st[0], st[1]));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}