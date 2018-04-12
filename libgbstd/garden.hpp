#ifndef gbstd_garden_hpp_was_included
#define gbstd_garden_hpp_was_included


#include"libgbstd/image.hpp"


namespace gbstd{
namespace gardens{


struct
sprite
{
  const images::image*  image=nullptr;

  point  src_point;
  point  dst_point;

  int  width =0;
  int  height=0;

};


class
garden
{
  std::vector<sprite>  m_sprites;

  int  m_number_of_sprites=0;

public:
  garden() noexcept{}

  void  clear()  noexcept{m_number_of_sprites = 0;}

  void  append(const sprite&  spr) noexcept;

  void  composite(image&  dst) const noexcept;

};


}

using gardens::sprite;
using gardens::garden;


}


#endif




