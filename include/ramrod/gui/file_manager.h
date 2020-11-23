#ifndef RAMROD_GUI_FILE_MANAGER_H
#define RAMROD_GUI_FILE_MANAGER_H

#include <string>

namespace ramrod {
  namespace gui {
    class file_manager
    {
    public:
      file_manager();
      /**
       * @brief Adds the missing absolute path to a relative path
       *
       * This function is useful when using relative paths to find/call/etc a file. It will copy
       * the executable's absolute path and paste it at the beginning of the `relative_path`
       *
       * @param relative_path pointer address to a string containing the relative path
       *
       */
      static void absolute_path(std::string *relative_path);
      /**
       * @brief Checks if file/folder path exists
       *
       * This function checks if the file/folder in `path` exist, if not then, it will add at the
       * beginning of `path` the executable's absolute path, if still not found, then return
       * `false`. This function will modify `*path` adding the absolute path of the program if
       * the original was not found, if still not found after the addition then, it will be
       * returned to the initial state.
       *
       * @param path Pointer address to a string containing the file/folder path
       *
       * @return `true` if found
       *
       */
      static bool check_path(std::string *path);
      /**
       * @brief Checks if file/folder path exists
       *
       * This function checks if the file/folder in `path` exist, if not then, it will add
       * the executable's absolute path at the beginning of `path`, if still not found,
       * then returns `false`. This function does not modify `path`.
       *
       * @param path Constant string containing the file/folder path
       *
       * @return `true` if found
       *
       */
      static bool check_path(const std::string &path);

      static bool create_directory(const std::string &path);
      /**
       * @brief Checks if file/folder path exists
       *
       * Simple file/folder existencial checking, this does not modify the input `path`
       *
       * @param path Absolute path of file/folder to check
       *
       * @return `true` if found
       *
       */
      static bool exists(const std::string &path);

      static bool is_directory(const std::string &path);
      /**
       * @brief return the absolute executable's path
       *
       * This function return the absolute path of the executable
       *
       * @return pString containing the executable's absolute path
       *
       */
      static const std::string &program_path();
      /**
       * @brief Updating the program's path
       *
       * The program's absolute path does not updates automatically if you change its location.
       * Call this function every time you change the program's location folder to
       * update the executable's absolute path.
       *
       */
      static void reset();

    private:
      static bool initialized_;
      static std::string program_path_;
    };
  } // namepsace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_FILE_MANAGER_H
