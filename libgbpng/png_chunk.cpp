#include"libgbpng/png.hpp"
#include<cstdlib>
#include<cstring>




namespace gbpng{




chunk::
chunk(const binary&  bin, chunk_name  name) noexcept:
binary(bin),
m_name(name)
{
  update_crc();
}


chunk::
chunk(binary&&  bin, chunk_name  name) noexcept:
binary(std::move(bin)),
m_name(name)
{
  update_crc();
}




chunk&
chunk::
operator=(const chunk&  rhs) noexcept
{
    if(this != &rhs)
    {
      binary::assign(rhs);

      m_name = rhs.m_name;

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
      binary::assign(std::move(rhs));

      m_name = rhs.m_name;

      m_crc = rhs.m_crc;
    }


  return *this;
}




void
chunk::
save(uint8_t*  dst) const noexcept
{
  put_be32(get_data_size(),dst);


  char  buf[5];

  m_name.print(buf);

  *dst++ = buf[0];
  *dst++ = buf[1];
  *dst++ = buf[2];
  *dst++ = buf[3];

    for(int  i = 0;  i < get_data_size();  ++i)
    {
      *dst++ = get_data()[i];
    }


  put_be32(m_crc,dst);
}


void
chunk::
load(const uint8_t*  src) noexcept
{
  auto  size = get_be32(src);


  char  buf[5];

  buf[0] = *src++;
  buf[1] = *src++;
  buf[2] = *src++;
  buf[3] = *src++;
  buf[4] = 0;

  m_name = chunk_name(buf[0],buf[1],buf[2],buf[3]);

  binary::assign(src,size);

  src += size;

  m_crc = get_be32(src);

    if(!test_crc())
    {
      printf("png chunk load error: crc not matched\n");
    }
}




void
chunk::
print() const noexcept
{
  char  buf[5];

  m_name.print(buf);

  printf("data_size: %d\n",get_data_size());
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




