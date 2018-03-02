#include"libgbsnd/expr.hpp"
#include"libgbsnd/script.hpp"
#include<new>


namespace gbsnd{
namespace exprs{




expr_element&
expr_element::
operator=(const expr_element&   rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::operand):
          new(&m_data) operand(rhs.m_data.o);
          break;
      case(kind::prefix_unary_operator):
      case(kind::postfix_unary_operator):
      case(kind::binary_operator):
          m_data.opw = rhs.m_data.opw;
          break;
        }
    }


  return *this;
}


expr_element&
expr_element::
operator=(expr_element&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::operand):
          new(&m_data) operand(std::move(rhs.m_data.o));
          break;
      case(kind::prefix_unary_operator):
      case(kind::postfix_unary_operator):
      case(kind::binary_operator):
          m_data.opw = rhs.m_data.opw;
          break;
        }
    }


  return *this;
}




void
expr_element::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::operand):
      gbstd::destruct(m_data.o);
      break;
  case(kind::prefix_unary_operator):
  case(kind::postfix_unary_operator):
  case(kind::binary_operator):
      break;
    }


  m_kind = kind::null;
}


}}




