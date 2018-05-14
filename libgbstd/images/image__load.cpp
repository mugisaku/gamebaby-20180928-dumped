#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"



namespace gbstd{
namespace images{




void
image::
load_from_image_data(const uint8_t*  data) noexcept
{
    if(is_image_data(data))
    {
      data += 3;

      int  w  = (*data++<<8);
           w |= (*data++   );
      int  h  = (*data++<<8);
           h |= (*data++   );

      resize(w,h);

        for(auto&  pix: m_pixels)
        {
          uint8_t  r = *data++;
          uint8_t  g = *data++;
          uint8_t  b = *data++;
          uint8_t  a = *data++;

            if(a)
            {
              pix.color = color(r>>5,g>>5,b>>5);
            }
        }
    }
}


std::vector<uint8_t>
image::
make_image_data() const noexcept
{
  std::vector<uint8_t>  buf;

  int  w = get_width() ;
  int  h = get_height();

  buf.emplace_back('i');
  buf.emplace_back('m');
  buf.emplace_back('g');
  buf.emplace_back(w>>8);
  buf.emplace_back(w&0xFF);
  buf.emplace_back(h>>8);
  buf.emplace_back(h&0xFF);

    for(auto&  pix: m_pixels)
    {
      auto  color = pix.color;

      buf.emplace_back(color.get_r255());
      buf.emplace_back(color.get_g255());
      buf.emplace_back(color.get_b255());
      buf.emplace_back(color? 255:0);
    }


  return std::move(buf);
}


}}




