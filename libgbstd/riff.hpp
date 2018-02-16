#ifndef riff_hpp_included
#define riff_hpp_included


#include<cstdint>
#include<cstdio>
#include<utility>


namespace gbstd{
namespace riffs{


inline
uint32_t
bget_le32(const uint8_t*  p) noexcept
{
  uint32_t  i;

  i  = (p[0]    );
  i |= (p[1]<< 8);
  i |= (p[2]<<16);
  i |= (p[3]<<24);

  return i;
}


inline
uint16_t
bget_le16(const uint8_t*  p) noexcept
{
  uint16_t  i;

  i  = (p[0]   );
  i |= (p[1]<<8);

  return i;
}


class riff_chunk;


class
riff_id
{
  char  m_data[4];

public:
  constexpr riff_id() noexcept: m_data{0}{}
  constexpr riff_id(char  c0, char  c1, char  c2, char  c3) noexcept: m_data{c0,c1,c2,c3}{}

  constexpr uint32_t  encode() const noexcept
  {
    return((m_data[0]<<24)|
           (m_data[1]<<16)|
           (m_data[2]<< 8)|
           (m_data[3]    ));
  }

  constexpr bool  operator==(const riff_id&  rhs) const noexcept
  {
    return(encode() == rhs.encode());
  }

  constexpr bool  operator!=(const riff_id&  rhs) const noexcept
  {
    return(encode() != rhs.encode());
  }

                  char&  operator[](int  i)       noexcept{return m_data[i];}
  constexpr const char&  operator[](int  i) const noexcept{return m_data[i];}

  void  print() const
  {
    printf("%c%c%c%c",m_data[0],m_data[1],m_data[2],m_data[3]);
  }

};


class
riff_subchunk_view
{
  riff_id  m_id;

  uint32_t  m_size=0;

  const uint8_t*  m_data=nullptr;

public:
  riff_subchunk_view() noexcept{}
  riff_subchunk_view(const riff_chunk&  src) noexcept{assign(src);}
  riff_subchunk_view(const uint8_t*  p) noexcept{assign(p);}

  riff_subchunk_view&  operator=(const riff_chunk&  src) noexcept;

  bool  operator==(const riff_id&  id) const noexcept{return m_id.encode() == id.encode();}

  void  assign(const riff_chunk&  src) noexcept;
  void  assign(const uint8_t*  p) noexcept;

  const riff_id&  id() const noexcept{return m_id;}

  uint32_t  size() const noexcept{return m_size;}

  const uint8_t*  data() const noexcept{return m_data;}

  void  print() const noexcept;

};


class
riff_chunk
{
  struct private_data;

  private_data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  riff_chunk() noexcept{}
  riff_chunk(const riff_chunk&   rhs) noexcept{*this = rhs;}
  riff_chunk(      riff_chunk&&  rhs) noexcept{*this = std::move(rhs);}
 ~riff_chunk(){unrefer();}

  riff_chunk&  operator=(const riff_chunk&   rhs) noexcept;
  riff_chunk&  operator=(      riff_chunk&&  rhs) noexcept;

  void  load_file(const char*  filepath) noexcept;

  uint32_t  size() const noexcept;

  const uint8_t*  data() const noexcept;

  riff_subchunk_view  subchunk_view() const noexcept{return riff_subchunk_view(*this);}

};


}


using riffs::riff_id;
using riffs::riff_chunk;
using riffs::riff_subchunk_view;
using riffs::bget_le16;
using riffs::bget_le32;


}


#endif




