#include "ramrod/gui/input.h"

#include "SDL_keyboard.h"                          // for SDL_StartTextInput
#include "ramrod/gl/shader.h"
#include "ramrod/gui/font_family.h"
#include "ramrod/gui/gui_manager.h"

namespace ramrod {
  namespace gui {
    input::input(const std::string &initial_text, const float width, const float height,
                 const float position_x, const float position_y,
                 const std::size_t tab_index) :
      gui::element(width, height, position_x, position_y),
      characters_(initial_text),
      character_position_(initial_text.size()),
      character_count_(character_position_),
      max_characters_{50},
      active_{false},
      selection_(),
      tab_index_{gui::gui_manager::create_new_tab_input(this, tab_index)},
      text_buffer_(false),
      background_color_{{0, 0, 0, 1.0f}},
      border_color_{{109.0f/255.0f, 185.0f/255.0f, 221.0f/255.0f, 1.0f}},
      border_size_{3.0f},
      text_color_{{1.0f, 1.0f, 1.0f, 1.0f}},
      text_size_{1.0f},
      font_{gui::gui_manager::get_font("")}
    {
      sprite_shader_ = gui::gui_manager::sprite_shader();
      u_background_color_ = sprite_shader_->uniform_location("u_background_color");
      u_border_color_ = sprite_shader_->uniform_location("u_border_color");
      u_id_ = sprite_shader_->uniform_location("u_id");
    }

    input::~input(){}

    float *input::background_color(){
      return background_color_.data;
    }

    void input::background_color(const uint8_t red, const uint8_t green,
                                 const uint8_t blue, const uint8_t alpha){
      background_color_.color.r = red/255.0f;
      background_color_.color.g = green/255.0f;
      background_color_.color.b = blue/255.0f;
      background_color_.color.a = alpha/255.0f;
    }

    void input::background_color(const float red, const float green,
                                 const float blue, const float alpha){
      background_color_.color.r = red;
      background_color_.color.g = green;
      background_color_.color.b = blue;
      background_color_.color.a = alpha;
    }

    void input::backspace(){
      if(character_position_ == 0) return;
      characters_.erase(characters_.begin() + character_position_ - 1);
      --character_position_;
      --character_count_;
      update_ = true;
    }

    void input::backspace_word(){
      if(character_position_ == 0) return;
      std::string::iterator begin = characters_.begin() + character_position_ - 1;
      std::string::iterator end = begin;
      std::size_t total{1};
      for(; begin != characters_.begin(); --begin, ++total)
        if(is_space(*begin)) break;
      characters_.erase(begin, end);
      character_position_ -= total;
      character_count_ -= total;
      update_ = true;
    }

    void input::blur(){
      SDL_StopTextInput();
      active_ = false;
      selection_.start = 0;
      selection_.count = 0;
    }

    float *input::border_color(){
      return border_color_.data;
    }

    void input::border_color(const uint8_t red, const uint8_t green,
                             const uint8_t blue, const uint8_t alpha){
      border_color_.color.r = red/255.0f;
      border_color_.color.g = green/255.0f;
      border_color_.color.b = blue/255.0f;
      border_color_.color.a = alpha/255.0f;
    }

    void input::border_color(const float red, const float green,
                             const float blue, const float alpha){
      border_color_.color.r = red;
      border_color_.color.g = green;
      border_color_.color.b = blue;
      border_color_.color.a = alpha;
    }

    void input::border_width(const float width){
      if(width < 0) return;
      border_size_ = width;
    }

    bool input::change_font(const std::string &font_name){
      gui::font_family *new_font = gui::gui_manager::get_font(font_name);
      if(new_font == nullptr) return false;
      font_ = new_font;
      return update_ = true;
    }

