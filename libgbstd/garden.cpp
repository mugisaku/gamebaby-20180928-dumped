#include"libgbstd/garden.hpp"




namespace gbstd{
namespace gardens{




void
garden::
append(const sprite&  spr) noexcept
{
    if(m_number_of_sprites < m_sprites.size())
    {
      m_sprites[m_number_of_sprites] = spr;
    }

  else
    {
      m_sprites.emplace_back(spr);
    }


  ++m_number_of_sprites;
}


void
garden::
composite(image&  dst) const noexcept
{
  auto   it = &m_sprites[                  0];
  auto  end = &m_sprites[m_number_of_sprites];

    while(it != end)
    {
      auto&  spr = *it++;

      auto  src_rect = rectangle(spr.src_point,spr.width,spr.height);

      auto  cur = image_cursor(dst,spr.dst_point);

      images::transfer(*spr.image,src_rect,cur,true);
    }
}




}}




