#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>
#include<png.h>
#include<zlib.h>




namespace gbpng{




const chunk
g_end_chunk(binary(),chunk_name("IEND"));


const chunk&
chunk_list::
get_end_chunk() noexcept
{
  return g_end_chunk;
}




void
chunk_list::
clear() noexcept
{
  auto  ptr = m_first;

    while(ptr)
    {
      auto  next = ptr->next;

      delete ptr       ;
             ptr = next;
    }


  m_first = nullptr;
  m_last  = nullptr;
}




chunk_list&
chunk_list::
assign(const chunk_list&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

        for(auto&  chk: rhs)
        {
          put_chunk(chunk(chk));
        }
    }


  return *this;
}


chunk_list&
chunk_list::
assign(chunk_list&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_first,rhs.m_first);
      std::swap(m_last ,rhs.m_last );
    }


  return *this;
}


chunk_list&
chunk_list::
assign(const image&  img) noexcept
{
  clear();

  auto  ihdr = img.make_image_header();
  auto  idat = img.make_image_data();

  put_chunk(ihdr.make_chunk());
  put_chunk(idat.make_chunk());

  return *this;
}


chunk_list&
chunk_list::
assign(const uint8_t*  src) noexcept
{
  clear();

  return append(src);
}


chunk_list&
chunk_list::
assign(const picture&  pic) noexcept
{
  clear();

  auto  ihdr = pic.make_image_header();
  auto  idat = pic.make_image_data();

  put_chunk(ihdr.make_chunk());
  put_chunk(idat.make_chunk());

  return *this;
}




chunk_list&
chunk_list::
append(const uint8_t*  src) noexcept
{
    for(;;)
    {
      chunk  chk;

      chk.load(src);

      src += chk.get_file_size();

        if(chk.get_name() == g_end_chunk.get_name())
        {
          break;
        }


        if(!chk.test_crc())
        {
          printf("png file load error: wrong crc\n");

          break;
        }


      put_chunk(std::move(chk));
    }


  return *this;
}




const chunk*
chunk_list::
get_chunk(chunk_name  name) const noexcept
{
    for(auto&  chk: *this)
    {
        if(chk.get_name() == name)
        {
          return &chk;
        }
    }


  return nullptr;
}


void
chunk_list::
put_chunk(chunk&&  chk) noexcept
{
  auto  nd = new node;

  nd->chunk = std::move(chk);

    if(m_last)
    {
      m_last->next = nd;
    }

  else
    {
      m_first = nd;
    }


  nd->previous = m_last     ;
                 m_last = nd;

  nd->next = nullptr;
}




void
chunk_list::
write(uint8_t*  dst) const noexcept
{
    for(auto&  chk: *this)
    {
      auto  size = chk.get_file_size();

      chk.save(dst);

      dst += size;
    }


  g_end_chunk.save(dst);
}




namespace{


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


}




void
chunk_list::
concatenate(uint8_t*&  ptr, size_t&  size) const noexcept
{
  size = 0;

    for(auto&  chk: *this)
    {
        if(chk.get_name() == chunk_name("IDAT"))
        {
          size += chk.get_data_size();
        }
    }


  ptr = new uint8_t[size];

  auto  p = ptr;

    for(auto&  chk: *this)
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
chunk_list::
extract(const image_header&  ihdr, uint8_t*&  dst_ptr) const noexcept
{
  uint8_t*  concatenated_data_ptr;
  size_t    concatenated_data_size;

  concatenate(concatenated_data_ptr,concatenated_data_size);


  uint8_t*  uncompressed_data_ptr;
  size_t    uncompressed_data_size;

  uncompress_(concatenated_data_ptr,concatenated_data_size,
              uncompressed_data_ptr,uncompressed_data_size);

  image_data::unfilter(uncompressed_data_ptr,ihdr,dst_ptr);

  delete[] concatenated_data_ptr;
  delete[] uncompressed_data_ptr;
}




void
chunk_list::
print() const noexcept
{
    for(auto&  chk: *this)
    {
      printf("{\n");

      chk.print();

      printf("\n}\n");
    }
}




}




