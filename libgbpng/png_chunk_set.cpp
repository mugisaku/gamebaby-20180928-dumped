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


void
chunk_set::
clear() noexcept
{
  image_header  ihdr(0,0);

  m_ihdr = ihdr.make_chunk();

  m_between_ihdr_and_idat.clear();

  m_idat.clear();

  m_after_idat.clear();
}


uint32_t
chunk_set::
calculate_stream_size() const noexcept
{
  uint32_t  size = 8;

  size += m_ihdr.get_stream_size();

    for(auto&  chk: m_between_ihdr_and_idat)
    {
      size += chk.get_stream_size();
    }


    for(auto&  chk: m_idat)
    {
      size += chk.get_stream_size();
    }


    for(auto&  chk: m_after_idat)
    {
      size += chk.get_stream_size();
    }


  size += g_end_chunk.get_stream_size();

  return size;
}




bool
chunk_set::
read_between_ihdr_and_idat(const uint8_t*&  p) noexcept
{
  chunk  tmp;

    for(;;)
    {
      p = tmp.read(p);

        if(p)
        {
            if(tmp == "IDAT")
            {
              m_idat.emplace_back(std::move(tmp));

              break;
            }

          else
            if(tmp == "IEND")
            {
              printf("chunk_set read_png error: there is no IDAT\n");

              p = nullptr;

              return false;
            }

          else
            {
              m_between_ihdr_and_idat.emplace_back(std::move(tmp));
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
chunk_set::
read_idat(const uint8_t*&  p) noexcept
{
  chunk  tmp;

    for(;;)
    {
      p = tmp.read(p);

        if(p)
        {
            if(tmp == "IDAT")
            {
              m_idat.emplace_back(std::move(tmp));
            }

          else
            if(tmp == "IEND")
            {
              return false;
            }

          else
            {
              m_after_idat.emplace_back(std::move(tmp));

              break;
            }
        }

      else
        {
          return false;
        }
    }


  return true;
}


void
chunk_set::
read_after_idat(const uint8_t*&  p) noexcept
{
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
              m_after_idat.emplace_back(std::move(tmp));
            }
        }

      else
        {
          break;
        }
    }
}


bool
chunk_set::
read_png_from_stream(const uint8_t*  p) noexcept
{
    if(std::memcmp(p,g_signature,8) != 0)
    {
      printf("png file load error: signature is invalid\n");

      return false;
    }


  p += 8;

  clear();

  chunk  tmp;

  p = tmp.read(p);

    if(!p || (tmp != "IHDR"))
    {
      printf("chunk_set read_png error: first chunk is not IHDR\n");

      return false;
    }


  m_ihdr = std::move(tmp);

    if(read_between_ihdr_and_idat(p) && p)
    {
        if(read_idat(p) && p)
        {
          read_after_idat(p);
        }
    }


  return p;
}


bool
chunk_set::
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
chunk_set::
write_png_to_stream(uint8_t*  p) const noexcept
{
    for(auto  c: g_signature)
    {
      *p++ = c;
    }


  p = m_ihdr.write(p);

    for(auto&  chk: m_between_ihdr_and_idat)
    {
      p = chk.write(p);
    }


    for(auto&  chk: m_idat)
    {
      p = chk.write(p);
    }


    for(auto&  chk: m_after_idat)
    {
      p = chk.write(p);
    }


  g_end_chunk.write(p);

  return true;
}


bool
chunk_set::
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
chunk_set::
print() const noexcept
{
  m_ihdr.print();

    for(auto&  chk: m_between_ihdr_and_idat)
    {
      chk.print();
    }


    for(auto&  chk: m_idat)
    {
      chk.print();
    }


    for(auto&  chk: m_after_idat)
    {
      chk.print();
    }
}




}




