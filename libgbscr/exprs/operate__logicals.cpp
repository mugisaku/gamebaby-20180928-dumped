#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace logicals{


void
not_(operand&  o, process&  proc)
{
  auto  v = to_value(o,proc);

  o = value(!v.convert_to_integer());
}


void
or_( operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);

    if(lv.convert_to_integer())
    {
      lo = value(true);
    }


  auto  rv = to_value(ro,proc);

  lo = value(rv.convert_to_integer());
}


void
and_(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);

    if(!lv.convert_to_integer())
    {
      lo = value(false);
    }


  auto  rv = to_value(ro,proc);

  lo = value(rv.convert_to_integer());
}




}}}




