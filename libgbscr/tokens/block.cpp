#include"libgbscr/token.hpp"
#include"libgbscr/stream.hpp"
#include<vector>


namespace  gbscr{
namespace tokens{




block::
block(stream&  s, char  open, char  close):
m_open(open),
m_close(close)
{
    for(;;)
    {
      s.skip_spaces();

      auto  c = s.get_char();

        if(c == close)
        {
          s.advance();

          break;
        }

      else
        {
          auto  tok = s.read_token();

            if(tok)
            {
              m_tokens.emplace_back(std::move(tok));
            }

          else
            {
              break;
            }
        }
    }
}




void
block::
print(int  indent) const noexcept
{
    if(m_open)
    {
      printf("%c",m_open);
    }


  auto  it     = begin();
  auto  it_end =   end();

    if(it != it_end)
    {
      it++->print(indent);

        while(it != it_end)
        {
          it++->print(indent);

          printf("\n");
        }
    }


    if(m_close)
    {
      printf("%c",m_close);
    }
}


}}




