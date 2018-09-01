#ifndef GMBB_io_HPP
#define GMBB_io_HPP


#include"libgbstd/ios/riff.hpp"
#include"libgbstd/ios/wave.hpp"
#include"libgbstd/string_view.hpp"


namespace gbstd{


void  write_to_file(const void*  ptr, size_t  size, const char*  filepath) noexcept;


}




#endif




