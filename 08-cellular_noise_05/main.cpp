#include "driver.hpp"

Vec2 random2(Vec2 p) {
  return fract(
      sin(Vec2{dot(p, Vec2{127.1, 311.7}), dot(p, Vec2{269.5, 183.3})}) *
      43758.5453);
}
Vec3 voronoi(Vec2 x, float rnd ) {
    Vec2 n = floor(x);
    Vec2 f = fract(x);

    // first pass: regular voronoi
    Vec2 mg, mr;
    float md = 8.0;
    for (int j=-1; j<=1; j++ ) {
        for (int i=-1; i<=1; i++ ) {
            Vec2 g = Vec2{float(i),float(j)};
            Vec2 o = random2( n + g )*rnd;
            // #ifdef ANIMATE
            // o = 0.5 + 0.5*sin( u_time + 6.2831*o );
            // #endif
            Vec2 r = g + o - f;
            float d = dot(r,r);

            if( d<md ) {
                md = d;
                mr = r;
                mg = g;
            }
        }
    }

    // second pass: distance to borders
    md = 8.0;
    for (int j=-2; j<=2; j++ ) {
        for (int i=-2; i<=2; i++ ) {
            Vec2 g = mg + Vec2{float(i),float(j)};
            Vec2 o = random2(n + g)*rnd;
            // #ifdef ANIMATE
            // o = 0.5 + 0.5*sin( u_time + 6.2831*o );
            // #endif
            Vec2 r = g + o - f;

            if( dot(mr-r,mr-r)>0.00001 ){
              md = std::min( md, dot( 0.5*(mr+r), normalize(r-mr) ) );
            }
        }
    }
    return Vec3{md, mr[0],mr[1]};
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
    float d = dot(st-0.5f,st-0.5f);
    Vec3 c = voronoi( 20.0f*st, pow(d,0.4f) );

    // borders
    color = mix( Vec3{1.0f,1.0f,1.0f}, color, smooth_step( 0.01f, 0.02f, c[0] ) );
    // feature poin
    float dd = length( Vec2{c[1],c[2]} );
    color += Vec3{1.0f,1.0f,1.0f}*(1.0-smooth_step( 0.0, 0.1, dd));
    //////////////////////////////////
    return std::make_tuple(frag.first, color, depth);
  }
};
REGISTER_SHADER(Shader, my, MyShader)

int main() {
  run("my");
  return 0;
}