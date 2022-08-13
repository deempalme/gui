#ifndef RAMROD_GUI_IMAGE_STREAM_H
#define RAMROD_GUI_IMAGE_STREAM_H

#include "ramrod/gui/element.h"

namespace ramrod {
  namespace gl {
    class pixel_buffer;
    class texture;
  }

  namespace gui {
    class window;

    class image_stream : public gui::element
    {
    public:
      image_stream(gui::window *window, const float width, const float height,
                   const int number_of_channels = 3, const float position_x = 0,
                   const float position_y = 0, const int z_index = 0,
                   const std::size_t tab_index = 0);

      virtual ~image_stream();

      char *map(const int width = -1, const int height = -1);

      void paint();

      // TODO: maybe implement a texture_allocate options for data_format and internal_format?
      void texture_id(const GLuint new_texture_id);

      bool texture_size(const int width, const int height);

      void unmap();

      bool upload_data(const void *data, std::size_t buffer_size);

    protected:

    private:
      gl::pixel_buffer *pbo_;
      std::size_t read_pbo_;
      gl::texture *texture_;
      int width_;
      int height_;
      int resolution_;
      GLenum data_format_;
      GLenum internal_format_;
      int channels_;

      char *pointer_;
      bool mapped_;

    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_IMAGE_STREAM_H
