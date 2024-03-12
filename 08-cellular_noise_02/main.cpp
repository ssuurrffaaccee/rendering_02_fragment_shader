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
    st *= 5.0f;           // Scale the coordinate system by 10
    Vec2 i_st = floor(st); // get the integer coords
    Vec2 f_st = fract(st); // get the fractional coords
    Vec3 color = Vec3{0.0f, 0.0f, 0.0f};

    float m_dist = 1.;  // minimum distance
    for (int y = -1; y <= 1; y++) {
      for (int x = -1; x <= 1; x++) {
        // Neighbor place in the grid
        Vec2 neighbor = Vec2{float(x), float(y)};

        // Random position from current + neighbor place in the grid
        Vec2 point = random2(i_st + neighbor);

        // // Animate the point
        //       point = 0.5 + 0.5*sin(u_time + 6.2831*point);

        // Vector between the pixel and the point
        Vec2 diff = neighbor + point - f_st;

        // Distance to the point
        float dist = length(diff);

        // Keep the closer distance
        m_dist = std::min(m_dist, dist);
      }
    }
    // Draw the min distance (distance field)
    color += m_dist;
    // Draw the min distance (distance field)
    color -= step(0.7f, abs(sin(50.0 * m_dist))) * .3;
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}