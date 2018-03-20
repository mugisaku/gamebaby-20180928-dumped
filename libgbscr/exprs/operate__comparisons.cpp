#include"libgbscr/exprs/operate.hpp"


namespace gbscr{
namespace exprs{
namespace comparisons{


void
eq(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() == rv.convert_to_integer());
}


void
neq(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() != rv.convert_to_integer());
}


void
lt(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() < rv.convert_to_integer());
}


void
lteq(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() <= rv.convert_to_integer());
}


void
gt(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() > rv.convert_to_integer());
}


void
gteq(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_value(lo,proc);
  auto  rv = to_value(ro,proc);

  lo = value(lv.convert_to_integer() >= rv.convert_to_integer());
}


}}}




