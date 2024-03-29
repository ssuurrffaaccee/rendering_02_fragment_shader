#pragma once
#include <assert.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "interval.hpp"
#include "vec.hpp"
/*
contract:
  +x = right direction
  +y = down direction
  width on x axis
  height on y axis
*/
class SSAADepthImage {
 public:
  SSAADepthImage(size_t width, size_t height, size_t SSAA)
      : SSAA_width_{width * SSAA}, SSAA_height_{height * SSAA}, SSAA_{SSAA} {
    CHECK(SSAA >= 1)
    data_ = std::make_shared<std::vector<Color>>();
    depth_buffer_ = std::make_shared<std::vector<float>>();
    data_->resize(SSAA_width_ * SSAA_height_, Color{0.0f, 0.0f, 0.0f});
    depth_buffer_->resize(SSAA_width_ * SSAA_height_,
                          std::numeric_limits<float>::lowest());
  }
  void set(size_t x, size_t y, const Color &color) {
    data_->operator[](y * SSAA_width_ + x) = color;
  }
  Color get(size_t x, size_t y) {
    return data_->operator[](y * SSAA_width_ + x);
  }
  void dump(const std::string name) {
    std::ofstream ofs(name + ".ppm");
    size_t width_ = SSAA_width_ / SSAA_;
    size_t height_ = SSAA_height_ / SSAA_;
    ofs << "P3\n" << width_ << ' ' << height_ << "\n255\n";
    size_t SSAA_sample_num = SSAA_ * SSAA_;
    for (size_t y = 0; y < height_; y++) {
      for (size_t x = 0; x < width_; x++) {
        Color color = Color{0.0f, 0.0f, 0.0f};
        for (size_t i = 0; i < SSAA_; i++) {
          for (size_t j = 0; j < SSAA_; j++) {
            color += get(x * SSAA_ + i, y * SSAA_ + j) / SSAA_sample_num;
          }
        }
        auto clamped_color = clamp(color, 0.0f, 1.0f);
        ofs << int(clamped_color[0] * 255) << ' ' << int(clamped_color[1] * 255)
            << ' ' << int(clamped_color[2] * 255) << '\n';
      }
    }
  }
  void set_with_depth_test(size_t x, size_t y, const Color &color,
                           float depth) {
    if (depth > depth_buffer_->operator[](y * SSAA_width_ + x)) {
      depth_buffer_->operator[](y * SSAA_width_ + x) = depth;
      data_->operator[](y * SSAA_width_ + x) = color;
    }
  }
  size_t SSAA_width_;
  size_t SSAA_height_;
  size_t SSAA_;
  std::shared_ptr<std::vector<Color>> data_;
  std::shared_ptr<std::vector<float>> depth_buffer_;
};