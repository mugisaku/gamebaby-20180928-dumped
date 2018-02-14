#include"stream_reader.hpp"
#include<cstring>




namespace tok{
namespace tok_types{


namespace{
bool
isalpha(char  c) noexcept
{
  return(((c >= 'a') && (c <= 'z')) ||
         ((c >= 'A') && (c <= 'Z')));
}


bool
isdigit(char  c) noexcept
{
  return((c >= '0') && (c <= '9'));
}


bool
isalnum(char  c) noexcept
{
  return(isalpha(c) || isdigit(c));
}


bool
isident0(char  c) noexcept
{
  return isalpha(c) || (c == '_');
}


bool
isidentn(char  c) noexcept
{
  return isalnum(c) || (c == '_');
}
}




bool
stream_reader::
operator==(gbstd::string_view  sv) const noexcept
{
    if((m_end-m_pointer) >= sv.size())
    {
      return std::memcmp(m_pointer,sv.data(),sv.size()) == 0;
    }


  return false;
}




bool
stream_reader::
advance_if_matched(gbstd::string_view  sv) noexcept
{
    if(*this == sv)
    {
      m_pointer += sv.size();

      return true;
    }


  return false;
}


bool
stream_reader::
is_pointing_identifier() const noexcept
{
  return isident0(*m_pointer);
}


bool
stream_reader::
is_pointing_number() const noexcept
{
  return isdigit(*m_pointer);
}


void
stream_reader::
newline() noexcept
{
  m_line_start = m_pointer;

  ++m_line_number;
}


gbstd::string_view
stream_reader::
read_identifier() noexcept
{
  m_string_buffer.resize(0,0);

    while(!is_reached_end() && isidentn(*m_pointer))
    {
      m_string_buffer.push_back(*m_pointer++);
    }


  return gbstd::string_view(m_string_buffer.data(),m_string_buffer.size());
}


gbstd::string_view
stream_reader::
read_quoted_string(char  close_char) noexcept
{
  m_string_buffer.resize(0,0);

    while(!is_reached_end())
    {
      auto  c = *m_pointer++;

        if(c == close_char)
        {
          break;
        }

      else
        if(c == '\\')
        {
          break;
        }

      else
        {
          m_string_buffer.push_back(c);
        }
    }


  return gbstd::string_view(m_string_buffer.data(),m_string_buffer.size());
}


}}




