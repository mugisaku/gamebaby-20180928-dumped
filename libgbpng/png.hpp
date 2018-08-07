#ifndef gbpng_hpp_included
#define gbpng_hpp_included


#include<cstdint>
#include<cstdio>
#include<utility>
#include<list>
#include<vector>
#include<memory>


#ifndef report
#define report  printf("%s %d: %s\n",__FILE__,__LINE__,__func__);
#endif


namespace gbpng{


inline
uint16_t
get_be16(const uint8_t*&  p) noexcept
{
  uint16_t  i;

  i  = (*p++<<8);
  i |= (*p++   );

  return i;
}


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
put_be16(uint16_t  i, uint8_t*&  p) noexcept
{
  *p++ = i>>8;
  *p++ = i   ;
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







class chunk_list;
class image;
class image_header;
class picture;
class palette;








class
binary
{
  uint32_t  m_data_size=0;

  uint8_t*  m_data=nullptr;

public:
 constexpr binary() noexcept{}
   binary(const void*  ptr, uint32_t  size) noexcept{assign(ptr,size);}
   binary(const binary&   rhs) noexcept{assign(rhs);}
   binary(      binary&&  rhs) noexcept{assign(std::move(rhs));}
  ~binary(){clear_data();}

  const uint8_t&  operator[](int  i) const noexcept{return m_data[i];}

  binary&  operator=(const binary&   rhs) noexcept{return assign(rhs);}
  binary&  operator=(      binary&&  rhs) noexcept{return assign(std::move(rhs));}

  binary&  assign(const binary&   rhs) noexcept;
  binary&  assign(      binary&&  rhs) noexcept;
  binary&  assign(const void*  ptr, uint32_t  size) noexcept;

  void  clear_data() noexcept;

  const uint8_t*  get_data() const noexcept{return m_data;}

  uint32_t  get_data_size() const noexcept{return m_data_size;}

};




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
chunk: public binary
{
  chunk_name  m_name;

  uint32_t  m_crc=0;

  uint32_t  calculate_crc() const noexcept;

public:
  chunk() noexcept{}
  chunk(const binary&   bin, chunk_name  name) noexcept;
  chunk(      binary&&  bin, chunk_name  name) noexcept;
  chunk(const chunk&   rhs) noexcept{*this = rhs;}
  chunk(      chunk&&  rhs) noexcept{*this = std::move(rhs);}

  chunk&  operator=(const chunk&   rhs) noexcept;
  chunk&  operator=(      chunk&&  rhs) noexcept;


  const chunk_name&  get_name() const noexcept{return m_name;}
  uint32_t            get_crc() const noexcept{return m_crc;}

  uint32_t  get_file_size() const noexcept{return 12+get_data_size();}

  void  save(      uint8_t*  dst) const noexcept;
  void  load(const uint8_t*  src)       noexcept;

  void  update_crc() noexcept{m_crc = calculate_crc();}

  bool  test_crc() const noexcept{return m_crc == calculate_crc();}

  void  print() const noexcept;

};




class
image_data: public binary
{

public:
  using binary::binary;

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

  static void  unfilter(const uint8_t*  src, const image_header&  ihdr, uint8_t*&  dst) noexcept;

};




class
image
{
  int  m_width =0;
  int  m_height=0;

  uint8_t*  m_data=nullptr;

  void  map_color(const uint8_t*  src, const palette&  plte) noexcept;
  void  copy_rgb(const uint8_t*  src) noexcept;
  void  copy_rgba(const uint8_t*  src) noexcept;
  void  copy_gray(const uint8_t*  src) noexcept;
  void  copy_gray_with_alpha(const uint8_t*  src) noexcept;

public:
   image() noexcept{}
   image(const image_header&  ihdr) noexcept;
   image(const chunk_list&  ls) noexcept{assign(ls);}
   image(const image&   rhs) noexcept{*this = rhs;}
   image(      image&&  rhs) noexcept{*this = std::move(rhs);}
  ~image(){delete[] m_data;}

   image&  operator=(const image&   rhs) noexcept;
   image&  operator=(      image&&  rhs) noexcept;
   image&  operator=(const chunk_list&  ls) noexcept{return assign(ls);}

  image&  assign(int  w, int  h,       uint8_t*      data) noexcept;
  image&  assign(int  w, int  h, const uint8_t*  src_data) noexcept;
  image&  assign(const chunk_list&  ls) noexcept;

  void  resize(int  w, int  h) noexcept;

  void  clear() noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        uint8_t*  get_rgba_pointer(int  x, int  y)       noexcept{return &m_data[(4*m_width*y)+(4*x)];}
  const uint8_t*  get_rgba_pointer(int  x, int  y) const noexcept{return &m_data[(4*m_width*y)+(4*x)];}

  void  get_filtered_data(uint8_t*&  ptr, size_t&  size) const noexcept;

  image_header  make_image_header() const noexcept;
  image_data    make_image_data() const noexcept;

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




enum class
dispose_type
{
  none,
  background,
  previous,

};


enum class
blend_type
{
  source,
  over,

};


class
animation_frame: public image
{
  uint32_t  m_x_offset;
  uint32_t  m_y_offset;

  uint16_t  m_delay_numerator;
  uint16_t  m_delay_denominator;

  dispose_type  m_dispose_type=dispose_type::none;
  blend_type    m_blend_type  =blend_type::source;

public:

};




class
picture: public image
{
  uint32_t  m_number_of_frames;
  uint32_t  m_loop_count;

  std::vector<animation_frame>  m_frame_list;

public:

};




class
chunk_list
{
  struct node{
    gbpng::chunk  chunk;

    node*  previous;
    node*  next;
  };

  node*  m_first=nullptr;
  node*  m_last =nullptr;

  void  concatenate(uint8_t*&  ptr, size_t&  size) const noexcept;

public:
  class iterator{
    node*  ptr;

  public:
    iterator(node*  nd=nullptr) noexcept: ptr(nd){}

    bool  operator!=(const iterator&  rhs) const noexcept{return ptr && (ptr != rhs.ptr);}

    const chunk&  operator*() noexcept{return ptr->chunk;}

    iterator&  operator++() noexcept
    {
      ptr = ptr->next;

      return *this;
    }
  };


  chunk_list() noexcept{}
  chunk_list(const image&  img) noexcept{assign(img);}
  chunk_list(const chunk_list&   rhs) noexcept{*this = rhs;}
  chunk_list(      chunk_list&&  rhs) noexcept{*this = std::move(rhs);}
  ~chunk_list(){clear();}

  chunk_list&  operator=(const chunk_list&   rhs) noexcept{return assign(rhs);}
  chunk_list&  operator=(      chunk_list&&  rhs) noexcept{return assign(std::move(rhs));}
  chunk_list&  operator=(const image&  img)       noexcept{return assign(img);}

  void  clear() noexcept;

  const chunk*  get_chunk(chunk_name  name) const noexcept;
  void          put_chunk(chunk&&  chk) noexcept;

  chunk_list&  assign(const chunk_list&   rhs) noexcept;
  chunk_list&  assign(      chunk_list&&  rhs) noexcept;
  chunk_list&  assign(const image&  img)       noexcept;
  chunk_list&  assign(const uint8_t*  src)     noexcept;
  chunk_list&  assign(const picture&  pic)     noexcept;

  chunk_list&  append(const uint8_t*  src) noexcept;

  iterator  begin() const noexcept{return iterator(m_first);}
  iterator    end() const noexcept{return iterator(       );}

  void  write(uint8_t*  ptr) const noexcept;

  static const chunk&  get_end_chunk() noexcept;

  void  extract(const image_header&  ihdr, uint8_t*&  dst_ptr) const noexcept;

  void  print() const noexcept;

};


uint32_t  calculate_png_stream_size(const chunk_list&  ls) noexcept;

bool  read_png_from_stream(      chunk_list&  ls, const uint8_t*  ptr) noexcept;
bool   write_png_to_stream(const chunk_list&  ls,       uint8_t*  ptr) noexcept;

bool  read_png_from_file(      chunk_list&  ls, const char*  path) noexcept;
bool   write_png_to_file(const chunk_list&  ls, const char*  path) noexcept;


}


#endif




