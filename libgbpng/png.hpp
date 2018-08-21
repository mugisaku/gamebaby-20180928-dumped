#ifndef gbpng_hpp_included
#define gbpng_hpp_included


#include<cstdint>
#include<cstdio>
#include<cstring>
#include<utility>
#include<list>
#include<vector>


#ifndef report
#define report  printf("%s %d: %s\n",__FILE__,__LINE__,__func__);
#endif


namespace gbpng{


class chunk_list;
class chunk_set;
class image;
class image_header;
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

        uint8_t&  operator[](int  i)       noexcept{return m_data[i];}
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

        uint8_t*  get_data()       noexcept{return m_data;}
  const uint8_t*  get_data() const noexcept{return m_data;}

  uint32_t  get_data_size() const noexcept{return m_data_size;}

  uint8_t*  begin() const noexcept{return m_data;}
  uint8_t*    end() const noexcept{return m_data+m_data_size;}

  binary    get_compressed() const noexcept;
  binary  get_uncompressed() const noexcept;

};




binary    get_filtered(const uint8_t*  src, const image_header&  ihdr) noexcept;
binary  get_unfiltered(const uint8_t*  src, const image_header&  ihdr) noexcept;




class
binary_view
{
  const uint8_t*  m_pointer=nullptr;

public:
  binary_view() noexcept{}
  binary_view(const uint8_t*  p) noexcept: m_pointer(p){}
  binary_view(const binary&  bin) noexcept{assign(bin);}

  binary_view&  operator=(const binary&  bin) noexcept{return assign(bin);}
  binary_view&  assign(const binary&  bin) noexcept;

  const uint8_t*  get_pointer() const noexcept{return m_pointer;}

  void  advance(int  n) noexcept{m_pointer += n;}

  uint8_t   get_8()    noexcept{return *m_pointer++;}
  uint16_t  get_be16() noexcept;
  uint32_t  get_be32() noexcept;

};


