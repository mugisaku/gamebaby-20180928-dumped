#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace arithmetics{


void
neg(operand&  o, process*  proc)
{
  auto  v = o.evaluate(proc);

    if(v.is_reference())
    {
      v = v.get_reference()->get_value();
    }


  o = value(-v.convert_to_integer());
}


void
add(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer()+rv.convert_to_integer());
}


void
sub(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer()-rv.convert_to_integer());
}


void
mul(operand&  lo, operand&  ro, process*  proc)
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


  lo = value(lv.convert_to_integer()*rv.convert_to_integer());
}


void
div(operand&  lo, operand&  ro, process*  proc)
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


  int  ri = rv.convert_to_integer();

    if(!ri)
    {
      throw division_by_zero{};
    }


  lo = value(lv.convert_to_integer()%ri);
}




}}}




