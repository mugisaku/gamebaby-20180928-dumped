#include"libgbscr/streams/stream.hpp"
#include<cstring>




namespace gbscr{


void
stream::
skip_linestyle_comment() noexcept
{
    for(;;)
    {
      auto  c = *m_pointer;

        if(is_reached_end())
        {
          break;
        }

      else           
        if(c == '\n')
        {
          ++m_pointer;

          newline();

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

        if(is_reached_end())
        {
          printf("ブロック式コメントが閉じられていない\n");

          throw get_context();
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
        if(c == '\n')
        {
          ++m_pointer;

          newline();
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
    while(!is_reached_end())
    {
      auto  c = *m_pointer;

        if((c ==  ' ') ||
           (c == '\t') ||
           (c == '\r') ||
           (c == '\v'))
        {
          ++m_pointer;
        }

      else           
        if(c == '\n')
        {
          ++m_pointer;

          newline();
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




