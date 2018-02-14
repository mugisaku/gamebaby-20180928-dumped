#include"libgbstd/binary_stream.hpp"
#include<cstdio>




namespace gbstd{
namespace binary_streams{




binary_stream_reader::
binary_stream_reader(const std::vector<char>&  bin) noexcept:
m_begin(bin.data()),
m_current(bin.data()),
m_end(bin.data()+bin.size())
{
}




int
binary_stream_reader::
get() noexcept
{
  return static_cast<unsigned char>(*m_current++);
}


int
binary_stream_reader::
get_be16() noexcept
{
  int  v = get()<<8;

  v |= get();

  return v;
}


int
binary_stream_reader::
get_be32() noexcept
{
  int  v = get()<<24;

  v |= get()<<16;
  v |= get()<< 8;
  v |= get()    ;

  return v;
}




}}