    bool input::change_font(gui::font_family *font_family){
      if(font_family == nullptr) return false;
      font_ = font_family;
      return update_ = true;
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

    void input::delete_character(){
      if(character_position_ == character_count_) return;
      characters_.erase(characters_.begin() + character_position_);
      --character_count_;
      update_ = true;
    }

    void input::delete_line(){
      clear();
    }

    void input::delete_word(){
      if(character_position_ == character_count_) return;
      std::string::iterator begin = characters_.begin() + character_position_;
      std::string::iterator end = begin;
      std::size_t total{1};
      for(; end != characters_.end(); ++end, ++total)
        if(is_space(*end)) break;
      characters_.erase(begin, end);
      character_count_ -= total;
      update_ = true;
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

    bool input::goto_position(const int position){
      if(static_cast<long>(character_position_) == position) return true;

      if(position < 0){
        character_position_ -= position;
        if(character_position_ > character_count_){
          character_position_ = 0;
          return false;
        }
        return true;
      }else if(position > static_cast<long>(character_count_)){
        character_position_ = character_count_;
        return false;
      }

      character_position_ = position;
      return true;
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

    void input::next_character(){
      if(++character_position_ >= character_count_)
        character_position_ = character_count_ - 1;
    }

    void input::next_word(){
      if(character_position_ >= character_count_) return;
      for(; character_position_ < character_count_; ++character_position_)
        if(is_space(characters_[character_position_])) break;
    }

    void input::paint(){
      if(!update_) return;
      if(resize_) change_size();
    }

    void input::previous_character(){
      if(--character_position_ > character_count_)
        character_position_ = 0;
    }

    void input::previous_word(){
      if(character_position_ == 0) return;
      for(; character_position_ > 0; --character_position_)
        if(is_space(characters_[character_position_])) break;
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

    void input::text_color(const uint8_t red, const uint8_t green,
                           const uint8_t blue, const uint8_t alpha){
      text_color_.color.r = red/255.0f;
      text_color_.color.g = green/255.0f;
      text_color_.color.b = blue/255.0f;
      text_color_.color.a = alpha/255.0f;
    }

    void input::text_color(const float red, const float green,
                           const float blue, const float alpha){
      text_color_.color.r = red;
      text_color_.color.g = green;
      text_color_.color.b = blue;
      text_color_.color.a = alpha;
    }

    void input::text_size(const uint16_t pixels){
      if(pixels == 0) return;
      text_size_ = static_cast<float>(pixels);
    }

    void input::text_size(const float size){
      if(size <= 0) return;
      text_size_ = size;
    }

    std::size_t input::tab_index(){
      return tab_index_;
    }

    void input::tab_index(const std::size_t new_tab_index){
      if(new_tab_index != tab_index_)
        gui::gui_manager::move_tab_input(this, tab_index_ = new_tab_index);
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

    bool input::is_new_line(const char *character){
      const std::string text(character);
      unsigned int value{static_cast<unsigned int>(text[0])};
      if(value == 2 || value == 3 || value == 10 || value == 13 || value == 25
      || value == 28 || value == 30 || value == 133 || value == 152 || value == 156)
        return true;
      return false;
    }

    bool input::is_new_line(std::string::reference character){
      unsigned int value{static_cast<unsigned int>(character)};
      if(value == 2 || value == 3 || value == 10 || value == 13 || value == 25
      || value == 28 || value == 30 || value == 133 || value == 152 || value == 156)
        return true;
      return false;
    }

    bool input::is_space(const char *character){
      const std::string text(character);
      unsigned int value{static_cast<unsigned int>(text[0])};
      if((value >= 0 && value <= 47) || (value >= 58 && value <= 63)
         || (value >= 91 && value <= 96) || (value >= 123 && value <= 191)) return true;
      return false;
    }

    bool input::is_space(std::string::reference character){
      unsigned int value{static_cast<unsigned int>(character)};
      if((value >= 0 && value <= 47) || (value >= 58 && value <= 63)
         || (value >= 91 && value <= 96) || (value >= 123 && value <= 191)) return true;
      return false;
    }

    // :::::::::::::::::::::::::::::::::::: PROTECTED FUNCTIONS :::::::::::::::::::::::::::::::::::
  } // namespace: gui
} // namespace: ramrod
