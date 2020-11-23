#include "ramrod/gui/event_handler.h"

#include <SDL2/SDL_keyboard.h>                     // for SDL_Keysym
#include <SDL2/SDL_keycode.h>                      // for SDLK_DOWN, SDLK_LEFT
#include <SDL2/SDL_mouse.h>                        // for SDL_BUTTON_LEFT
#include <SDL2/SDL_video.h>                        // for SDL_WINDOWEVENT_CLOSE
#include "SDL_events.h"                            // for SDL_Event, SDL_Win...
#include "SDL_rect.h"                              // for SDL_Rect
#include "SDL_stdinc.h"                            // for SDL_free
#include "ramrod/console.h"                        // for formatted
#include "ramrod/console/attention.h"              // for attention, attenti...
#include "ramrod/console/types.h"                  // for attention
#include "ramrod/gui/enumerators.h"                // for ctrl, not_none
#include "ramrod/gui/window.h"                     // for window

namespace ramrod {
  namespace gui {
    event_handler::event_handler(gui::window *parent) :
      event_(),
      display_properties_{ 0, 0, 1024, 512 },
      window_properties_(display_properties_),
      initial_window_properties_(display_properties_),
      closing_{false},
      hidden_{true},
      keyboard_active_{false},
      mouse_active_{false},
      mouse_blocked_{false},
      mouse_left_active_{false},
      mouse_middle_active_{false},
      mouse_right_active_{false},
      has_changed_{true},
      window_{parent},
      key_modifier_{-1},
      previous_x_{-1},
      previous_y_{-1}
    {
    }

    void event_handler::force_change(){
      has_changed_ = true;
    }

    void event_handler::poll_events(){
      while(SDL_PollEvent(&event_) > 0) analyze_event();
    }

    void event_handler::wait_for_events(){
      if(SDL_WaitEvent(&event_) > 0) analyze_event();
      poll_events();
    }

    void event_handler::wait_for_events(const int milliseconds){
      if(SDL_WaitEventTimeout(&event_, milliseconds) > 0) analyze_event();
      poll_events();
    }

    // ::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void event_handler::drop_text_event(const SDL_DropEvent &event){
      if(event.type == SDL_DROPBEGIN){
      }else if(event.type == SDL_DROPCOMPLETE){
      }else if(event.type == SDL_DROPFILE){
        SDL_free(event.file);
      }else if(event.type == SDL_DROPTEXT){
        SDL_free(event.file);
      }
    }

    void event_handler::finger_event(const SDL_TouchFingerEvent &event){
    }

    void event_handler::key_event(const SDL_KeyboardEvent &event){
      const bool key_up{event.type == SDL_KEYUP};

      key_modifier_ = event.keysym.mod & ramrod::gui::keyboard::not_none;

      switch(event.keysym.sym){
        case SDLK_DOWN:
        break;
        case SDLK_UP:
        break;
        case SDLK_LEFT:
        break;
        case SDLK_RIGHT:
        break;
        case SDLK_c:
          if(key_modifier_ & ramrod::gui::keyboard::ctrl && key_up){
            rr::attention("coping");
          }
        break;
        case SDLK_v:
          if(key_modifier_ & ramrod::gui::keyboard::ctrl && key_up){
            rr::attention("pasting");
          }
        break;
        case SDLK_x:
          if(key_modifier_ & ramrod::gui::keyboard::ctrl && key_up){
            rr::attention("cuting");
          }
        break;
        default: break;
      }
    }

    void event_handler::mouse_button_event(const SDL_MouseButtonEvent &event){
      mouse_left_active_ = mouse_middle_active_ = mouse_right_active_ = false;

      if(!(mouse_active_ = (event.state == SDL_PRESSED)))
        //      core_->change_cursor(torero::gui::Cursor::Normal);

        switch(event.button){
          case SDL_BUTTON_LEFT:
            mouse_left_active_ = true;

            if(!mouse_blocked_ && mouse_active_){
              //          core_->change_cursor(torero::gui::Cursor::ArrowALL);
            }
          break;
          case SDL_BUTTON_MIDDLE:
            mouse_middle_active_ = true;
          break;
          case SDL_BUTTON_RIGHT:
            mouse_right_active_ = true;

            if(!mouse_blocked_ && mouse_active_){
              //          core_->change_cursor(torero::gui::Cursor::ArrowALL);
            }
          break;
          default: break;
        }
    }

