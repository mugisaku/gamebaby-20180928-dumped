#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<png.h>
#include<zlib.h>




namespace gbpng{




const chunk
g_end_chunk(0,chunk_name('I','E','N','D'),nullptr);




uint32_t
file::
calculate_binary_size() const noexcept
{
  uint32_t  size = 8+12;

    for(auto&  chk: m_chunk_list)
    {
      size += chk.get_binary_size();
    }


  size += g_end_chunk.get_binary_size();

  return size;
}


const chunk*
file::
get_chunk(chunk_name  name) const noexcept
{
    for(auto&  chk: m_chunk_list)
    {
        if(chk.get_name() == name)
        {
          return &chk;
        }
    }


  return nullptr;
}


void
file::
put_chunk(chunk&&  chk) noexcept
{
  m_chunk_list.emplace_back(std::move(chk));
}




constexpr const uint8_t
g_signature[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 


void
file::
save(uint8_t*  dst) const noexcept
{
    for(auto  c: g_signature)
    {
      *dst++ = c;
    }


    for(auto&  chk: m_chunk_list)
    {
      auto  size = chk.get_binary_size();

      chk.save(dst);

      dst += size;
    }


  g_end_chunk.save(dst);
}


void
file::
load(const uint8_t*  src) noexcept
{
    if(std::memcmp(src,g_signature,8) != 0)
    {
      printf("png file load error: signature is invalid\n");

      return;
    }


  src += 8;

  m_chunk_list.clear();

    while(1)
    {
      chunk  chk;

      chk.load(src);

      src += chk.get_binary_size();

        if(chk.get_name() == g_end_chunk.get_name())
        {
          break;
        }


        if(!chk.test_crc())
        {
          printf("png file load error: wrong crc\n");

          break;
        }


      m_chunk_list.emplace_back(std::move(chk));
    }
}




namespace{
void
concatenate(const std::list<chunk>&  ls, uint8_t*&  ptr, size_t&  size) noexcept
{
  size = 0;

    for(auto&  chk: ls)
    {
        if(chk.get_name() == chunk_name("IDAT"))
        {
          size += chk.get_data_size();
        }
    }


  ptr = new uint8_t[size];

  auto  p = ptr;

    for(auto&  chk: ls)
    {
        if(chk.get_name() == chunk_name("IDAT"))
        {
          auto  current_size = chk.get_data_size();

          std::memcpy(p,chk.get_data(),current_size);

          p += current_size;
        }
    }
}


void
uncompress_(const uint8_t*   src_ptr, size_t   src_size,
                 uint8_t*&  dst_ptr, size_t&  dst_size) noexcept
{
  dst_size = src_size*2;

  dst_ptr = nullptr;

    for(;;)
    {
      unsigned long int  size = dst_size;

      delete[] dst_ptr                    ;
               dst_ptr = new uint8_t[size];

        if(uncompress(dst_ptr,&size,src_ptr,src_size) == Z_OK)
        {
          dst_size = size;

          break;
        }


      dst_size *= 2;
    }
}


void
extract(const std::list<chunk>&  ls, const image_header&  ihdr, uint8_t*&  dst_ptr) noexcept
{
  uint8_t*  concatenated_data_ptr;
  size_t    concatenated_data_size;

  concatenate(ls,concatenated_data_ptr,concatenated_data_size);


  uint8_t*  uncompressed_data_ptr;
  size_t    uncompressed_data_size;

  uncompress_(concatenated_data_ptr,concatenated_data_size,
              uncompressed_data_ptr,uncompressed_data_size);

  file::unfilter(uncompressed_data_ptr,ihdr,dst_ptr);

  delete[] concatenated_data_ptr;
  delete[] uncompressed_data_ptr;

  
}
}


image
file::
make_image() const noexcept
{
  auto  ihdr_chunk = get_chunk("IHDR");

    if(!ihdr_chunk)
    {
    }


  auto  ihdr = image_header(*ihdr_chunk);

  uint8_t*  data_ptr;

  extract(m_chunk_list,ihdr,data_ptr);


  image  img;

  int  w = ihdr.get_width() ;
  int  h = ihdr.get_height();

    if(ihdr.does_use_palette())
    {
      auto  plte_chunk = get_chunk("PLTE");

        if(!plte_chunk)
        {
        }

      else
        {
          palette  plte(*plte_chunk);
        }


      delete[] data_ptr;
    }

  else
    {
        if(ihdr.does_use_color())
        {
            if(ihdr.does_use_alpha())
            {
              img.assign(w,h,data_ptr);
            }

          else
            {
              img.resize(w,h);

              auto  src = data_ptr;

              auto  dst = img.get_pixel(0,0);

                for(int  y = 0;  y < h;  ++y){
                for(int  x = 0;  x < w;  ++x){
                  *dst++ = *src++;
                  *dst++ = *src++;
                  *dst++ = *src++;
                  *dst++ =    255;
                }}


              delete[] data_ptr;
            }
        }

      else
        {
          delete[] data_ptr;
        }
    }


  return std::move(img);
}


void
file::
print() const noexcept
{
    for(auto&  chk: m_chunk_list)
    {
      printf("{\n");

      chk.print();

      printf("\n}\n");
    }
}


}




