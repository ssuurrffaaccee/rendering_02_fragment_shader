#include "driver.hpp"

Vec2 random2(Vec2 p) {
  return fract(
      sin(Vec2{dot(p, Vec2{127.1, 311.7}), dot(p, Vec2{269.5, 183.3})}) *
      43758.5453);
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
    Color color = Vec3{0.0f,0.0f,0.0f};
    // Cell positions
    Vec2 point[4];
    point[0] = Vec2{0.83f,0.75f};
    point[1] = Vec2{0.60f,0.07f};
    point[2] = Vec2{0.28f,0.64f};
    point[3] = Vec2{0.31f,0.26f};

    float m_dist = 1.;  // minimum distance
    Vec2 m_point;        // minimum position

    // Iterate through the points positions
    for (int i = 0; i < 5; i++) {
        float dist = length(st-point[i]);
        if ( dist < m_dist ) {
            // Keep the closer distance
            m_dist = dist;
            // Kepp the position of the closer point
            m_point = point[i];
        }
    }
    // tint acording the closest point position
    color[0] = m_point[0];
    color[1] = m_point[1];
    color[0] -= abs(sin(80.0*m_dist))*0.07;
    color[1] -= abs(sin(80.0*m_dist))*0.07;
    color += 1.0f-step(0.02f, m_dist);
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}