#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<png.h>
#include<zlib.h>




namespace gbpng{




constexpr const uint8_t
g_signature[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 


uint32_t
calculate_png_stream_size(const chunk_list&  ls) noexcept
{
  uint32_t  size = 8+12;

    for(auto&  chk: ls)
    {
      size += chk.get_file_size();
    }


  size += chunk_list::get_end_chunk().get_file_size();

  return size;
}




bool
read_png_from_stream(chunk_list&  ls, const uint8_t*  ptr) noexcept
{
    if(std::memcmp(ptr,g_signature,8) != 0)
    {
      printf("png file load error: signature is invalid\n");

      return false;
    }


  ptr += 8;

  ls.append(ptr);

  return true;
}


bool
write_png_to_stream(const chunk_list&  ls, uint8_t*  ptr) noexcept
{
    for(auto  c: g_signature)
    {
      *ptr++ = c;
    }


  ls.write(ptr);

  return true;
}


bool
read_png_from_file(chunk_list&  ls, const char*  path) noexcept
{
  auto  f = fopen(path,"rb");

    if(!f)
    {
      printf("read_png_from_file error: could not open file\n");

      return false;
    }


  std::vector<uint8_t>  buf;

    for(;;)
    {
      int  c = fgetc(f);

        if(feof(f))
        {
          break;
        }


        if(ferror(f))
        {
          printf("open_file error: file error\n");

          break;
        }


      buf.emplace_back(c);
    }


  fclose(f);

  return read_png_from_stream(ls,buf.data());
}


bool
write_png_to_file(const chunk_list&  ls, const char*  path) noexcept
{
  auto  f = fopen(path,"wb");

    if(!f)
    {
      printf("write_png_to_file error: could not open file\n");

      return false;
    }


  auto  size = calculate_png_stream_size(ls);

  auto  ptr = new uint8_t[size];

  write_png_to_stream(ls,ptr);

    if(fwrite(ptr,size,1,f) != 1)
    {
      printf("write_png_to_file error: file writing error\n");
    }


  fclose(f);

  delete[] ptr;

  return true;
}




}




