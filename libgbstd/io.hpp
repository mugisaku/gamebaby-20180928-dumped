#ifndef GMBB_io_HPP
#define GMBB_io_HPP


#include"libgbstd/ios/riff.hpp"
#include"libgbstd/ios/wave.hpp"
#include"libgbstd/string_view.hpp"


namespace gbstd{


void  save_file(gbstd::string_view  filename, gbstd::string_view  content) noexcept;


}




#endif




