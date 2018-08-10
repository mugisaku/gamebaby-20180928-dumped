#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




namespace{
constexpr const uint8_t
g_signature[] = {0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A}; 


const chunk
g_end_chunk(binary(),chunk_name("IEND"));
}


uint32_t
chunk_list::
calculate_stream_size() const noexcept
{
  uint32_t  size = 8;

    for(auto&  chk: m_container)
    {
      size += chk.get_stream_size();
    }


  size += g_end_chunk.get_stream_size();

  return size;
}




bool
chunk_list::
read_png_from_stream(const uint8_t*  p) noexcept
{
    if(std::memcmp(p,g_signature,8) != 0)
    {
      printf("chunk_set read error: signature is invalid\n");

      return false;
    }


  p += 8;

  m_container.clear();

  chunk  tmp;

    for(;;)
    {
      p = tmp.read(p);

        if(p)
        {
            if(tmp == "IEND")
            {
              break;
            }

          else
            {
              m_container.emplace_back(std::move(tmp));
            }
        }

      else
        {
          return false;
        }
    }


  return true;
}


bool
chunk_list::
read_png_from_file(const char*  path) noexcept
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

  return read_png_from_stream(buf.data());
}




bool
chunk_list::
write_png_to_stream(uint8_t*  p) const noexcept
{
    for(auto  c: g_signature)
    {
      *p++ = c;
    }


    for(auto&  chk: m_container)
    {
      p = chk.write(p);

        if(!p)
        {
          return false;
        }
    }


  g_end_chunk.write(p);

  return true;
}


bool
chunk_list::
write_png_to_file(const char*  path) const noexcept
{
  auto  f = fopen(path,"wb");

    if(!f)
    {
      printf("write_png_to_file error: could not open file\n");

      return false;
    }


  auto  size = calculate_stream_size();

  auto  ptr = new uint8_t[size];

  write_png_to_stream(ptr);

    if(fwrite(ptr,size,1,f) != 1)
    {
      printf("write_png_to_file error: file writing error\n");
    }


  fclose(f);

  delete[] ptr;

  return true;
}




void
chunk_list::
print() const noexcept
{
    for(auto&  chk: m_container)
    {
      chk.print();
    }
}




}




