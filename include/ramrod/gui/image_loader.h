#ifndef RAMROD_GUI_IMAGE_LOADER_H
#define RAMROD_GUI_IMAGE_LOADER_H

#include <string>


namespace ramrod {
  namespace gui {
    class image_loader
    {
    public:
      /**
       * @brief image_loader
       *
       * The return value from an image loader is an `unsigned char *` data() which
       * points to the pixel data, or `nullptr` on an allocation failure or if the image
       * is corrupt or invalid. The pixel data consists of `height` scanlines of `width`
       * pixels, with each pixel consisting of N interleaved 8-bit components; the first
       * pixel pointed to is top-left-most in the image. There is no padding between
       * image scanlines or between pixels, regardless of format. The number of
       * components N is `desired_channels` if it is non-zero, or `channels_in_file`
       * otherwise. If `desired_channels` is non-zero, `channels_in_file` has the number
       * of components that _would_ have been output otherwise. E.g. if you set
       * `desired_channels` to 4, you will always get RGBA output, but you can check
       * `channels_in_file` to see if it's trivially opaque because e.g. there were
       * only 3 channels in the source image.
       *
       * @param path             String containing the path to the file image
       * @param invert_y_axis    true will invert the image vertically
       * @param desired_channels Number of channels that will be output, use 0 to use the
       *                         image's default number
       */
      image_loader(const std::string &path,
                   const bool invert_y_axis = true,
                   const int desired_channels = 0);
      ~image_loader();
      /**
       * @brief Getting the number of components from this image
       *
       * They could be:
       *        1           grey
       *        2           grey, alpha
       *        3           red, green, blue
       *        4           red, green, blue, alpha
       *
       * @return Number of image components
       */
      int channels_in_file();
      /**
       * @brief Getting the image data
       *
       * @return Pointer to the image data or `nullptr` if not found
       */
      unsigned char *data();
      /**
       * @brief Getting information about the failure in case there is one
       *
       * @return Failure description or empty if there is none
       */
      const char *failure_reason();
      /**
       * @brief Frees image's memory
       */
      void free();
      /**
       * @brief Getting the image's height
       *
       * @return Height in pixels
       */
      int height();
      /**
       * @brief Loading a new image
       *
       * The return value from an image loader is an `unsigned char *` which
       * points to the pixel data, or `nullptr` on an allocation failure or if the image
       * is corrupt or invalid. The pixel data consists of `height` scanlines of `width`
       * pixels, with each pixel consisting of N interleaved 8-bit components; the first
       * pixel pointed to is top-left-most in the image. There is no padding between
       * image scanlines or between pixels, regardless of format. The number of
       * components N is `desired_channels` if it is non-zero, or `channels_in_file`
       * otherwise. If `desired_channels` is non-zero, `channels_in_file` has the number
       * of components that _would_ have been output otherwise. E.g. if you set
       * `desired_channels` to 4, you will always get RGBA output, but you can check
       * `channels_in_file()` to see if it's trivially opaque because e.g. there were
       * only 3 channels in the source image.
       *
       * @param path             String containing the path to the file image
       * @param invert_y_axis    `true` will invert the image vertically
       * @param desired_channels Number of channels that will be output, use 0 to use the
       *                         image's default number
       *
       * @return A pointer to the image data, or nullptr if there was an error when loading
       *         the image, use failure_reason() to get detailed description of the latest error
       */
      unsigned char *load(const std::string &path,
                          const bool invert_y_axis = true,
                          const int desired_channels = 0);
      /**
       * @brief Indicates if the image was found and properly loaded
       *
       * @return `false` if the file path does not exist or there was an error loading the image
       *         use `failure_reason()` to see what was the problem
       */
      bool loaded();
      /**
       * @brief Getting the image's full path
       *
       * @return String containing the path to the image
       */
      const std::string &path();
      /**
       * @brief Getting the image's width
       *
       * @return Width in pixels
       */
      int width();

    private:
      std::string image_path_;
      unsigned char *image_data_;
      int width_, height_, channels_;
      bool loaded_;
    };
  } // namespace: gui
} // namespace: ramrod

#endif // RAMROD_GUI_IMAGE_LOADER_H
