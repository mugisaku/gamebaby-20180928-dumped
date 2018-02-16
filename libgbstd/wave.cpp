#include"libgbstd/wave.hpp"
#include<cstdlib>
#include<cstdio>



namespace gbstd{
namespace waves{


void
wave::
read_fmt( const riff_subchunk_view&  rv) noexcept
{
  auto  p = rv.data();

  auto  format_id = bget_le16(p);

    if(format_id != 1)
    {
      printf("リニアPCMではない\n");
      return;
    }


  p += 2;

  m_number_of_channels = bget_le16(p);

  p += 2;

  m_sampling_rate = bget_le32(p);

  p += 4;

  m_byte_rate = bget_le32(p);

  p += 4;

  m_block_align = bget_le16(p);

  p += 2;

  m_number_of_bits_per_sample = bget_le16(p);

  p += 2;
}


void
wave::
read_data(const riff_subchunk_view&  rv) noexcept
{
  m_length = rv.size();
  m_data   = rv.data();
}


void
wave::
assign(const riff_subchunk_view&  rv) noexcept
{
    if(rv.id() != riff_id('W','A','V','E'))
    {
      printf("無効なWAVE\n");
      return;
    }


  auto    p =   rv.data();
  auto  end = p+rv.size();

    while(p < end)
    {
      riff_subchunk_view  child_rv(p);

        if(child_rv.id() == riff_id('f','m','t',' '))
        {
          read_fmt(child_rv);
        }

      else
        if(child_rv.id() == riff_id('d','a','t','a'))
        {
          read_data(child_rv);
        }

      else
        if(child_rv.id() == riff_id('L','I','S','T'))
        {
        }

      else
        {
          child_rv.print();

          printf("不明なチャンク\n");

          break;
        }


      p = child_rv.data()+child_rv.size();
    }
}


namespace{
void
fput_le32(uint32_t  i, FILE*  f) noexcept
{
  fputc((i    )&0xFF,f);
  fputc((i>> 8)&0xFF,f);
  fputc((i>>16)&0xFF,f);
  fputc((i>>24)&0xFF,f);
}
void
fput_le16(uint16_t  i, FILE*  f) noexcept
{
  fputc((i    )&0xFF,f);
  fputc((i>> 8)&0xFF,f);
}
}


void
wave::
save_to_file(FILE*  f) const noexcept
{
  fwrite("RIFF",1,4,f);

  fpos_t  riff_size_pos;

  fput_le32(4+4+4+2+2+4+4+2+2+4+4+length(),f);

  fwrite("WAVE",1,4,f);
  fwrite("fmt ",1,4,f);

  fput_le32(16,f);

  fput_le16( 1,f);//format_id
  fput_le16( number_of_channels(),f);
  fput_le32(sampling_rate(),f);
  fput_le32(byte_rate(),f);
  fput_le16(block_align(),f);
  fput_le16(number_of_bits_per_sample(),f);

  fwrite("data",1,4,f);

  fput_le32(length(),f);

  fwrite(m_data,1,length(),f);
}


void
wave::
print() const noexcept
{
  printf("       number of channels: %8d\n",number_of_channels());
  printf("            sampling rate: %8d\n",sampling_rate());
  printf("                byte rate: %8d\n",byte_rate());
  printf("              block align: %8d\n",block_align());
  printf("number of bits per sample: %8d\n",number_of_bits_per_sample());
  printf("              data length: %8d\n",length());
}


}}




