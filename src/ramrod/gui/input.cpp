#include "ramrod/gui/input.h"

#include "SDL_keyboard.h"                          // for SDL_StartTextInput

#include "ramrod/gui/gui_manager.h"
#include "ramrod/gui/window.h"

namespace ramrod {
  namespace gui {
    input::input(gui::window *window, const std::string &initial_text,
                 const float width, const float height,
                 const float position_x, const float position_y,
                 const int z_index, const std::size_t tab_index) :
      gui::element(window, width, height, position_x, position_y, z_index, tab_index),
      characters_(initial_text),
      character_position_(initial_text.size()),
      character_count_(character_position_),
      max_characters_{50},
      active_{false},
      selection_(),
      text_buffer_(false),
      text_size_{1.0f}
    {
      tab_index_ = window_->add_tab_index(this, tab_index);
    }

    void input::blur(){
      SDL_StopTextInput();
      active_ = false;
      selection_.start = 0;
      selection_.count = 0;
    }

    bool input::clear(){
      if(character_count_ == 0) return false;
      character_count_ = 0;
      characters_.clear();
      character_position_ = 0;
      selection_.start = 0;
      selection_.count = 0;
      return update_ = true;
    }

    void input::focus(const bool select_all){
      SDL_StartTextInput();
      active_ = true;
      character_position_ = character_count_;
      if(select_all){
        select(0, character_count_);
        update_ = true;
      }
    }

    bool input::insert(const std::string &text){
      if(text.empty()) return false;

      if(character_position_ == max_characters_) return false;
      if((character_position_ + text.size()) > max_characters_){
        characters_.insert(text.begin(), text.begin(),
                           text.begin() + static_cast<long>(max_characters_ - character_position_));
        character_position_ = characters_.size();
        return !(update_ = true);
      }
      characters_.insert(character_position_, text);
      character_position_ = characters_.size();
      return update_ = true;
    }

    std::size_t input::maximum_characters(){
      return max_characters_;
    }

    void input::maximum_characters(const std::size_t new_maximum){
      max_characters_ = new_maximum;
    }

    std::size_t input::select(const std::size_t start, const std::size_t count){
      if(start > character_count_) return 0;
      selection_.start = start;
      selection_.count = count;
      if((start + count) > character_count_)
        selection_.count = character_count_ - selection_.start;
      update_ = true;
      return selection_.count;
    }

    std::size_t input::select_all(){
      if(character_count_ == 0) return 0;
      selection_.start = 0;
      update_ = true;
      return selection_.count = character_count_;
    }

    std::string input::selection(){
      if(selection_.count == 0) return std::string();
      return characters_.substr(selection_.start, selection_.count);
    }

    std::size_t input::tab_index(){
      return tab_index_;
    }

    void input::tab_index(const std::size_t new_tab_index){
      tab_index_ = window_->modify_tab_index(this, new_tab_index);
    }

    std::size_t input::total(){
      return character_count_;
    }

    const std::string &input::value(){
      return characters_;
    }

    bool input::value(const std::string &new_text){
      if(new_text.empty()) return false;

      if(new_text.size() > max_characters_){
        character_count_ = character_position_ = max_characters_;
        characters_.replace(0, character_count_ - 1, new_text);
        return !(update_ = true);
      }
      character_position_ = character_count_ = new_text.size();
      characters_ = new_text;
      return update_ = true;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::

    void input::background_color(const uint8_t /*red*/, const uint8_t /*green*/,
                                 const uint8_t /*blue*/, const uint8_t /*alpha*/){}

    void input::background_color(const float /*red*/, const float /*green*/,
                                 const float /*blue*/, const float /*alpha*/){}

    void input::border_color(const uint8_t /*red*/, const uint8_t /*green*/,
                             const uint8_t /*blue*/, const uint8_t /*alpha*/){}

    void input::border_color(const float /*red*/, const float /*green*/,
                             const float /*blue*/, const float /*alpha*/){}

    void input::border_width(const float /*width*/){}

    void input::text_color(const uint8_t /*red*/, const uint8_t /*green*/,
                           const uint8_t /*blue*/, const uint8_t /*alpha*/){}

    void input::text_color(const float /*red*/, const float /*green*/,
                           const float /*blue*/, const float /*alpha*/){}

    void input::text_size(const uint16_t /*pixels*/){}

    void input::text_size(const float /*size*/){}

    void input::paint(){
      if(!update_) return;

      update_ = true;
    }
  } // namespace: gui
} // namespace: ramrod
