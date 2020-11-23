#ifndef RAMROD_GUI_GUI_MANAGER_H
#define RAMROD_GUI_GUI_MANAGER_H

#include <map>

namespace ramrod {
  namespace gl {
    class shader;
    class texture;
  }

  namespace gui {
    class element;
    class font_loader;
    class input;
    class window;

    class gui_manager
    {
    public:
      gui_manager(gui::window *parent);
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
      std::size_t insert_new_element(gui::element *new_element);
      /**
       * @brief Getting the new tab index for an gui::input element
       *
       * @param input Input element that will have a new tab index
       *
       * @return New input's tab index
       */
      std::size_t create_new_tab_input(gui::input *input);
      bool remove_element(gui::element *old_element);
      bool remove_tab_index(gui::input *old_input);

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

    protected:
      virtual void initialize();
      /**
       * @brief Painting the scren
       *
       * @param force ´true´ to force a redraw of all GUI elements
       */
      virtual void paint(bool force = true);

    private:
      gui::window *parent_;

      gl::texture *sprite_;
      // Font container: key is the font's name
      std::map<const std::string, gui::font_loader> fonts_;
      std::uint16_t global_font_size_;

      std::map<const std::size_t, gui::element*> elements_;
      std::multimap<const std::size_t, gui::input*> tab_list_;

      gl::shader *text_shader_;
      unsigned int t_u_color_, t_u_size_;
      gl::shader *image_shader_;

      inline static std::size_t last_element_id_ = 0;
      inline static std::size_t last_tab_id_ = 0;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_GUI_MANAGER_H
