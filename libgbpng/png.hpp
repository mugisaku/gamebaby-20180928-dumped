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
class chunk_set;
class image;
class image_header;
class movie;
class palette;








class
binary
{
  uint32_t  m_data_size=0;

  uint8_t*  m_data=nullptr;

public:
 constexpr binary() noexcept{}
   binary(const void*  ptr, uint32_t  size) noexcept{assign(ptr,size);}
   binary(                  uint32_t  size) noexcept{resize(    size);}
   binary(const binary&   rhs) noexcept{assign(rhs);}
   binary(      binary&&  rhs) noexcept{assign(std::move(rhs));}
  ~binary(){clear_data();}

  const uint8_t&  operator[](int  i) const noexcept{return m_data[i];}

  operator bool() const noexcept{return m_data_size;}

  binary&  operator=(const binary&   rhs) noexcept{return assign(rhs);}
  binary&  operator=(      binary&&  rhs) noexcept{return assign(std::move(rhs));}

  binary&  assign(const binary&   rhs) noexcept;
  binary&  assign(      binary&&  rhs) noexcept;
  binary&  assign(const void*  ptr, uint32_t  size) noexcept;

  void  resize(uint32_t  size) noexcept;

  void  clear_data() noexcept;

  void  write(const uint8_t*  ptr, size_t  size, uint32_t  i) noexcept;

  const uint8_t*  get_data() const noexcept{return m_data;}

  uint32_t  get_data_size() const noexcept{return m_data_size;}

  uint8_t*  begin() const noexcept{return m_data;}
  uint8_t*    end() const noexcept{return m_data+m_data_size;}

  binary    get_compressed() const noexcept;
  binary  get_uncompressed() const noexcept;

  binary    get_filtered(const image_header&  ihdr) const noexcept;
  binary  get_unfiltered(const image_header&  ihdr) const noexcept;

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

  bool  operator==(const chunk_name&  name) const noexcept{return m_name == name;}
  bool  operator!=(const chunk_name&  name) const noexcept{return m_name != name;}


  const chunk_name&  get_name() const noexcept{return m_name;}
  uint32_t            get_crc() const noexcept{return m_crc;}

  uint32_t  get_stream_size() const noexcept{return 12+get_data_size();}

        uint8_t*  write(      uint8_t*  dst) const noexcept;
  const uint8_t*   read(const uint8_t*  src)       noexcept;

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
image_data: public binary
{
public:
  using binary::binary;

  image_data(const std::vector<const chunk*>&  ls) noexcept{assign(ls);}

  image_data&  assign(const std::vector<const chunk*>&  ls) noexcept;

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};


class
frame_data: public image_data
{
  uint32_t  m_sequence_number=0;

public:
  frame_data(uint32_t  seq_num, const std::vector<const chunk*>&  ls) noexcept{assign(seq_num,ls);}
  frame_data(uint32_t  seq_num, image_data&&  idat) noexcept{assign(seq_num,std::move(idat));}

  frame_data&  assign(uint32_t  seq_num, const std::vector<const chunk*>&  ls) noexcept;
  frame_data&  assign(uint32_t  seq_num, image_data&&  idat) noexcept;

  uint32_t  get_sequence_number() const noexcept{return m_sequence_number;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




class
animation_control
{
  uint32_t  m_number_of_frames=0;
  uint32_t  m_number_of_plays=0;

public:
  animation_control() noexcept{}
  animation_control(const chunk&  chk) noexcept{assign(chk);}

  animation_control&  operator=(const chunk&  chk) noexcept{return assign(chk);}
  animation_control&  assign(const chunk&  chk) noexcept;

  uint32_t  get_number_of_frames() const noexcept{return m_number_of_frames;}
  uint32_t  get_number_of_plays()  const noexcept{return m_number_of_plays;}

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
frame_control
{
  uint32_t  m_sequence_number=0;

  uint32_t  m_x_offset=0;
  uint32_t  m_y_offset=0;

  uint32_t  m_width=0;
  uint32_t  m_height=0;

  uint16_t  m_delay_numerator=0;
  uint16_t  m_delay_denominator=0;

  dispose_type  m_dispose_type=dispose_type::none;
  blend_type    m_blend_type  =blend_type::source;

public:
  frame_control() noexcept{}
  frame_control(const chunk&  chk) noexcept{assign(chk);}

  frame_control&  operator=(const chunk&  chk) noexcept{return assign(chk);}
  frame_control&  assign(const chunk&  chk) noexcept;

  uint32_t  get_sequence_number(           ) const noexcept{return m_sequence_number    ;}
  void      set_sequence_number(uint32_t  n)       noexcept{       m_sequence_number = n;}

  uint32_t  get_x_offset() const noexcept{return m_x_offset;}
  uint32_t  get_y_offset() const noexcept{return m_y_offset;}

  void  set_x_offset(uint32_t  v) noexcept{m_x_offset = v;}
  void  set_y_offset(uint32_t  v) noexcept{m_y_offset = v;}

  uint32_t  get_width()  const noexcept{return m_width;}
  uint32_t  get_height() const noexcept{return m_height;}

  void  set_width( uint32_t  v) noexcept{m_width  = v;}
  void  set_height(uint32_t  v) noexcept{m_height = v;}

  uint16_t  get_delay_numerator()   const noexcept{return m_delay_numerator;}
  uint16_t  get_delay_denominator() const noexcept{return m_delay_denominator;}

  void  set_delay_numerator(uint16_t  v)   noexcept{m_delay_numerator   = v;}
  void  set_delay_denominator(uint16_t  v) noexcept{m_delay_denominator = v;}

  dispose_type  get_dispose_type() const noexcept{return m_dispose_type;}
  blend_type    get_blend_type()   const noexcept{return m_blend_type;}

  void  set_dispose_type(dispose_type  t) noexcept{m_dispose_type = t;}
  void    set_blend_type(blend_type  t)   noexcept{m_blend_type   = t;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

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
   image(const image_header&  ihdr, const palette*  plte, const image_data&  idat) noexcept{assign(ihdr,plte,idat);}
   image(const chunk_list&  ls) noexcept{assign(ls);}
   image(const image&   rhs) noexcept{*this = rhs;}
   image(      image&&  rhs) noexcept{*this = std::move(rhs);}
  ~image(){delete[] m_data;}

   image&  operator=(const image&   rhs) noexcept;
   image&  operator=(      image&&  rhs) noexcept;
   image&  operator=(const chunk_list&  ls) noexcept{return assign(ls);}

  image&  assign(int  w, int  h,       uint8_t*      data) noexcept;
  image&  assign(int  w, int  h, const uint8_t*  src_data) noexcept;
  image&  assign(const image_header&  ihdr, const palette*  plte, const image_data&  idat) noexcept;
  image&  assign(const chunk_list&  ls) noexcept;

  void  resize(int  w, int  h) noexcept;

  void  clear() noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

        uint8_t*  get_rgba_pointer(int  x, int  y)       noexcept{return &m_data[(4*m_width*y)+(4*x)];}
  const uint8_t*  get_rgba_pointer(int  x, int  y) const noexcept{return &m_data[(4*m_width*y)+(4*x)];}

  image_header  make_image_header() const noexcept;
  image_data    make_image_data() const noexcept;

};




class
animation_element
{
  const chunk*  m_fctl;

  std::vector<const chunk*>  m_fdat;

public:
  animation_element(const chunk*  fctl) noexcept: m_fctl(fctl){}

  void  append(const chunk*  fdat) noexcept{m_fdat.emplace_back(fdat);}

};


class
animation_frame
{
  image  m_image;

  uint32_t  m_sequence_number=0;

  uint16_t  m_delay_numerator  =0;
  uint16_t  m_delay_denominator=0;

public:
  animation_frame(image&&  img, const frame_control&  fctl) noexcept{assign(std::move(img),fctl);}

  animation_frame&  assign(image&&  img, const frame_control&  fctl) noexcept;

  const image&  get_image() const noexcept{return m_image;}

  chunk  make_data_chunk() const noexcept;
  chunk  make_control_chunk() const noexcept;

};




class
chunk_list
{
  std::vector<chunk>  m_container;

public:
  uint32_t  calculate_stream_size() const noexcept;

  bool  read_png_from_stream(const uint8_t*  p) noexcept;
  bool  read_png_from_file(const char*  path) noexcept;

  bool  write_png_to_stream(uint8_t*  p) const noexcept;
  bool  write_png_to_file(const char*  path) const noexcept;

  const chunk*  begin() const noexcept{return m_container.data();}
  const chunk*    end() const noexcept{return m_container.data()+m_container.size();}
  
  void  print() const noexcept;

};




class
chunk_set
{
  void  read_after_ihdr(const chunk*&  it, const chunk*  it_end) noexcept;
  void  read_idat(      const chunk*&  it, const chunk*  it_end) noexcept;
  void  read_after_idat(const chunk*&  it, const chunk*  it_end) noexcept;
  void  read_fdat(      const chunk*&  it, const chunk*  it_end, animation_element*  e) noexcept;

  const chunk*  m_ihdr=nullptr;

  std::vector<const chunk*>  m_between_ihdr_and_idat;

  const chunk*  m_actl=nullptr;
  const chunk*  m_plte=nullptr;
  const chunk*  m_top_fctl=nullptr;

  std::vector<const chunk*>  m_idat;

  std::vector<const chunk*>  m_after_idat;

  std::vector<animation_element>  m_animation_elements;

public:
  chunk_set() noexcept{clear();}
  chunk_set(const chunk_list&  ls) noexcept{assign(ls);}

  chunk_set&  assign(const chunk_list&  ls) noexcept;

  void  clear() noexcept;

  bool  is_animation() const noexcept{return m_actl;}

  image_header  get_image_header() const noexcept{return image_header(*m_ihdr);}
  image_data    get_image_data()   const noexcept{return image_data(m_idat);}

  const chunk*  get_actl_chunk() const noexcept{return m_actl;}
  const chunk*  get_plte_chunk() const noexcept{return m_plte;}
  const chunk*  get_top_fctl_chunk() const noexcept{return m_top_fctl;}

  void  print() const noexcept;

};


class
movie
{
  int  m_width =0;
  int  m_height=0;

  uint32_t  m_number_of_plays=0;

  std::vector<animation_frame>  m_frame_list;

public:
  movie(const chunk_list&  ls) noexcept{assign(ls);}
  movie(const chunk_set&  set) noexcept{assign(set);}

  movie&  operator=(const chunk_list&  ls) noexcept{return assign(ls);}
  movie&  operator=(const chunk_set&  set) noexcept{return assign(set);}
  movie&  assign(const chunk_list&  ls) noexcept;
  movie&  assign(const chunk_set&  set) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  uint32_t  get_number_of_plays(           ) const noexcept{return m_number_of_plays    ;}
  void      set_number_of_plays(uint32_t  n)       noexcept{       m_number_of_plays = n;}

  const std::vector<animation_frame>&  get_frame_list() const noexcept{return m_frame_list;}

  chunk  make_control_chunk() const noexcept;

};


}


#endif




