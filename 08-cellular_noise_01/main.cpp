#include "driver.hpp"

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
    Vec3 color = Vec3{0.0f, 0.0f, 0.0f};
    // Cell positions
    Vec2 point[4];
    point[0] = Vec2{0.83, 0.75};
    point[1] = Vec2{0.60, 0.07};
    point[2] = Vec2{0.28, 0.64};
    point[3] = Vec2{0.31, 0.26};
    float m_dist = 1.0f; // minimum distance
    // Iterate through the points positions
    for (int i = 0; i < 5; i++) {
      float dist = length(st - point[i]);
      // Keep the closer distance
      m_dist = std::min(m_dist, dist);
    }
    // Draw the min distance (distance field)
    color += m_dist;
    // Draw the min distance (distance field)
    color -= step(0.7f,abs(sin(50.0*m_dist)))*.3;
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}