#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




indexed_color_image&
indexed_color_image::
assign(const direct_color_image&  src_img)
{
  int  w = src_img.get_width() ;
  int  h = src_img.get_height();

  allocate(w,h);

  int  num_pixels = w*h;

  m_palette.set_number_of_colors(0);

  auto  dst =         get_row_pointer(0);
  auto  src = src_img.get_row_pointer(0);

    for(int  i = 0;  i < num_pixels;  ++i)
    {
      color  c;

      c.r = *src++;
      c.g = *src++;
      c.b = *src++;
           ++src;

      auto  res = m_palette.append_color(c);

        if(res >= 0)
        {
          *dst++ = res;
        }

      else
        {
          printf("indexed_color_image assign error: number of colors is over\n");

          break;
        }
    }


  return *this;
}


indexed_color_image&
indexed_color_image::
assign(image_source&  isrc)
{
  m_palette = isrc.plte;

  store(std::move(isrc.data),isrc.ihdr.get_width(),isrc.ihdr.get_height());

  return *this;
}


indexed_color_image&
indexed_color_image::
assign(const chunk_list&  ls)
{
  direct_color_image  img(ls);

  return assign(ls);
}




chunk_list
indexed_color_image::
make_chunk_list(int  bit_depth) const
{
  chunk_list  ls;

  image_header  ihdr(get_width(),get_height(),pixel_format::indexed);

  ihdr.set_bit_depth(bit_depth);

  ihdr.check_error();


  ls.push_back(ihdr.make_chunk());

  ls.push_back(m_palette.make_chunk());


  auto  idat = get_image_data(bit_depth);

  ls.push_back(idat.make_chunk());


  return std::move(ls);
}




void
indexed_color_image::
write_png_to_memory(uint8_t*  ptr, int  bit_depth) const
{
  auto  ls = make_chunk_list(bit_depth);

  ls.write_png_to_memory(ptr);
}


void
indexed_color_image::
write_png_to_file(FILE*  f, int  bit_depth) const
{
  auto  ls = make_chunk_list(bit_depth);

  ls.write_png_to_file(f);
}


void
indexed_color_image::
write_png_to_file(const char*  path, int  bit_depth) const
{
  auto  ls = make_chunk_list(bit_depth);

  ls.write_png_to_file(path);
}




image_data
indexed_color_image::
get_image_data(int  bit_depth) const
{
  int  w = get_width() ;
  int  h = get_height();

  image_header  ihdr(w,h,pixel_format::indexed);

  ihdr.set_bit_depth(bit_depth);

  ihdr.check_error();

    if(bit_depth < 8)
    {
      auto  bitpacked = get_bitpacked(get_row_pointer(0),ihdr);

      return image_data(bitpacked.begin(),ihdr);
    }


  return image_data(get_row_pointer(0),ihdr);
}



}




