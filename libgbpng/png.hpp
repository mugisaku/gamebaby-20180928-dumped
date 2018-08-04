#ifndef gbpng_hpp_included
#define gbpng_hpp_included


#include<cstdint>
#include<cstdio>
#include<utility>
#include<list>
#include<vector>


#ifndef report
#define report  printf("%s %d: %s\n",__FILE__,__LINE__,__func__);
#endif


namespace gbpng{


inline
uint32_t
get_be32(const uint8_t*&  p) noexcept
{
  uint32_t  i;

  i  = (*p++<<24);
  i |= (*p++<<16);
  i |= (*p++<< 8);
  i |= (*p++    );

  return i;
}


inline
void
put_be32(uint32_t  i, uint8_t*&  p) noexcept
{
  *p++ = i>>24;
  *p++ = i>>16;
  *p++ = i>>8 ;
  *p++ = i    ;
}


class
chunk_name
{
  uint32_t  m_data;

public:
  constexpr chunk_name(char  c0=0, char  c1=0, char  c2=0, char  c3=0) noexcept:
  m_data((c0<<24)|(c1<<16)|(c2<<8)|c3){}

  constexpr chunk_name(const char*  s) noexcept:
  m_data((s[0]<<24)|(s[1]<<16)|(s[2]<<8)|s[3]){}

  constexpr bool  operator==(const chunk_name&  rhs) const noexcept{return m_data == rhs.m_data;}
  constexpr bool  operator!=(const chunk_name&  rhs) const noexcept{return m_data != rhs.m_data;}

  constexpr operator uint32_t() const noexcept{return m_data;}

  void  print(char*  buf) const noexcept
  {
    buf[0] = m_data>>24;
    buf[1] = m_data>>16;
    buf[2] = m_data>> 8;
    buf[3] = m_data    ;
    buf[4] =          0;
  }

};


class
chunk
{
  uint32_t  m_data_size=0;

  chunk_name  m_name;

  uint8_t*  m_data=nullptr;

  uint32_t  m_crc=0;

  uint32_t  calculate_crc() const noexcept;

public:
  constexpr chunk() noexcept: m_data_size(0), m_name(), m_data(nullptr), m_crc(0){}
  chunk(uint32_t  data_size, chunk_name  name, const void*  data) noexcept;
  chunk(const chunk&   rhs) noexcept{*this = rhs;}
  chunk(      chunk&&  rhs) noexcept{*this = std::move(rhs);}

  ~chunk(){clear_data();}

  chunk&  operator=(const chunk&   rhs) noexcept;
  chunk&  operator=(      chunk&&  rhs) noexcept;


  constexpr uint32_t   get_data_size() const noexcept{return m_data_size;}
  constexpr chunk_name      get_name() const noexcept{return m_name;}
  constexpr const uint8_t*  get_data() const noexcept{return m_data;}
  constexpr uint32_t         get_crc() const noexcept{return m_crc;}

  constexpr uint32_t  get_binary_size() const noexcept{return 12+m_data_size;}

  void  save(      uint8_t*  dst) const noexcept;
  void  load(const uint8_t*  src)       noexcept;

  void  copy_data(const uint8_t*  src, uint32_t  size) noexcept;
  void  clear_data() noexcept;

  void  update_crc() noexcept{m_crc = calculate_crc();}

  bool  test_crc() const noexcept{return m_crc == calculate_crc();}

  void  print() const noexcept;

};




struct
color
{
  uint8_t  r;
  uint8_t  g;
  uint8_t  b;

};


class
palette
{
  int  m_number_of_colors=0;

  color  m_colors[256];

public:
  palette() noexcept{}
  palette(const chunk&  chk) noexcept;

        color&  get_color(int  i)       noexcept{return m_colors[i];}
  const color&  get_color(int  i) const noexcept{return m_colors[i];}

  int   get_number_of_colors(      ) const noexcept{return m_number_of_colors         ;}
  void  set_number_of_colors(int  n)       noexcept{       m_number_of_colors = n&0xFF;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




class
image_header
{
  int  m_width =0;
  int  m_height=0;

  int  m_bit_depth=8;

  struct flags{
    static constexpr int  palette = 1;
    static constexpr int    color = 2;
    static constexpr int    alpha = 4;
   };


  int  m_flags=flags::color|flags::alpha;

  palette  m_palette;

  bool  m_interlaced=false;

public:
  image_header(int  w, int  h) noexcept: m_width(w), m_height(h){}
  image_header(const chunk&  chk) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int   get_bit_depth(      ) const noexcept{return m_bit_depth    ;}
  void  set_bit_depth(int  v)       noexcept{       m_bit_depth = v;}

  void  use_palette() noexcept{m_flags |= flags::palette;}
  void    use_color() noexcept{m_flags |= flags::color;}
  void    use_alpha() noexcept{m_flags |= flags::alpha;}

  void  unuse_palette() noexcept{m_flags &= ~flags::palette;}
  void    unuse_color() noexcept{m_flags &= ~flags::color;}
  void    unuse_alpha() noexcept{m_flags &= ~flags::alpha;}

  bool  does_use_palette() const noexcept{return m_flags&flags::palette;}
  bool    does_use_color() const noexcept{return m_flags&flags::color;}
  bool    does_use_alpha() const noexcept{return m_flags&flags::alpha;}

  bool  is_interlaced() const noexcept{return m_interlaced;}

  int  get_number_of_bytes_per_pixel() const noexcept;
  int  get_size_of_image() const noexcept{return get_number_of_bytes_per_pixel()*m_width*m_height;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};


class
image_data
{
  size_t  m_data_size=0;

  uint8_t*  m_data=nullptr;

public:
  image_data(const chunk&  chk) noexcept;
  ~image_data(){delete[] m_data;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};


class
image
{
  int  m_width =0;
  int  m_height=0;

  uint8_t*  m_rgba_buffer=nullptr;

public:
   image() noexcept{}
   image(const image&   rhs) noexcept{*this = rhs;}
   image(      image&&  rhs) noexcept{*this = std::move(rhs);}
   image(const image_header&  ihdr) noexcept{resize(ihdr.get_width(),ihdr.get_height());}
  ~image(){delete[] m_rgba_buffer;}

   image&  operator=(const image&   rhs) noexcept;
   image&  operator=(      image&&  rhs) noexcept;

  image&  assign(int  w, int  h,       uint8_t*      data) noexcept;
  image&  assign(int  w, int  h, const uint8_t*  src_data) noexcept;

  void  resize(int  w, int  h) noexcept;

  void  clear() noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        uint8_t*  get_pixel(int  x, int  y)       noexcept{return &m_rgba_buffer[(4*m_width*y)+(4*x)];}
  const uint8_t*  get_pixel(int  x, int  y) const noexcept{return &m_rgba_buffer[(4*m_width*y)+(4*x)];}

};


class
file
{
  std::list<chunk>  m_chunk_list;

public:
  const chunk*  get_chunk(chunk_name  name) const noexcept;
  void          put_chunk(chunk&&  chk) noexcept;

  uint32_t  calculate_binary_size() const noexcept;

  void  save(      uint8_t*  dst) const noexcept;
  void  load(const uint8_t*  src)       noexcept;

  image  make_image() const noexcept;

  void  print() const noexcept;

  static void  unfilter(const uint8_t*  src, const image_header&  ihdr, uint8_t*&  dst) noexcept;

};


}


#endif




