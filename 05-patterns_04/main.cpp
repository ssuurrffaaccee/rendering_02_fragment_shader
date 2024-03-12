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

float framework1(const Vec2& st) {
  auto _st = st;
  _st -= 0.5;
  static float base{0.0f};
  static float half_width{0.05};
  float u = smooth_range(base-half_width, base + half_width, _st[0]);
  float v = smooth_range(base-half_width, base + half_width, _st[1]);
  return u + v;
}

// float box(const Vec2& st) {
//   auto _st = st;
//   _st -= 0.5;
//   float u = (step(0.0f, _st[0]) - 0.5f) * 2.0f;
//   float v = (step(0.0f, _st[1]) - 0.5f) * 2.0f;
//   return u * v;
// }
float box(Vec2 &st, float dx, float dy) {
  float u = step(-dx, st[0]) - step(dx, st[0]);
  float v = step(-dy, st[1]) - step(dy, st[1]);
  return u * v;
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
    Vec2 grid = tile(st, 10.f);
    float gray_color{0.0f};
    gray_color += framework1(grid);
    grid = rotate2D(grid, float(M_PI) * 0.25f);
    grid = grid - Vec2{0.5f,0.5f};
    gray_color += box(grid,0.2f,0.2f);
    gray_color *= 1.0f-box(grid,0.1f,0.1f);
    // color = gray(box(grid));
    // color += gray(circle(grid,0.25));
    // color += gray(framework(grid));
    // color += Color{grid[0], grid[1], 0.0f};
    color = gray(1.0f-gray_color);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}