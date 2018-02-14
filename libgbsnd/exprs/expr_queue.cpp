#include"libgbsnd/expr.hpp"


namespace gbsnd{
namespace exprs{


struct
expr_queue::
node
{
  exprs::expr  expr;;

  node*  next;

};


void
expr_queue::
push(const expr&  e) noexcept
{
    if()
  m_expr_list.emplace_back(e);

    if(!m_current)
    {
      m_current = e.begin();
      m_end     = e.end();
    }
}


const expr_element&
expr_queue::
pop() noexcept
{
/*
  const auto  n = m_expr_list.size();

    if(!n)
    {
      return nullptr;
    }


START:
    if(m_current < m_end)
    {
      return m_current++;
    }

  else
    if(m_index < (n-1))
    {
      auto&  e = m_expr_list[++m_index];

      m_current = e.begin();
      m_end     = e.end();

      goto START;
    }

  else
    {
      m_expr_list.clear();

      m_index = 0;

      m_current = nullptr;
      m_end     = nullptr;
    }


*/
}



}}




