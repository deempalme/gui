#ifndef RAMROD_GUI_GUI_MANAGER_H
#define RAMROD_GUI_GUI_MANAGER_H

#include <map>

#include "ramrod/gl/shader.h"
#include "ramrod/gui/type_events.h"
#include "ramrod/gui/types.h"

namespace ramrod {
  namespace gl {
    class buffer;
    class frame_buffer;
    class texture;
    class uniform_buffer;
  }

  namespace gui {
    class element;
    class font_loader;
    class input;
    class window;

    class gui_manager
    {
    public:
      gui_manager(gui::window *window);
      ~gui_manager();
      /**
       * @brief Inserting a new element into the painting queue
       *
       * This is done automatically for every new element: input_text, text, image, line, etc.
       *
       * @param new_element Pointer to the element that will be added
       *
       * @return New element's ID
       */
      std::size_t add_element(gui::element *new_element);
      /**
       * @brief Getting the new tab index for an gui::input element
       *
       * @param input Input element that will have a new tab index
       *
       * @return New input's tab index
       */
      std::size_t add_tab_index(gui::input *input, const std::size_t new_tab_index);
      int add_z_index(gui::element *element, const int new_z_index);

      const gui::pixel_id &calculate_ids(int x, int y);

      void bind_shader(const GLuint shader_id);
      void bind_texture(const GLuint texture_id);
      std::size_t hovered_element();
      /**
       * @brief Getting the ID of the last added element
       *
       * @return Numeric ID or the last added element
       */
      std::size_t last_element_id();
      /**
       * @brief Getting the last added tab index
       *
       * @return The last added numeric tab index
       */
      std::size_t last_tab_id();
      /**
       * @brief Getting the last added texture id or zero if there is none
       *
       * @return The last added numeric texture id
       */
      GLuint last_texture_id();
      /**
       * @brief Getting the last added z index
       *
       * @return The last added numeric z index
       */
      int last_z_index();
      std::size_t modify_tab_index(gui::input *input, const std::size_t new_tap_index);
      int modify_z_index(gui::element *element, const int new_z_index);

      bool remove_element(gui::element *old_element);
      bool remove_tab_index(gui::input *old_input);
      bool remove_z_index(gui::element *old_element);

      bool new_sprite(const std::string &sprite_path);
      GLuint sprite_id();
      float sprite_height();
      float sprite_width();
      GLuint sprite_shader();

    protected:
      virtual void initialize();
      // Mouse's events
      virtual void mouse_down_event(const gui::mouse_event::button &event);
      virtual void mouse_move_event(const gui::mouse_event::move &event);
      virtual void mouse_up_event(const gui::mouse_event::button &event);
      /**
       * @brief Painting the scren
       */
      virtual void paint();

      virtual void resize(const float width, const float height);

      void pre_paint();
      void post_paint();

    private:
      gui::window *window_;
      // Font container: key is the font's name
      std::map<const std::string, gui::font_loader> fonts_;
      std::uint16_t global_font_size_;

      gl::shader *text_shader_;
      unsigned int t_u_color_, t_u_size_;

      gl::shader *sprite_shader_;
      unsigned int s_u_parent_id_, s_u_whole_;

      gl::shader *frame_shader_;

      gl::frame_buffer *frame_buffer_;
      gl::texture *back_texture_, *front_texture_;
      gl::uniform_buffer *scene_uniform_;
      gl::buffer *unitary_buffer_;

      gl::texture *sprite_;
      float sprite_height_;
      float sprite_width_;

      bool using_elements_;

      std::map<const std::size_t, gui::element*> elements_;
      std::multimap<const std::size_t, gui::input*> tab_list_;
      std::multimap<const int, gui::element*> z_index_list_;

      std::size_t last_element_id_;
      std::size_t last_tab_id_;
      int last_z_index_;
      GLuint last_shader_id_;
      GLuint last_texture_id_;
      GLuint last_bound_texture_id_;

      gui::pixel_id ids_;
      int width_, height_;
      float width_factor_, height_factor_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_MANAGER_H
