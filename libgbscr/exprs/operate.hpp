#ifndef operate_hpp_was_included
#define operate_hpp_was_included


#include"libgbscr/expr.hpp"
#include"libgbscr/process.hpp"


namespace gbscr{
namespace exprs{




namespace assignments{
void   st(operand&  lo, operand&  ro, process*  proc);
void  add(operand&  lo, operand&  ro, process*  proc);
void  sub(operand&  lo, operand&  ro, process*  proc);
void  mul(operand&  lo, operand&  ro, process*  proc);
void  div(operand&  lo, operand&  ro, process*  proc);
void  rem(operand&  lo, operand&  ro, process*  proc);
void  and_(operand&  lo, operand&  ro, process*  proc);
void  or_( operand&  lo, operand&  ro, process*  proc);
void  xor_(operand&  lo, operand&  ro, process*  proc);
void  shl( operand&  lo, operand&  ro, process*  proc);
void  shr( operand&  lo, operand&  ro, process*  proc);
}
namespace arithmetics{
void  neg(operand&  o, process*  proc);
void  add(operand&  lo, operand&  ro, process*  proc);
void  sub(operand&  lo, operand&  ro, process*  proc);
void  mul(operand&  lo, operand&  ro, process*  proc);
void  div(operand&  lo, operand&  ro, process*  proc);
void  rem(operand&  lo, operand&  ro, process*  proc);
}
namespace comparisons{
void  eq(  operand&  lo, operand&  ro, process*  proc);
void  neq( operand&  lo, operand&  ro, process*  proc);
void  lt(  operand&  lo, operand&  ro, process*  proc);
void  lteq(operand&  lo, operand&  ro, process*  proc);
void  gt(  operand&  lo, operand&  ro, process*  proc);
void  gteq(operand&  lo, operand&  ro, process*  proc);
}
namespace bitwises{
void  not_(operand&  o, process*  proc);
void  and_(operand&  lo, operand&  ro, process*  proc);
void  or_( operand&  lo, operand&  ro, process*  proc);
void  xor_(operand&  lo, operand&  ro, process*  proc);
void  shl( operand&  lo, operand&  ro, process*  proc);
void  shr( operand&  lo, operand&  ro, process*  proc);
}
namespace logicals{
void  not_(operand&  o, process*  proc);
void  or_( operand&  lo, operand&  ro, process*  proc);
void  and_(operand&  lo, operand&  ro, process*  proc);
}


}}


#endif




