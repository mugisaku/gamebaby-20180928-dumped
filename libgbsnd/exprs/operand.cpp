#include"libgbsnd/expr.hpp"
#include"libgbsnd/execution.hpp"
#include<new>


namespace gbsnd{
namespace exprs{




operand&
operand::
operator=(bool  b) noexcept
{
  clear();

  m_kind = kind::boolean_literal;

  m_data.b = b;

  return *this;
}


operand&
operand::
operator=(uint64_t  i) noexcept
{
  clear();

  m_kind = kind::integer_literal;

  m_data.i = i;

  return *this;
}


operand&
operand::
operator=(const identifier&  id) noexcept
{
    if(id == gbstd::string_view("true"))
    {
      *this = true;
    }

  else
    if(id == gbstd::string_view("false"))
    {
      *this = false;
    }

  else
    {
      clear();

      m_kind = kind::identifier;

      new(&m_data) identifier(id);
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
      case(kind::boolean_literal):
          m_data.b = rhs.m_data.b;
          break;
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
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
      case(kind::boolean_literal):
          m_data.b = rhs.m_data.b;
          break;
      case(kind::integer_literal):
          m_data.i = rhs.m_data.i;
          break;
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
  case(kind::integer_literal):
      break;
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


  m_kind = kind::null;
}


value
operand::
evaluate(execution_context*  ctx) const noexcept
{
    switch(m_kind)
    {
  case(kind::boolean_literal):
      return value(m_data.b);
      break;
  case(kind::integer_literal):
      return value(static_cast<int>(m_data.i));
      break;
  case(kind::identifier):
      return ctx? ctx->get_value(m_data.id.view()):value();
      break;
  case(kind::expression):
      return m_data.e.evaluate(ctx);
      break;
  case(kind::expression_list):
      return m_data.els.back().evaluate(ctx);
      break;
  case(kind::paired_expression):
      printf("paired_exprは単純評価できない\n");
      break;
  case(kind::operation):
      return m_data.op.evaluate(ctx);
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
  case(kind::boolean_literal):
      printf("%s",m_data.b? "true":"false");
      break;
  case(kind::integer_literal):
      printf("%lu",m_data.i);
      break;
  case(kind::identifier):
      printf("%s",m_data.id.data());
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




