#ifndef routine_hpp
#define routine_hpp


#include"libgbact/character.hpp"
#include"libgbact/stage.hpp"
#include"libgbstd/program.hpp"


namespace gbact{
namespace routines{


class
sleep_context: public programs::context
{
  uint32_t  m_time;

public:
  sleep_context(uint32_t  t=0) noexcept: m_time(g_time+t){}

  void  step() noexcept override;

};



class
chooser_context: public programs::context
{
  std::vector<gbstd::string>  m_text_list;

  spaces::image_object*  m_cursor;

  int  m_index;
  
public:
  chooser_context(std::initializer_list<gbstd::string_view>  ls={}) noexcept;

  void  step() noexcept override;

};



}
}


extern image  g_misc_image;




#endif




