#include"libgbscr/exprs/operate.hpp"


namespace gbscr{
namespace exprs{
namespace bitwises{


void
not_(operand&  o, process*  proc)
{
  auto  v = o.evaluate(proc);

    if(v.is_reference())
    {
      v = v.get_reference()->get_value();
    }


  o = value(~v.convert_to_integer());
}


void
and_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(lv.is_reference())
    {
      lv = lv.get_reference()->get_value();
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lo = value(lv.convert_to_integer()&rv.convert_to_integer());
}


void
or_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(lv.is_reference())
    {
      lv = lv.get_reference()->get_value();
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lo = value(lv.convert_to_integer()|rv.convert_to_integer());
}


void
xor_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(lv.is_reference())
    {
      lv = lv.get_reference()->get_value();
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lo = value(lv.convert_to_integer()^rv.convert_to_integer());
}


void
shl(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(lv.is_reference())
    {
      lv = lv.get_reference()->get_value();
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lo = value(lv.convert_to_integer()<<rv.convert_to_integer());
}


void
shr(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(lv.is_reference())
    {
      lv = lv.get_reference()->get_value();
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lo = value(lv.convert_to_integer()>>rv.convert_to_integer());
}


}}}




