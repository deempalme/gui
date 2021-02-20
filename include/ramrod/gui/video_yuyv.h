#ifndef RAMROD_GUI_GUI_VIDEO_YUYV_H
#define RAMROD_GUI_GUI_VIDEO_YUYV_H

#include <cstdint>

#include "ramrod/gui/element.h"

namespace ramrod {
  namespace gl {
    class buffer;
    class pixel_buffer;
    class shader;
    class texture;
  }

  namespace gui {
    class window;

    class video_yuyv : public gui::element
    {
    public:
      video_yuyv(gui::window *window, const float resolution_x, const float resolution_y,
                 const float width = 256.0f, const float height = 128.0f,
                 const float position_x = 0, const float position_y = 0,
                 const int z_index = 0, const std::size_t tab_index = 0);
      virtual ~video_yuyv();

      void paint();

      bool resolution(const float width, const float height);
      float resolution_x();
      float resolution_y();

      GLuint texture_id();

      bool upload_data(void *data, std::size_t buffer_size, const std::size_t offset = 0);

    private:
      float resolution_x_;
      float resolution_y_;
      GLsizei i_resolution_y_;
      GLsizei half_resolution_x_;
      GLsizei half_resolution_;

      gl::pixel_buffer *pbo_;
      std::size_t read_pbo_;
      gl::texture *yuyv_;

      inline static gl::shader *shader_ = nullptr;
      inline static GLuint u_yuyv_size_ = 0;
      inline static std::size_t elements_ = 0;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_VIDEO_YUYV_H
