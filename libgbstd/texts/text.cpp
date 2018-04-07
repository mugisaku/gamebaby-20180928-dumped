#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{




void
text::
resize(int  w, int  h) noexcept
{
  m_width  = w;
  m_height = h;

  m_chars.resize(w*h);
}



}}




