#ifndef RAMROD_GUI_EVENT_HANDLER_H
#define RAMROD_GUI_EVENT_HANDLER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL_events.h>  // for SDL_Event, SDL_KeyboardEvent, SDL_Mouse...
#include <SDL2/SDL_rect.h>    // for SDL_Rect

#include "ramrod/gui/event.h"

namespace ramrod::gui {
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
    virtual void finger_down_event(const event::finger &event);
    virtual void finger_move_event(const event::finger &event);
    virtual void finger_up_event(const event::finger &event);

    virtual void multigesture_event(const event::multigesture &event);

    // Keyboard's events
    virtual void key_down_event(const event::key &event) = 0;
    virtual void key_up_event(const event::key &event) = 0;

    // Mouse's events
    virtual void click_event(const event::mouse_button &event);
    virtual void mouse_down_event(const event::mouse_button &event) = 0;
    virtual void mouse_move_event(const event::mouse_move &event) = 0;
    virtual void mouse_up_event(const event::mouse_button &event) = 0;
    virtual void mouse_wheel_event(const event::mouse_wheel &event) = 0;

    // Window's events
    virtual void close_event(const event::window &event);
    virtual void hide_event(const event::window &event);
    virtual void maximize_event(const event::window &event);
    virtual void minimize_event(const event::window &event);
    virtual void move_event(const event::window_move &event);
    virtual void resize_event(const event::window_resize &event);
    virtual void restore_event(const event::window_resize &event);
    virtual void show_event(const event::window &event);
    virtual void quit_event(const event::window &event);

    // Text's events
    virtual void drop_text_event(const SDL_DropEvent &event);
    virtual void text_input_event(const SDL_TextInputEvent &event);
    virtual void text_edit_event(const SDL_TextEditingEvent &event);

    virtual void close();
    virtual void hide();
    virtual void show();
    virtual void update_position();
    virtual void update_size();

    SDL_Event event_;

  private:
    void analyze_event();

    void window_event(const SDL_WindowEvent &event);
  };
} // namespace: ramrod::gui

#endif // RAMROD_GUI_EVENT_HANDLER_H
