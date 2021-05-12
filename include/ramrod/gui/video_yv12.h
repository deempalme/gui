#ifndef RAMROD_GUI_GUI_VIDEO_YV12_H
#define RAMROD_GUI_GUI_VIDEO_YV12_H

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

    class video_yv12 : public gui::element
    {
    public:
      video_yv12(gui::window *window,
                 const int resolution_x, const int resolution_y,
                 const float width = 256.0f, const float height = 128.0f,
                 const float position_x = 0, const float position_y = 0,
                 const int z_index = 0, const std::size_t tab_index = 0);
      virtual ~video_yv12();

      void invert_y_axis();

      void paint();

      bool resolution(const int width, const int height);
      int resolution_x();
      int resolution_y();

      GLuint texture_id();

      bool upload_data(const void *data, const std::size_t offset = 0);
      bool upload_data(const void *y_data, const std::size_t y_offset,
                       const void *u_data, const std::size_t u_offset,
                       const void *v_data, const std::size_t v_offset);

    private:
      GLsizei resolution_x_, resolution_y_;
      GLsizei resolution_;
      GLsizei quarter_resolution_;

      gl::pixel_buffer *pbo_;
      std::size_t read_pbo_;
      gl::texture *y_tex_, *u_tex_, *v_tex_;

      bool inverted_;

      inline static gl::shader *shader_ = nullptr;
      inline static std::size_t elements_ = 0;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_VIDEO_YV12_H
