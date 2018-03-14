#include"libgbscr/expr.hpp"
#include"libgbscr/process.hpp"
#include<new>


namespace gbscr{
namespace exprs{




namespace assignments{
void   st(operand&  lo, operand&  ro, process*  proc) noexcept;
void  add(operand&  lo, operand&  ro, process*  proc) noexcept;
void  sub(operand&  lo, operand&  ro, process*  proc) noexcept;
void  mul(operand&  lo, operand&  ro, process*  proc) noexcept;
void  div(operand&  lo, operand&  ro, process*  proc) noexcept;
void  rem(operand&  lo, operand&  ro, process*  proc) noexcept;
void  and_(operand&  lo, operand&  ro, process*  proc) noexcept;
void  or_( operand&  lo, operand&  ro, process*  proc) noexcept;
void  xor_(operand&  lo, operand&  ro, process*  proc) noexcept;
void  shl( operand&  lo, operand&  ro, process*  proc) noexcept;
void  shr( operand&  lo, operand&  ro, process*  proc) noexcept;
}
namespace arithmetics{
void  neg(operand&  o, process*  proc) noexcept;
void  add(operand&  lo, operand&  ro, process*  proc) noexcept;
void  sub(operand&  lo, operand&  ro, process*  proc) noexcept;
void  mul(operand&  lo, operand&  ro, process*  proc) noexcept;
void  div(operand&  lo, operand&  ro, process*  proc) noexcept;
void  rem(operand&  lo, operand&  ro, process*  proc) noexcept;
}
namespace comparisons{
void  eq(  operand&  lo, operand&  ro, process*  proc) noexcept;
void  neq( operand&  lo, operand&  ro, process*  proc) noexcept;
void  lt(  operand&  lo, operand&  ro, process*  proc) noexcept;
void  lteq(operand&  lo, operand&  ro, process*  proc) noexcept;
void  gt(  operand&  lo, operand&  ro, process*  proc) noexcept;
void  gteq(operand&  lo, operand&  ro, process*  proc) noexcept;
}
namespace bitwises{
void  not_(operand&  o, process*  proc) noexcept;
void  and_(operand&  lo, operand&  ro, process*  proc) noexcept;
void  or_( operand&  lo, operand&  ro, process*  proc) noexcept;
void  xor_(operand&  lo, operand&  ro, process*  proc) noexcept;
void  not_(operand&  lo, operand&  ro, process*  proc) noexcept;
void  shl( operand&  lo, operand&  ro, process*  proc) noexcept;
void  shr( operand&  lo, operand&  ro, process*  proc) noexcept;
}
namespace logicals{
void  not_(operand&  o, process*  proc) noexcept;
void  or_( operand&  lo, operand&  ro, process*  proc) noexcept;
void  and_(operand&  lo, operand&  ro, process*  proc) noexcept;
}


void
operate_prefix_unary(operand&  o, operator_word  opw, process*  proc) noexcept
{
  void  (*fn)(operand&  o, process*  proc) noexcept=nullptr;

       if(opw == gbstd::string_view("!")){fn = logicals::not_;}
  else if(opw == gbstd::string_view("~")){fn = bitwises::not_;}
  else if(opw == gbstd::string_view("-")){fn = arithmetics::neg;}

    if(!fn)
    {
      printf("prefix unary operation error\n");
	
      return;
    }


  fn(o,proc);
}


void
operate_postfix_unary(operand&  o, operator_word  opw, process*  proc) noexcept
{
    if(opw == gbstd::string_view("")){}
}


void
operate_binary(operand&  lo, operand&  ro, operator_word  opw, process*  proc) noexcept
{
  void  (*fn)(operand&  lo, operand&  ro, process*  proc) noexcept=nullptr;

       if(opw == gbstd::string_view( "||")){fn = logicals::or_;}
  else if(opw == gbstd::string_view( "&&")){fn = logicals::and_;}
  else if(opw == gbstd::string_view(  "=")){fn = assignments::st;}
  else if(opw == gbstd::string_view( "+=")){fn = assignments::add;}
  else if(opw == gbstd::string_view( "-=")){fn = assignments::sub;}
  else if(opw == gbstd::string_view( "*=")){fn = assignments::mul;}
  else if(opw == gbstd::string_view( "/=")){fn = assignments::div;}
  else if(opw == gbstd::string_view( "%=")){fn = assignments::rem;}
  else if(opw == gbstd::string_view("<<=")){fn = assignments::shl;}
  else if(opw == gbstd::string_view(">>=")){fn = assignments::shr;}
  else if(opw == gbstd::string_view( "|=")){fn = assignments::or_;}
  else if(opw == gbstd::string_view( "&=")){fn = assignments::and_;}
  else if(opw == gbstd::string_view( "^=")){fn = assignments::xor_;}
  else if(opw == gbstd::string_view(  "+")){fn = arithmetics::add;}
  else if(opw == gbstd::string_view(  "-")){fn = arithmetics::sub;}
  else if(opw == gbstd::string_view(  "*")){fn = arithmetics::mul;}
  else if(opw == gbstd::string_view(  "/")){fn = arithmetics::div;}
  else if(opw == gbstd::string_view(  "%")){fn = arithmetics::rem;}
  else if(opw == gbstd::string_view( "<<")){fn = bitwises::shl;}
  else if(opw == gbstd::string_view( ">>")){fn = bitwises::shr;}
  else if(opw == gbstd::string_view(  "|")){fn = bitwises::or_;}
  else if(opw == gbstd::string_view(  "&")){fn = bitwises::and_;}
  else if(opw == gbstd::string_view(  "^")){fn = bitwises::xor_;}
  else if(opw == gbstd::string_view( "==")){fn = comparisons::eq;}
  else if(opw == gbstd::string_view( "!=")){fn = comparisons::neq;}
  else if(opw == gbstd::string_view(  "<")){fn = comparisons::lt;}
  else if(opw == gbstd::string_view( "<=")){fn = comparisons::lteq;}
  else if(opw == gbstd::string_view(  ">")){fn = comparisons::gt;}
  else if(opw == gbstd::string_view( ">=")){fn = comparisons::gteq;}
/*
  else if(opw == gbstd::string_view(  ".")){fn = s::;}
  else if(opw == gbstd::string_view( "->")){fn = s::;}
  else if(opw == gbstd::string_view(  "?")){fn = s::;}
  else if(opw == gbstd::string_view( "()")){fn = s::;}
  else if(opw == gbstd::string_view( "[]")){fn = s::;}
  else if(opw == gbstd::string_view( "::")){fn = s::;}
  else if(opw == gbstd::string_view(  ",")){fn = s::;}
*/

    if(!fn)
    {
      printf("binary operation error\n");

      return;
    }


  fn(lo,ro,proc);
}


void
operate_conditional(operand&  o1, operand&  o2, operand&  o3, process*  proc) noexcept
{
  auto  cond = o1.evaluate(proc).get_integer_safely();

  o1 = operand((cond? o2:o3).evaluate(proc));
}


void
operate_stack(operand_stack&  stack, const expr_element&  e, process*  proc) noexcept
{
    if(e.is_operand())
    {
      stack.push(e.get_operand());
    }

  else
    if(e.is_prefix_unary_operator())
    {
        if(stack.size() < 1)
        {
          printf("単項演算の演算項が足りない\n");

          return;
        }


      operate_prefix_unary(stack.top(),e.get_operator_word(),proc);
    }

  else
    if(e.is_postfix_unary_operator())
    {
        if(stack.size() < 1)
        {
          printf("単項演算の演算項が足りない\n");

          return;
        }


      operate_postfix_unary(stack.top(),e.get_operator_word(),proc);
    }

  else
    if(e.is_binary_operator())
    {
        if(stack.size() < 2)
        {
          printf("二項演算の演算項が足りない\n");

          return;
        }


      auto&  ro = stack.pop();
      auto&  lo = stack.top();

      operate_binary(lo,ro,e.get_operator_word(),proc);
    }
}




}}




