#include"unicode.hpp"
#include<cstdio>




namespace gbstd{
namespace unicodes{


namespace{
size_t
utf8_byte_number(unsigned char  c) noexcept
{
       if(!(c>>7)             ){return 1;}
  else if( (c>>1) == 0b1111110){return 6;}
  else if( (c>>2) == 0b111110 ){return 5;}
  else if( (c>>3) == 0b11110  ){return 4;}
  else if( (c>>4) == 0b1110   ){return 3;}
  else if( (c>>5) == 0b110    ){return 2;}
  else if( (c>>6) == 0b10     ){return 1;}

  printf("%dはUTF8の先頭ではありません\n",c);

  return 0;
}
}


size_t
u8slen(gbstd::string_view  sv) noexcept
{
  size_t  len = 0;

  utf8_decoder  dec(sv);

    while(dec)
    {
      dec();

      ++len;
    }


  return len;
}


namespace{
int
decode(int  c, int  shift_amount=0) noexcept
{
  return((c&0b111111)<<shift_amount);
}
int
encode(int  c, int  shift_amount=0) noexcept
{
  return((c>>shift_amount)&0b111111);
}
}


char32_t
utf8_decoder::
operator()() noexcept
{
  char32_t  c = 0;

  const auto  n = utf8_byte_number(*m_pointer);

    if((m_pointer+n) > m_end)
    {
      m_pointer = m_end;

      return 0;
    }


    switch(n)
    {
  case(1): c = ((m_pointer[0]        )    )                                            ;break;
  case(2): c = ((m_pointer[0]&0b11111)<< 6)|decode(m_pointer[1])                       ;break;
  case(3): c = ((m_pointer[0]&0b01111)<<12)|decode(m_pointer[1],6)|decode(m_pointer[2]);break;

  case(4):
    c = ((m_pointer[0]&0b111)<<18)|decode(m_pointer[1],12)|
                                   decode(m_pointer[2], 6)|
                                   decode(m_pointer[3]   );
    break;
  case(5):
    c = ((m_pointer[0]&0b11)<<24)|decode(m_pointer[1],18)|
                                  decode(m_pointer[2],12)|
                                  decode(m_pointer[3], 6)|
                                  decode(m_pointer[4]   );
    break;
  case(6):
    c = ((m_pointer[0]&0b1)<<30)|decode(m_pointer[1],24)|
                                 decode(m_pointer[2],18)|
                                 decode(m_pointer[3],12)|
                                 decode(m_pointer[4], 6)|
                                 decode(m_pointer[5]   );
    break;
  default:
      printf("不正なUTF8のバイト数です(%d)\n",n);
      return 0;
    }


  m_pointer += n;

  return c;
}




utf8_encoder&
utf8_encoder::
operator()(char32_t  c) noexcept
{
  char*  p = codes;

    if(c <= 0x7F)
    {
      *p++ = c;
    }

  else
    if((c >= 0x0080) &&
       (c <= 0x07FF))
    {
      *p++ = (0b11000000|(c>>6));
      *p++ = (0b10000000|encode(c));
    }

  else
    if((c >= 0x0800) &&
       (c <= 0xFFFF))
    {
      *p++ = (0b11100000|(c>>12));
      *p++ = (0b10000000|encode(c,6));
      *p++ = (0b10000000|encode(c  ));
    }

  else
    if((c >= 0x010000) &&
       (c <= 0x1FFFFF))
    {
      *p++ = (0b11110000|(c>>18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    if((c >= 0x0200000) &&
       (c <= 0x3FFFFFF))
    {
      *p++ = (0b11111000|(c>>24));
      *p++ = (0b10000000|encode(c,18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    if((c >= 0x04000000) &&
       (c <= 0x7FFFFFFF))
    {
      *p++ = (0b11111100|(c>>30));
      *p++ = (0b10000000|encode(c,24));
      *p++ = (0b10000000|encode(c,18));
      *p++ = (0b10000000|encode(c,12));
      *p++ = (0b10000000|encode(c, 6));
      *p++ = (0b10000000|encode(c   ));
    }

  else
    {
      printf("%dはユニコードではありません\n",c);
    }


  *p = 0;

  return *this;
}


}}




