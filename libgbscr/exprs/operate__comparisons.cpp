#include"libgbscr/exprs/operate.hpp"


namespace gbscr{
namespace exprs{
namespace comparisons{


void
eq(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() == rv.convert_to_integer());
}


void
neq(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() != rv.convert_to_integer());
}


void
lt(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() < rv.convert_to_integer());
}


void
lteq(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() <= rv.convert_to_integer());
}


void
gt(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() > rv.convert_to_integer());
}


void
gteq(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer() >= rv.convert_to_integer());
}


}}}




