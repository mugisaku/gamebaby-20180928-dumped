#include"libgbstd/text.hpp"




namespace gbstd{
namespace texts{


namespace{
size_t
get_buffer_length(int  col_n, int  row_n) noexcept
{
  return (col_n+1)*row_n;
}


void
fill_by_zero(char16_t*  begin, char16_t*  end) noexcept
{
  auto  p = begin;

    while(p != end)
    {
      *p++ = 0;
    }
}


}


void
text_roll::
clear() noexcept
{
  delete[] m_data_source          ;
           m_data_source = nullptr;
  delete[] m_line_source;
           m_line_source = nullptr;
}


void
text_roll::
reset() noexcept
{
  fill_by_zero(m_data_source,m_data_source_end);

  auto  cur = m_first;

    while(cur)
    {
      cur->current = cur->begin;

      cur = cur->next;
    }


  m_current = m_first;
}


void
text_roll::
resize(int  col_n, int  row_n) noexcept
{
    if(row_n <= 1)
    {
      printf("[resize error]\n");

      return;
    }


  clear();

  m_number_of_columns = col_n;
  m_number_of_rows    = row_n;


  auto  len = get_buffer_length(col_n,row_n);

  m_data_source     = new char16_t[len];
  m_data_source_end = m_data_source+len;

  m_line_source = new line[row_n];

  char16_t*  data_p = m_data_source;
  line*      line_p = m_line_source;

  m_first   = line_p;
  m_current = line_p;

    if(row_n-- > 0)
    {
      ++line_p;

      m_current->begin = data_p         ;
                         data_p += col_n;
      m_current->end   = data_p++       ;

        while(row_n--)
        {
                      m_current->next = line_p++;
          m_current = m_current->next           ;

          m_current->begin = data_p         ;
                             data_p += col_n;
          m_current->end   = data_p++       ;
        }
    }


  m_last       = m_current;
  m_last->next =   nullptr;

  reset();
}


void
text_roll::
rotate() noexcept
{
  auto  old_first = m_first                            ;
                    m_first = old_first->next          ;
                              old_first->next = nullptr;

  m_last->next = old_first;
  m_last       = old_first;
  m_current    = old_first;

  m_current->current = m_current->begin;

  fill_by_zero(m_current->begin,m_current->end);
}


void
text_roll::
push(char16_t  c) noexcept
{
    if(!is_full() && c)
    {
        if(c == '\n')
        {
          m_current = m_current->next;
        }

      else
        if(m_current->current != m_current->end)
        {
//printf("%s\n",gbstd::utf8_encoder(c).codes);

          *m_current->current++ = c;

            if(m_current->current == m_current->end)
            {
              m_current = (m_current == m_last)? nullptr:m_current->next;
            }
        }
    }
}



bool
text_roll::
is_full() const noexcept
{
  return !m_current;
}



}}




