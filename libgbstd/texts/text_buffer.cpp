#include"libgbstd/text.hpp"
#include"libgbstd/environment.hpp"
#include"libgbstd/unicode.hpp"
#include<cctype>




namespace gbstd{
namespace texts{




namespace{
bool  islower(char  c) noexcept{return((c >= 'a') && (c <= 'z'));}
bool  isupper(char  c) noexcept{return((c >= 'A') && (c <= 'Z'));}
bool  isalpha(char  c) noexcept{return(isupper(c) || islower(c));}
bool  isdigit(char  c) noexcept{return((c >= '0') && (c <= '9'));}
bool  isalnum(char  c) noexcept{return(isalpha(c) || isdigit(c));}

bool  isident0(char  c) noexcept{return(isalpha(c) || (c == '_'));}
bool  isidentn(char  c) noexcept{return(isalnum(c) || (c == '_'));}
}




void
text_buffer::
clear() noexcept
{
  delete[] m_data_source          ;
           m_data_source = nullptr;

  m_data_length = 0;
}


void
text_buffer::
reset() noexcept
{
  m_input_pointer = m_data_source;

  m_decoder = gbstd::string_view(m_data_source,m_data_length);

  m_input_pointer[0] = 0;
}


void
text_buffer::
resize(size_t  length) noexcept
{
  clear();

  m_data_source = new char[length+1];

  m_data_length = length;

  reset();
}


namespace{
void
scan(gbstd::string_view::iterator&  src,
     gbstd::string_view::iterator   src_end, char*  dst, size_t  n) noexcept
{
    while((n > 1) && (src != src_end))
    {
      auto  c = *src++;

        if(c == ')')
        {
          break;
        }

      else
        {
          *dst++ = c;

          --n;
        }
    }


  *dst = 0;
}
}


void
text_buffer::
push(gbstd::string_view  sv, bool  with_newline)
{
  auto   it = sv.begin();
  auto  end = sv.end();
    
    while((it != end) && (m_input_pointer < m_decoder.get_end()))
    {
        if(*it == '$')
        {
          ++it;

            if(*it == '$')
            {
              *m_input_pointer++ = *it++;
            }

          else
            if(*it == '(')
            {
              char  buf[256];

              scan(++it,end,buf,sizeof(buf));

              push(environment::get_value(buf).data(),false);
            }

          else
            {
              printf("$の後に$か(が続いていない\n");
            }
        }

      else
        {
          *m_input_pointer++ = *it++;
        }
    }


    if(with_newline)
    {
        if(m_input_pointer < m_decoder.get_end())
        {
          *m_input_pointer++ = '\n';
        }
    }


  *m_input_pointer = 0;
}


char16_t
text_buffer::
pop() noexcept
{
    if(m_decoder)
    {
      return m_decoder();
    }


  return 0;
}




}}




