#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{


chunk::
chunk(uint32_t  data_size, chunk_name  name, const void*  data) noexcept:
m_data_size(data_size),
m_name(name)
{
  m_data = new uint8_t[data_size];

  std::memcpy(m_data,data,data_size);

  update_crc();
}




chunk&
chunk::
operator=(const chunk&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear_data();

      m_data_size = rhs.m_data_size;

      m_name = rhs.m_name;

      m_data = new uint8_t[rhs.m_data_size];

      std::memcpy(m_data,rhs.m_data,rhs.m_data_size);

      m_crc = rhs.m_crc;
    }


  return *this;
}


chunk&
chunk::
operator=(chunk&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear_data();

      std::swap(m_data_size,rhs.m_data_size);

      m_name = rhs.m_name;

      std::swap(m_data,rhs.m_data);

      m_crc = rhs.m_crc;
    }


  return *this;
}




void
chunk::
save(uint8_t*  dst) const noexcept
{
  put_be32(m_data_size,dst);


  char  buf[5];

  m_name.print(buf);

  *dst++ = buf[0];
  *dst++ = buf[1];
  *dst++ = buf[2];
  *dst++ = buf[3];

    for(int  i = 0;  i < m_data_size;  ++i)
    {
      *dst++ = m_data[i];
    }


  put_be32(m_crc,dst);
}


void
chunk::
load(const uint8_t*  src) noexcept
{
  auto  size = get_be32(src);


  char  buf[4];

  buf[0] = *src++;
  buf[1] = *src++;
  buf[2] = *src++;
  buf[3] = *src++;

  m_name = chunk_name(buf[0],buf[1],buf[2],buf[3]);

  copy_data(src,size);

  src += size;

  m_crc = get_be32(src);

    if(!test_crc())
    {
      printf("png chunk load error: crc not matched\n");
    }
}




void
chunk::
copy_data(const uint8_t*  src, uint32_t  size) noexcept
{
  clear_data();

  m_data = new uint8_t[size];

  std::memcpy(m_data,src,size);

  m_data_size = size;
}


void
chunk::
clear_data() noexcept
{
  delete[] m_data          ;
           m_data = nullptr;

  m_data_size = 0;
}


void
chunk::
print() const noexcept
{
  char  buf[5];

  m_name.print(buf);

  printf("data_size: %d\n",m_data_size);
  printf("name: %s\n",buf);
  printf("crc: 0x%08X\n",m_crc);

    switch(m_name)
    {
  case(chunk_name("IHDR")):
      {
        image_header  ihdr(*this);

        ihdr.print();
      }
      break;
  case(chunk_name("IDAT")):
      break;
  case(chunk_name("PLTE")):
      break;
    }
}




}




