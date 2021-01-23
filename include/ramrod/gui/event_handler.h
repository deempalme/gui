#ifndef RAMROD_GUI_EVENT_HANDLER_H
#define RAMROD_GUI_EVENT_HANDLER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_events.h>  // for SDL_Event, SDL_KeyboardEvent, SDL_Mouse...
#include <SDL2/SDL_rect.h>    // for SDL_Rect

#include "ramrod/gui/types.h"

namespace ramrod {
  namespace gui {
    class input;
    class window;

    class event_handler
    {
    public:
      event_handler(gui::window *parent);
      /**
       * @brief Forcing this window to redraw the screen
       */
      void force_change();
      /**
       * @brief Use this function to poll for currently pending events.
       */
      void poll_events();
      /**
       * @brief Waiting for triggering events
       *
       * Waits until an event is triggered (mouse, keyboard, etc). This will pause the
       * program's execution until an event is triggered.
       */
      void wait_for_events();
      /**
       * @brief Waiting for triggering events or until time runs out
       *
       * Waits until an event is triggered (mouse, keyboard, etc) or until the timeout
       * (in seconds) has passed. This will pause the program's execution until an event
       * is triggered or the time is over.
       *
       * @param timeout Waiting time in milliseconds.
       */
      void wait_for_events(const int milliseconds);

    protected:
      virtual void drop_text_event(const SDL_DropEvent &event);
      virtual void finger_event(const SDL_TouchFingerEvent &event);
      virtual void key_event(const SDL_KeyboardEvent &event);
      virtual void mouse_button_event(const SDL_MouseButtonEvent &event);
      virtual void mouse_motion_event(const SDL_MouseMotionEvent &event);
      virtual void mouse_wheel_event(const SDL_MouseWheelEvent &event);
      virtual void text_input_event(const SDL_TextInputEvent &event);
      virtual void text_edit_event(const SDL_TextEditingEvent &event);
      virtual void window_event(const SDL_WindowEvent &event);

      SDL_Event event_;
      SDL_Rect display_properties_, window_properties_, initial_window_properties_;
      bool closing_, hidden_;
      bool keyboard_active_;
      bool mouse_active_, mouse_blocked_;
      bool mouse_left_active_, mouse_middle_active_, mouse_right_active_;
      bool has_changed_;

    private:
      void analyze_event();

      gui::window *window_;

      int key_modifier_;
      int previous_x_, previous_y_;
      std::size_t current_element_;
      std::size_t current_children_;
      const gui::identifier *current_identifier_;
      gui::input *current_input_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_EVENT_HANDLER_H
