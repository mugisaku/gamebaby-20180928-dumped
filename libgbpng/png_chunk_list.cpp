#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




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

  put_chunk(pic.make_control_chunk());

  auto  inc_flag = pic.does_use_main_image_as_first_frame();

    if(inc_flag)
    {
      put_chunk(pic.make_frame_control_chunk());
    }


  auto  ihdr = pic.make_image_header();
  auto  idat = pic.make_image_data();

  put_chunk(ihdr.make_chunk());
  put_chunk(idat.make_chunk());

  uint32_t  seq_num = inc_flag? 1:0;

    for(auto&  frm: pic.get_frame_list())
    {
      put_chunk(frm.make_control_chunk(seq_num  ));
      put_chunk(frm.make_chunk(        seq_num++));
    }


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