class
binary_cursor
{
  uint8_t*  m_pointer=nullptr;

public:
  binary_cursor() noexcept{}
  binary_cursor(uint8_t*  p) noexcept: m_pointer(p){}
  binary_cursor(const binary&  bin) noexcept{assign(bin);}

  binary_cursor&  operator=(const binary&  bin) noexcept{return assign(bin);}
  binary_cursor&  assign(const binary&  bin) noexcept;

  uint8_t*  get_pointer() const noexcept{return m_pointer;}

  void  advance(int  n) noexcept{m_pointer += n;}

  void     put_8(uint8_t   i) noexcept{*m_pointer++ = i;}
  void  put_be16(uint16_t  i) noexcept;
  void  put_be32(uint32_t  i) noexcept;

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
  palette(const chunk&  chk) noexcept{assign(chk);}

  palette&  assign(const chunk&  chk) noexcept;

  int    find_color(const color&  color) const noexcept;
  int  append_color(const color&  color)       noexcept;

        color&  get_color(int  i)       noexcept{return m_colors[i];}
  const color&  get_color(int  i) const noexcept{return m_colors[i];}

  int   get_number_of_colors(      ) const noexcept{return m_number_of_colors         ;}
  void  set_number_of_colors(int  n)       noexcept{       m_number_of_colors = n&0xFF;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




enum class
pixel_format
{
  grayscale            = 0,
  grayscale_with_alpha = 4,
  indexed              = 3,
  rgb                  = 2,
  rgba                 = 6,
  unknown              = 8,
  null                 = 16,

};


constexpr const char*
get_pixel_format_name(pixel_format  fmt) noexcept
{
    switch(fmt)
    {
  case(pixel_format::grayscale           ): return "grayscale";
  case(pixel_format::grayscale_with_alpha): return "grayscale with alpha";
  case(pixel_format::indexed             ): return "indexed color";
  case(pixel_format::rgb                 ): return "RGB";
  case(pixel_format::rgba                ): return "RGBA";
  case(pixel_format::unknown             ): return "unknown";
  case(pixel_format::null                ): return "null";
    }


  return "unknown";
}


constexpr int
get_number_of_bytes_per_pixel(pixel_format  fmt) noexcept
{
    switch(fmt)
    {
  case(pixel_format::grayscale           ): return 1;
  case(pixel_format::grayscale_with_alpha): return 2;
  case(pixel_format::indexed             ): return 1;
  case(pixel_format::rgb                 ): return 3;
  case(pixel_format::rgba                ): return 4;
  case(pixel_format::unknown             ): return 1;
  case(pixel_format::null                ): return 1;
    }


  return 0;
}



class
image_header
{
  int  m_width =0;
  int  m_height=0;

  int  m_bit_depth=8;

  int  m_number_of_bytes_per_pixel=0;

  pixel_format  m_pixel_format=pixel_format::rgba;

  bool  m_interlaced=false;

public:
  image_header() noexcept{}
  image_header(int  w, int  h) noexcept: m_width(w), m_height(h){}
  image_header(int  w, int  h, pixel_format  fmt) noexcept: m_width(w), m_height(h){set_pixel_format(fmt);}
  image_header(const chunk&  chk) noexcept;

  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

  int   get_bit_depth(      ) const noexcept{return m_bit_depth    ;}
  void  set_bit_depth(int  v)       noexcept{       m_bit_depth = v;}

  void                 set_pixel_format(pixel_format  fmt)       noexcept;
  const pixel_format&  get_pixel_format(                 ) const noexcept{return m_pixel_format;}

  int  get_number_of_bytes_per_pixel() const noexcept{return m_number_of_bytes_per_pixel;}

  int  get_pitch() const noexcept{return m_number_of_bytes_per_pixel*m_width;}

  int  get_image_size() const noexcept{return get_pitch()*m_height;}

  bool  is_interlaced() const noexcept{return m_interlaced;}

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




class
image_data: public binary
{
public:
  using binary::binary;

  image_data(const uint8_t*  ptr, const image_header&  ihdr) noexcept{assign(ptr,ihdr);}
  image_data(const std::vector<const chunk*>&  ls) noexcept{assign(ls);}

  image_data&  assign(const std::vector<const chunk*>&  ls) noexcept;
  image_data&  assign(const uint8_t*  ptr, const image_header&  ihdr) noexcept;

  chunk  make_chunk() const noexcept;

  binary  extract(const image_header&  ihdr) const noexcept;

  void  print() const noexcept;

};




class
image
{
  int  m_width =0;
  int  m_height=0;

  binary  m_binary;

protected:
  void  allocate(int  bpp, int  w, int  h) noexcept;
  void  store(binary&&  bin, int  w, int  h) noexcept;

        uint8_t*  get_row_pointer(int  bpp, int  y)       noexcept{return m_binary.get_data()+((bpp*m_width)*y);}
  const uint8_t*  get_row_pointer(int  bpp, int  y) const noexcept{return m_binary.get_data()+((bpp*m_width)*y);}

        uint8_t*  get_pixel_pointer(int  bpp, int  x, int  y)       noexcept{return get_row_pointer(bpp,y)+(bpp*x);}
  const uint8_t*  get_pixel_pointer(int  bpp, int  x, int  y) const noexcept{return get_row_pointer(bpp,y)+(bpp*x);}

public:
  int  get_width()  const noexcept{return m_width ;}
  int  get_height() const noexcept{return m_height;}

};


class direct_color_image;


class
indexed_color_image: public image
{
  palette  m_palette;

public:
  indexed_color_image() noexcept{}
  indexed_color_image(const direct_color_image&  src_img) noexcept{assign(src_img);}

  indexed_color_image&  operator=(const direct_color_image&  src_img) noexcept{return assign(src_img);}
  indexed_color_image&     assign(const direct_color_image&  src_img) noexcept;

  void  allocate(int  w, int  h) noexcept{image::allocate(1,w,h);}

         palette&  get_palette()       noexcept{return m_palette;}
   const palette&  get_palette() const noexcept{return m_palette;}

        uint8_t*  get_row_pointer(int  y)       noexcept{return image::get_row_pointer(1,y);}
  const uint8_t*  get_row_pointer(int  y) const noexcept{return image::get_row_pointer(1,y);}

        uint8_t*  get_pixel_pointer(int  x, int  y)       noexcept{return get_row_pointer(y)+(x);}
  const uint8_t*  get_pixel_pointer(int  x, int  y) const noexcept{return get_row_pointer(y)+(x);}

  void  save_file(const char*  path, int  bit_depth) const noexcept;

  image_data  get_image_data(int  bit_depth) const noexcept;

};


class image_source;


class
direct_color_image: public image
{
public:
  direct_color_image() noexcept{}
  direct_color_image(image_source&  isrc) noexcept{assign(isrc);}
  direct_color_image(const char*  path) noexcept{load_file(path);}

  direct_color_image&  assign(image_source&  isrc) noexcept;

  void  save_file(const char*  path, pixel_format  fmt, int  bit_depth) const noexcept;
  void  load_file(const char*  path) noexcept;

  void  allocate(int  w, int  h) noexcept{image::allocate(4,w,h);}

        uint8_t*  get_row_pointer(int  y)       noexcept{return image::get_row_pointer(4,y);}
  const uint8_t*  get_row_pointer(int  y) const noexcept{return image::get_row_pointer(4,y);}

        uint8_t*  get_pixel_pointer(int  x, int  y)       noexcept{return get_row_pointer(y)+(4*x);}
  const uint8_t*  get_pixel_pointer(int  x, int  y) const noexcept{return get_row_pointer(y)+(4*x);}

  image_data  get_image_data(pixel_format  fmt, int  bit_depth) const noexcept;

};




class
transparency_info
{
  pixel_format  m_pixel_format=pixel_format::null;

  int  m_number_of_values=0;

  union{
    uint8_t*   m_uint8 =nullptr;
    uint16_t*  m_uint16;
  };

public:
  transparency_info() noexcept{}
  transparency_info(const transparency_info&   rhs) noexcept{assign(rhs);}
  transparency_info(      transparency_info&&  rhs) noexcept{assign(std::move(rhs));}
  transparency_info(const chunk&  chk, pixel_format  fmt) noexcept{assign(chk,fmt);}
 ~transparency_info() noexcept{clear();}

  transparency_info&  operator=(const transparency_info&   rhs) noexcept{return assign(rhs);}
  transparency_info&  operator=(      transparency_info&&  rhs) noexcept{return assign(std::move(rhs));}

  transparency_info&  assign(const transparency_info&   rhs) noexcept;
  transparency_info&  assign(      transparency_info&&  rhs) noexcept;
  transparency_info&  assign(const chunk&  chk, pixel_format  fmt) noexcept;

  void  clear() noexcept;

  int   get_number_of_values(      ) const noexcept{return m_number_of_values    ;}
  void  set_number_of_values(int  n)       noexcept{       m_number_of_values = n;}

  uint8_t  get_alpha(uint8_t  i) const noexcept;
  uint8_t  get_alpha(uint16_t  v) const noexcept;
  uint8_t  get_alpha(uint16_t  r, uint16_t  g, uint16_t  b) const noexcept;

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




class
background_info
{
  pixel_format  m_pixel_format=pixel_format::null;

  uint16_t  m_values[3]={0};

public:
  background_info() noexcept{}
  background_info(const chunk&  chk, pixel_format  fmt) noexcept{assign(chk,fmt);}

  background_info&  assign(const chunk&  chk, pixel_format  fmt) noexcept;

  chunk  make_chunk() const noexcept;

  void  print() const noexcept;

};




struct
image_source
{
  image_header       ihdr;
  palette            plte;
  transparency_info  trns;
  background_info    bkgd;

  binary  data;

};




class
chunk_list
{
  std::vector<chunk>  m_container;

public:
  void  push_front(chunk&&  chk) noexcept;
  void  push_back( chunk&&  chk) noexcept;

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

  const chunk*  m_ihdr=nullptr;

  std::vector<const chunk*>  m_between_ihdr_and_idat;

  const chunk*  m_plte=nullptr;
  const chunk*  m_trns=nullptr;
  const chunk*  m_bkgd=nullptr;

  std::vector<const chunk*>  m_idat;

  std::vector<const chunk*>  m_after_idat;

public:
  chunk_set() noexcept{clear();}
  chunk_set(const chunk_list&  ls) noexcept{assign(ls);}

  chunk_set&  assign(const chunk_list&  ls) noexcept;

  void  clear() noexcept;

  image_header  get_image_header() const noexcept{return image_header(*m_ihdr);}
  image_data    get_image_data()   const noexcept{return image_data(m_idat);}

  const chunk*  get_plte_chunk() const noexcept{return m_plte;}
  const chunk*  get_trns_chunk() const noexcept{return m_trns;}
  const chunk*  get_bkgd_chunk() const noexcept{return m_bkgd;}

  void  print() const noexcept;

};






class
frame_data: public image_data
{
  uint32_t  m_sequence_number=0;

public:
  frame_data() noexcept{}
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

  void  set_number_of_frames(uint32_t  n) noexcept{m_number_of_frames = n;}
  void  set_number_of_plays( uint32_t  n) noexcept{m_number_of_plays  = n;}

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
animation_builder
{
  image_header  m_ihdr;

  uint32_t  m_delay_ms=0;

  uint32_t  m_sequence_number=0;

  uint32_t  m_number_of_frames=0;

  std::vector<chunk>  m_buffer;

public:
  animation_builder() noexcept{}
  animation_builder(const image_header&  ihdr, uint32_t  delay_ms) noexcept{reset(ihdr,delay_ms);}

  void  reset(const image_header&  ihdr, uint32_t  delay_ms) noexcept;

  void  append(const uint8_t*  ptr) noexcept;

  chunk_list  build(uint32_t  number_of_plays=0) const noexcept;

};




}


#endif




