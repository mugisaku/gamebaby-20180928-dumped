#include"libgbscr/stream.hpp"
#include<cstring>




namespace gbscr{


void
stream::
skip_linestyle_comment() noexcept
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(!*this)
        {
          break;
        }

      else           
        if(c == '\n')
        {
          ++m_pointer;

          break;
        }

      else
        {
          ++m_pointer;
        }           
    }
}


void
stream::
skip_blockstyle_comment()
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(!*this)
        {
          printf("ブロック式コメントが閉じられていない\n");

          throw m_pointer;
        }

      else           
        if(c == '*')
        {
          ++m_pointer;

            if(*m_pointer == '/')
            {
              ++m_pointer;

              break;
            }
        }

      else
        {
          ++m_pointer;
        }           
    }
}


void
stream::
skip_spaces()
{
    while(*this)
    {
      auto  c = *m_pointer;

        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\n') ||
           (c == '\r') ||
           (c == '\v'))
        {
          ++m_pointer;
        }

      else           
        if(c == '/')
        {
          c = *(m_pointer+1);

            if(c == '*')
            {
              m_pointer += 2;

              skip_blockstyle_comment();
            }

          else
            if(c == '/')
            {
              m_pointer += 2;

              skip_linestyle_comment();
            }

          else
            {
              break;
            }
        }

      else
        {
          break;
        }           
    }
}


}




