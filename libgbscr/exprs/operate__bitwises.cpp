#include"libgbscr/exprs/operate.hpp"


namespace gbscr{
namespace exprs{
namespace bitwises{


void
not_(operand&  o, process&  proc)
{
  auto  v = to_value(o,proc);

  o = value(~v.convert_to_integer());
}


void
and_(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()&rv.convert_to_integer());
}


void
or_(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()|rv.convert_to_integer());
}


void
xor_(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()^rv.convert_to_integer());
}


void
shl(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()<<rv.convert_to_integer());
}


void
shr(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);


  lo = value(lv.convert_to_integer()>>rv.convert_to_integer());
}


}}}




