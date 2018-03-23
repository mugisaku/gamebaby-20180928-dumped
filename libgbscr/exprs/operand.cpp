#include"libgbscr/expr.hpp"
#include"libgbscr/process.hpp"
#include<new>


namespace gbscr{
namespace exprs{




operand&
operand::
operator=(identifier&&  id) noexcept
{
  gbstd::string_view  sv(id.get_string());

    if(sv == gbstd::string_view("null"))
    {
      *this = value();
    }

  else
    if(sv == gbstd::string_view("true"))
    {
      *this = true;
    }

  else
    if(sv == gbstd::string_view("false"))
    {
      *this = false;
    }

  else
    {
      clear();

      m_kind = kind::identifier;

      new(&m_data) identifier(std::move(id));
    }


  return *this;
}


operand&
operand::
operator=(const expr&  e) noexcept
{
  clear();

  m_kind = kind::expression;

  new(&m_data) expr(e);

  return *this;
}


operand&
operand::
operator=(expr_list&&  els) noexcept
{
  clear();

  m_kind = kind::expression_list;

  new(&m_data) expr_list(std::move(els));

  return *this;
}


operand&
operand::
operator=(paired_expr&&  pe) noexcept
{
  clear();

  m_kind = kind::paired_expression;

  new(&m_data) paired_expr(std::move(pe));

  return *this;
}


operand&
operand::
operator=(const operation&  op) noexcept
{
  clear();

  m_kind = kind::operation;

  new(&m_data) operation(op);

  return *this;
}


operand&
operand::
operator=(const value&  v) noexcept
{
  clear();

  m_kind = kind::value;

  new(&m_data) value(v);

  return *this;
}


operand&
operand::
operator=(const operand&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      m_kind = rhs.m_kind;

        switch(m_kind)
        {
      case(kind::identifier):
          new(&m_data) identifier(rhs.m_data.id);
          break;
      case(kind::expression):
          new(&m_data) expr(rhs.m_data.e);
          break;
      case(kind::expression_list):
          new(&m_data) expr_list(rhs.m_data.els);
          break;
      case(kind::paired_expression):
          new(&m_data) paired_expr(rhs.m_data.pe);
          break;
      case(kind::operation):
          new(&m_data) operation(rhs.m_data.op);
          break;
      case(kind::value):
          new(&m_data) value(rhs.m_data.v);
          break;
        }
    }


  return *this;
}


operand&
operand::
operator=(operand&&  rhs) noexcept
{
    if(this != &rhs)
    {
      clear();

      std::swap(m_kind,rhs.m_kind);

        switch(m_kind)
        {
      case(kind::identifier):
          new(&m_data) identifier(std::move(rhs.m_data.id));
          break;
      case(kind::expression):
          new(&m_data) expr(std::move(rhs.m_data.e));
          break;
      case(kind::expression_list):
          new(&m_data) expr_list(std::move(rhs.m_data.els));
          break;
      case(kind::paired_expression):
          new(&m_data) paired_expr(std::move(rhs.m_data.pe));
          break;
      case(kind::operation):
          new(&m_data) operation(std::move(rhs.m_data.op));
          break;
      case(kind::value):
          new(&m_data) value(std::move(rhs.m_data.v));
          break;
        }
    }


  return *this;
}



void
operand::
clear() noexcept
{
    switch(m_kind)
    {
  case(kind::identifier):
      gbstd::destruct(m_data.id);
      break;
  case(kind::expression):
      gbstd::destruct(m_data.e);
      break;
  case(kind::expression_list):
      gbstd::destruct(m_data.els);
      break;
  case(kind::paired_expression):
      gbstd::destruct(m_data.pe);
      break;
  case(kind::operation):
      gbstd::destruct(m_data.op);
      break;
  case(kind::value):
      gbstd::destruct(m_data.v);
      break;
    }


  new(&m_data) value();

  m_kind = kind::value;
}


value
operand::
evaluate(process&  proc) const
{
    switch(m_kind)
    {
  case(kind::identifier):
      return proc.get_value(m_data.id.get_string());
      break;
  case(kind::expression):
      return m_data.e.evaluate(proc);
      break;
  case(kind::expression_list):
      return m_data.els.back().evaluate(proc);
      break;
  case(kind::paired_expression):
      printf("paired_exprは単純評価できない\n");
      break;
  case(kind::operation):
      return m_data.op.evaluate(proc);
      break;
  case(kind::value):
      return m_data.v;
      break;
    }


  return value();
}


void
operand::
print() const noexcept
{
    switch(m_kind)
    {
  case(kind::identifier):
      printf("%s",m_data.id.get_string().data());
      break;
  case(kind::expression):
      m_data.e.print();
      break;
  case(kind::expression_list):
      printf("(");

        for(auto&  e: m_data.els)
        {
          e.print();

          printf(",");
        }


      printf(")");
      break;
  case(kind::paired_expression):
      m_data.pe.get_left().print();
      printf(":");
      m_data.pe.get_right().print();
      break;
  case(kind::operation):
      m_data.op.print();
      break;
  case(kind::value):
      m_data.v.print();
      break;
    }
}


}}




