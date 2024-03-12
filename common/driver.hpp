#include <vector>

#include "camera.hpp"
#include "cube.hpp"
#include "depth_image.hpp"
#include "renderer.hpp"
#include "simple_shader.hpp"
#include "sphere.hpp"
ModelPool build_model_pool() {
  ModelPool model_pool;
  Model square;
  {
    square.points_ =
        std::vector<Vec3>{Vec3{-1.0f, -1.0f, -0.01f}, Vec3{1.0f, -1.0f, -0.01f},
                          Vec3{-1.0f, 1.0f, -0.01f}, Vec3{1.0f, 1.0f, -0.01f}};
    square.triangles_.push_back(std::tuple<int, int, int>(0, 1, 3));
    square.triangles_.push_back(std::tuple<int, int, int>(3, 2, 0));
    square.attributes_.resize(square.triangles_.size());
  }
  model_pool.models_["square"] = std::move(square);
  return model_pool;
}
std::vector<VertexStageData> model_to_vertices(const Model &model) {
  std::vector<VertexStageData> vertices;
  for (int i = 0; i < model.points_.size(); i++) {
    VertexStageData data;
    // homo_position
    data.insert(data.end(), model.points_[i].begin(), model.points_[i].end());
    data.push_back(1.0f);
    vertices.push_back(std::move(data));
  }
  return vertices;
}
void simple_render_homoposition(Renderer *renderer,
                                const PerspectiveCamera &camera,
                                SSAADepthImage &image,
                                const ModelPool &model_pool,
                                const Instance &instance,
                                const std::string &shader_name) {
  auto &shader_keeper = ShaderKeeper::get();
  std::shared_ptr<Shader> shader = shader_keeper.build(shader_name);
  renderer->shader_ = shader.get();
  renderer->shader_->set_global_data(1, (void *)&camera.clipping_planes_);
  renderer->shader_->set_global_data(2,
                                     (void *)&camera.world_to_view_transform_);
  renderer->shader_->set_global_data(3, (void *)&camera.projection_transform_);
  renderer->color_buffer_ = image.data_->data();
  renderer->depth_buffer_ = image.depth_buffer_->data();
  renderer->frame_width_ = image.SSAA_width_;
  renderer->frame_height_ = image.SSAA_height_;
  renderer->shader_->set_global_data(4, (void *)&image.SSAA_width_);
  renderer->shader_->set_global_data(5, (void *)&image.SSAA_height_);
  auto iter = model_pool.models_.find(instance.name_);
  CHECK(iter != model_pool.models_.end())
  auto model_transform = instance.to_model_transform();
  renderer->shader_->set_global_data(0, (void *)&model_transform);
  auto vertices = model_to_vertices(iter->second);
  renderer->is_draw_framework_ = instance.is_frame_;
  renderer->enable_back_face_culling_ = false;
  renderer->render(vertices, iter->second.triangles_, iter->second.attributes_);
}
void run(const std::string &shader_name, const std::string &dump_name) {
  try {
    Vec3 world_centor = Vec3{0.0f, 0.0f, 0.0f};
    size_t height = 1000;
    size_t width = 1000;
    PerspectiveCameraConfig camera_config;
    camera_config.position_ = Vec3{0.0f, 0.0f, 1.0f};
    camera_config.forward_ = world_centor - camera_config.position_;
    camera_config.up_ = Vec3{0.0f, 1.0f, 0.0f};
    camera_config.far_ = -1000.0f;
    camera_config.near_ = -1.0f;
    camera_config.left_ = -1.0f;
    camera_config.right_ = 1.0f;
    camera_config.top_ = 1.0f;
    camera_config.down_ = -1.0f;
    PerspectiveCamera camera;
    camera.init(camera_config);
    size_t SSAA = 1;
    SSAADepthImage image{width, height, SSAA};
    ModelPool model_pool = build_model_pool();
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
    {
      auto instance = Instance{"square", Vec3{0.0f, 0.0f, 0.0f},
                               Vec3{1.0f, 1.0f, 1.0f}, eye<float, 3>(), false};
      simple_render_homoposition(renderer.get(), camera, image, model_pool,
                                 instance, shader_name);
    }
    image.dump(dump_name);
  } catch (MyExceptoin &e) {
    std::cout << e.what() << "\n";
  }
}

void run(const std::string &shader_name) { run(shader_name, "xxxx"); }