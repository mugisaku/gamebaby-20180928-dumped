#include"libgbstd/binary_stream.hpp"
#include<cstdio>




namespace gbstd{
namespace binary_streams{




void
binary_stream_writer::
put(int  c) noexcept
{
    if(m_offset < m_stream->get_size())
    {
      m_stream->overwrite(c,m_offset);
    }

  else
    {
      m_stream->append(c);
    }


  m_offset += 1;
}


void
binary_stream_writer::
put_be16(int  c) noexcept
{
  put(c>>8);
  put(c   );
}


void
binary_stream_writer::
put_be32(int  c) noexcept
{
  put(c>>24);
  put(c>>16);
  put(c>> 8);
  put(c    );
}


}}




