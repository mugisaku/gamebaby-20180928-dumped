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




}}