    void event_handler::mouse_motion_event(const SDL_MouseMotionEvent &event){
      if(mouse_active_ && !mouse_blocked_){
        if(mouse_left_active_ || mouse_right_active_){
          //        core_->camera().mouse_to_camera(event.xrel, event.yrel, mouse_left_active_);
          has_changed_ = true;
        }
      }
    }

    void event_handler::mouse_wheel_event(const SDL_MouseWheelEvent &event){
      if(!mouse_blocked_){
        //      core_->camera().zoom(event.y > 0 ? torero::camera::Zoom::In : torero::camera::Zoom::Out);
        has_changed_ = true;
      }
    }

    void event_handler::text_input_event(const SDL_TextInputEvent &event){
      rr::formatted("pressed key: %s\n", rr::message::attention, event.text);
    }

    void event_handler::text_edit_event(const SDL_TextEditingEvent &event){
      rr::formatted("text edited: %s\n", rr::message::attention, event.text);
    }

    void event_handler::window_event(const SDL_WindowEvent &event){
      switch(event.event){
        case SDL_WINDOWEVENT_CLOSE:
          closing_ = true;
        break;
        case SDL_WINDOWEVENT_SHOWN:
          hidden_ = false;
        break;
        case SDL_WINDOWEVENT_HIDDEN:
          hidden_ = true;
        break;
        case SDL_WINDOWEVENT_MINIMIZED:
          hidden_ = true;
        break;
        case SDL_WINDOWEVENT_MAXIMIZED:
          hidden_ = false;
        break;
        case SDL_WINDOWEVENT_RESTORED:
          hidden_ = false;
        break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          if(event.data1 > 0 && event.data2 > 0) hidden_ = false;

          initial_window_properties_.w = window_properties_.w = event.data1;
          initial_window_properties_.h = window_properties_.h = event.data2;

          window_->restart_viewport();
          force_change();
        break;
        case SDL_WINDOWEVENT_MOVED:
          initial_window_properties_.x = window_properties_.x = event.data1;
          initial_window_properties_.y = window_properties_.y = event.data2;
        break;
        default: break;
      }
    }

    // :::::::::::::::::::::::::::::::::::: PRIVATE FUNCTIONS ::::::::::::::::::::::::::::::::::::

    void event_handler::analyze_event(){
      switch(event_.type){
        case SDL_QUIT:
          closing_ = true;
        break;
        case SDL_MOUSEBUTTONDOWN:
          mouse_button_event(event_.button);
        break;
        case SDL_MOUSEBUTTONUP:
          mouse_button_event(event_.button);
        break;
        case SDL_MOUSEMOTION:
          mouse_motion_event(event_.motion);
        break;
        case SDL_MOUSEWHEEL:
          mouse_wheel_event(event_.wheel);
        break;
        case SDL_KEYDOWN:
          key_event(event_.key);
        break;
        case SDL_KEYUP:
          key_event(event_.key);
        break;
        case SDL_FINGERDOWN:
          finger_event(event_.tfinger);
        break;
        case SDL_FINGERUP:
          finger_event(event_.tfinger);
        break;
        case SDL_FINGERMOTION:
          finger_event(event_.tfinger);
        break;
        case SDL_WINDOWEVENT:
          window_event(event_.window);
        break;
        case SDL_TEXTINPUT:
          text_input_event(event_.text);
        break;
        case SDL_TEXTEDITING:
          text_edit_event(event_.edit);
        break;
        case SDL_DROPBEGIN:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPCOMPLETE:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPFILE:
          drop_text_event(event_.drop);
        break;
        case SDL_DROPTEXT:
          drop_text_event(event_.drop);
        break;
        default: break;
      }
    }
  } // namespace: gui
} // namespace: ramrod
