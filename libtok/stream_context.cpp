#include"stream_reader.hpp"
#include<cstring>




namespace tok{
namespace tok_types{


void
stream_context::
print() const noexcept
{
  printf("[tok stream %4d行]\n",m_line_number);

  auto  p = m_line_start;

    while(*p && (*p != '\n'))
    {
      printf("%c",*p++);
    }


  printf("\n");


  p = m_line_start;

    while(p++ < m_pointer)
    {
      printf(" ");
    }


  printf("[^]\n");
}


}}




