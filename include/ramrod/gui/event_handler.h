#ifndef RAMROD_GUI_EVENT_HANDLER_H
#define RAMROD_GUI_EVENT_HANDLER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_events.h>  // for SDL_Event, SDL_KeyboardEvent, SDL_Mouse...
#include <SDL2/SDL_rect.h>    // for SDL_Rect

#include "ramrod/gui/type_events.h"

namespace ramrod {
  namespace gui {
    class window;

    class event_handler
    {
    public:
      event_handler();
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
      void wait_for_events(const std::uint32_t milliseconds);

    protected:
      // Finger's events
      virtual void finger_down_event(const gui::finger_event::finger &event) = 0;
      virtual void finger_move_event(const gui::finger_event::finger &event) = 0;
      virtual void finger_up_event(const gui::finger_event::finger &event) = 0;

      virtual void multigesture_event(const gui::multigesture::event &event) = 0;

      // Keyboard's events
      virtual void key_down_event(const gui::keyboard_event::key &event) = 0;
      virtual void key_up_event(const gui::keyboard_event::key &event) = 0;

      // Mouse's events
      virtual void click_event(const gui::mouse_event::button &event) = 0;
      virtual void mouse_down_event(const gui::mouse_event::button &event) = 0;
      virtual void mouse_move_event(const gui::mouse_event::move &event) = 0;
      virtual void mouse_up_event(const gui::mouse_event::button &event) = 0;
      virtual void mouse_wheel_event(const gui::mouse_event::wheel &event) = 0;

      // Window's events
      virtual void close_event(const gui::window_event::window &event) = 0;
      virtual void hide_event(const gui::window_event::window &event) = 0;
      virtual void maximize_event(const gui::window_event::window &event) = 0;
      virtual void minimize_event(const gui::window_event::window &event) = 0;
      virtual void move_event(const gui::window_event::move &event) = 0;
      virtual void resize_event(const gui::window_event::resize &event) = 0;
      virtual void restore_event(const gui::window_event::resize &event) = 0;
      virtual void show_event(const gui::window_event::window &event) = 0;
      virtual void quit_event(const gui::window_event::window &event) = 0;

      // Text's events
      virtual void drop_text_event(const SDL_DropEvent &event);
      virtual void text_input_event(const SDL_TextInputEvent &event) = 0;
      virtual void text_edit_event(const SDL_TextEditingEvent &event) = 0;

      virtual void close() = 0;
      virtual void hide() = 0;
      virtual void show() = 0;
      virtual void update_position() = 0;
      virtual void update_size() = 0;

      SDL_Event event_;

    private:
      void analyze_event();

      void window_event(const SDL_WindowEvent &event);
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_EVENT_HANDLER_H
