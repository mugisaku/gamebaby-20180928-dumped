#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace arithmetics{


void
neg(operand&  o, process*  proc)
{
  auto  v = to_value(o,proc);

  o = value(-v.convert_to_integer());
}


void
add(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer()+rv.convert_to_integer());
}


void
sub(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()-rv.convert_to_integer());
}


void
mul(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer()*rv.convert_to_integer());
}


void
div(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  int  ri = rv.convert_to_integer();

    if(!ri)
    {
      throw division_by_zero{};
    }


  lo = value(lv.convert_to_integer()/ri);
}


void
rem(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  int  ri = rv.convert_to_integer();

    if(!ri)
    {
      throw division_by_zero{};
    }


  lo = value(lv.convert_to_integer()%ri);
}




}}}




