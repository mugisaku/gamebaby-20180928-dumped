#include"libgbstd/font.hpp"




namespace gbstd{
namespace fonts{


void
font::
clear() noexcept
{
    for(auto&  ln: m_link_table)
    {
      delete[] ln          ;
               ln = nullptr;
    }


  m_width  = 0;
  m_height = 0;
}




const uint32_t*
font::
get_glyph_data(char16_t  c) const noexcept
{
  return m_link_table[(c < 0xFF00)? c:(c-0xFF00+' ')];
}


namespace{
void
load(uint32_t*  ln, int  h, int  line_size, binary_stream_reader&  r)
{
    for(int  i = 0;  i < h;  ++i)
    {
      uint32_t   v;

        if(line_size <= 8)
        {
          v = r.get()<<24;
        }

      else
        if(line_size <= 16)
        {
          v = r.get_be16()<<16;
        }

      else
        {
          v = r.get_be32();
        }


        if(ln)
        {
          ln[i] = v;
        }
    }
}


}


void
font::
load_from_file(gbstd::string_view  path) noexcept
{
  binary_stream  s;

    if(s.set_content_from_file(path))
    {
      auto  r = s.make_reader();

      load_from_stream(r);
    }
}


void
font::
load_from_stream(binary_stream_reader&  r) noexcept
{
  clear();

  m_width          = r.get();
  m_height         = r.get();
  m_bits_per_pixel = r.get();

  int  const line_size = m_bits_per_pixel*m_width;

  auto  n = r.get_be16();

    while(n--)
    {
      auto  unicode = r.get_be16();

        if(unicode >= 0xFF00)
        {
          unicode -= 0xFF00-' ';
        }


      auto&  ln = m_link_table[unicode];

        if(!ln)
        {
          ln = new uint32_t[m_height];
        }


      load(ln,m_height,line_size,r);
    }
}




void
font::
save_to_file(gbstd::string_view  path) const noexcept
{
  binary_stream  s;

  auto  w = s.make_writer();

  save_to_stream(w);

  s.output_content_to_file(path);
}


void
font::
save_to_stream(binary_stream_writer&  w) const noexcept
{
  auto&  bpp = m_bits_per_pixel;

  int  const line_size = 2*m_width;

  w.put(m_width);
  w.put(m_height);
  w.put(2);

  auto  off = w.get_offset();

  w.put_be16(0);


  int  n = 0;

  int  unicode = 0;

    for(auto  ln: m_link_table)
    {
        if(ln)
        {
          ++n;

          w.put_be16(unicode);

            if(bpp == 1)
            {
              int  table[16][16] = {0};

                for(int  i = 0;  i < m_height;  ++i)
                {
                  auto  v = ln[i];

                    for(int  x = 0;  x < m_width;  ++x)
                    {
                      table[i][x] = (v>>31);

                      v <<= 1;
                    }
                }


                for(int  y = 0;  y < 16;  ++y){
                for(int  x = 0;  x < 16;  ++x){
                  auto  v = table[y][x];

                    if(v == 1)
                    {
                        if((x     ) && !table[y  ][x-1]){table[y  ][x-1] = 2;}
                        if((y     ) && !table[y-1][x  ]){table[y-1][x  ] = 2;}
                        if((x < 15) && !table[y  ][x+1]){table[y  ][x+1] = 2;}
                        if((y < 15) && !table[y+1][x  ]){table[y+1][x  ] = 2;}
                    }
                }}


                for(int  y = 0;  y < m_height;  ++y)
                {
                  uint32_t  v = 0;

                    for(int  x = 0;  x < m_width;  ++x)
                    {
                      v <<= 2;
                      v  |= table[y][x];
                    }


                       if(line_size <=  8){w.put(     v);}
                  else if(line_size <= 16){w.put_be16(v);}
                  else                    {w.put_be32(v);}
                }
            }

          else
            {
                for(int  y = 0;  y < m_height;  ++y)
                {
                  w.put_be32(ln[y]);
                }
            }
        }


      ++unicode;
    }


  w.set_offset(off);

  w.put_be16(n);
}


}}




