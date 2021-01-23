#ifndef RAMROD_GUI_GUI_MANAGER_H
#define RAMROD_GUI_GUI_MANAGER_H

#include <map>

#include "ramrod/gui/font_family.h"
#include "ramrod/gui/types.h"

namespace ramrod {
  namespace gl {
    class frame_buffer;
    class pixel_buffer;
    class shader;
    class texture;
    class uniform_buffer;
  }

  namespace gui {
    class element;
    class input;
    class window;

    class gui_manager
    {
    public:
      gui_manager(gui::window *parent);
      ~gui_manager();
      /**
       * @brief Activates an input with a determined `id`
       *
       * @param id Input's id to look for
       *
       * @return `false` if `id` not found
       */
      bool activate_input(const std::size_t id);
      /**
       * @brief Activates a specific input
       *
       * If `nullptr` is choosen then it will deactivate the current active input
       *
       * @param input Input's pointer to activate
       *
       * @return `false` if input is `nullptr`
       */
      bool activate_input(gui::input *input);
      /**
       * @brief Getting the input that is active (ready to accept an keyboard input)
       *
       * @return Pointer to the active `gui::input`
       */
      gui::input *active_input();
      /**
       * @brief Adds a new font family to the GUI
       *
       * @param font_distance_path Path towards the image file that contains
       *                           the font's distance field
       * @param font_info_path Path towards the font's info file with extension .fnt
       *
       * @return `false` if font family was not found or it has errors
       */
      bool add_font(const std::string &font_distance_path,
                    const std::string &font_info_path);
      /**
       * @brief Deletes a specific font family from the GUI
       *
       * @param font_name Font's name string as described in gui::font_family::font_name()
       *
       * @return `false` if the found was not found
       */
      bool delete_font(const std::string &font_name);
      /**
       * @brief Reading the pixel value with coordinates (x, y)
       *
       * @param x Horizontal coordinate in pixels
       * @param y Vertical coordinate in pixels
       */
      const identifier *read_pixel(const int x, const int y);
      /**
       * @brief Selecting the next `gui::input` in the tab queue
       *
       * @return Pointer to the next `gui::input` in the tab queue or `nullptr` if
       *         there is none
       */
      gui::input *tab_next();
      /**
       * @brief Selecting the previous `gui::input` in the tab queue
       *
       * @return Pointer to the previous `gui::input` in the tab queue or `nullptr` if
       *         there is none
       */
      gui::input *tab_previous();

      // :::::::::::::::::::::::::::::::::::: Static functions ::::::::::::::::::::::::::::::::::::
      /**
       * @brief Inserting a new gui::input element into the tab queue with a defined index
       *
       * Attention: if called more than once for the same `input` then it will create
       * repeated indexes in the tab's list, better cal `move_tab_input` to change
       * the input's tab index.
       *
       * @param input Input element that will be inserted into the tab queue
       * @param index The tab index that this `gui::input` will have or 0 to autodefine it
       *
       * @return Input's tab index or 0 if `input` is `nullptr`
       */
      static std::size_t create_new_tab_input(gui::input *input, const std::size_t index = 0);
      /**
       * @brief Gets the pointer to a font family
       *
       * @param font_name String name of the font family as shown in gui::font_family::font_name()
       *                  or empty to get the default font.
       *
       * @return `nullptr` if font was not found
       */
      static gui::font_family *get_font(const std::string &font_name);
      /**
       * @brief Inserting a new element into the painting queue
       *
       * This is done automatically for every new element: input_text, text, image, line, etc.
       *
       * @param new_element Pointer to the element that will be added
       *
       * @return New element's ID
       */
      static std::size_t insert_new_element(gui::element *new_element);
      /**
       * @brief Getting the ID of the last added element
       *
       * @return Numeric ID or the last added element
       */
      static std::size_t last_element_id();
      /**
       * @brief Getting the last added tab index
       *
       * @return The last added numeric tab index
       */
      static std::size_t last_tab_id();
      /**
       * @brief Moves the tab index of a `gui::input`
       *
       * This will also remove all old tab indexes that point to this `gui::input`
       *
       * @param input Pointer to the `gui::input` that will be moved
       * @param new_index New tab index
       *
       * @return `true` if found and changed it`
       */
      static bool move_tab_input(gui::input *input, const std::size_t new_index);
      /**
       * @brief Removing a GUI element from the drawing queue
       *
       * @param old_element Pointer to the `gui::element` that will be removed
       *
       * @return `false` if `gui::element` not found or `nullptr`
       */
      static bool remove_element(gui::element *old_element);
      /**
       * @brief Removing a tab input from the tab queue
       *
       * Removes an `gui::input` from the tab queue, so pressing the button tab would
       * not select/activate the removed `gui::input`
       *
       * @param old_input Pointer to the `gui::input` that will be removed
       *
       * @return `false` if `gui::input` not found or `nullptr`
       */
      static bool remove_tab_index(gui::input *old_input);
      static const gl::shader *sprite_shader();

    protected:
      virtual void initialize();
      /**
       * @brief Loading the default fonts
       *
       * This function loads the default fonts, override it to change the default fonts
       */
      virtual void load_default_font();
      virtual void main_initialize();
      /**
       * @brief Painting the scren
       */
      virtual void paint();

      void resize();

    private:
      gui::window *parent_;

      gl::uniform_buffer *window_binding_;

      std::uint16_t global_font_size_;
      gl::shader *text_shader_;
      unsigned int t_u_color_, t_u_parent_id_, t_u_size_;

      gui::input *active_input_;
      std::map<const std::size_t, gui::input*>::iterator current_tab_;

      gl::texture *sprite_;
      unsigned int s_u_background_color_, s_u_border_color_, s_u_id_;
      gl::texture *window_ids_;
      gl::frame_buffer *window_elements_;
      gl::pixel_buffer *window_buffer_;
      gui::pixel<float> pixel_data_;
      gui::identifier identifier_;

      // Font container: key is the font's name
      inline static gl::shader *sprite_shader_ = nullptr;
      inline static gui::font_family *default_font_ = nullptr;
      inline static std::map<const std::string, gui::font_family> fonts_;
      inline static std::map<const std::size_t, gui::element*> elements_;
      inline static std::size_t last_element_id_ = 0;
      inline static std::size_t last_tab_id_ = 0;
      inline static std::multimap<const std::size_t, gui::input*> tab_list_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_MANAGER_H
