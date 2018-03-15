#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace assignments{


void
st(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  auto  rv = ro.evaluate(proc);

    if(rv.is_reference())
    {
      rv = rv.get_reference()->get_value();
    }


  lv.get_reference()->get_value() = rv;
}


void
add(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  arithmetics::add(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
sub(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  arithmetics::sub(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
mul(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  arithmetics::mul(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
div(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  arithmetics::div(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
rem(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  arithmetics::rem(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
and_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  bitwises::and_(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
or_( operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  bitwises::or_(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
xor_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  bitwises::xor_(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
shl( operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  bitwises::shl(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}


void
shr( operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = lo.evaluate(proc);

    if(!lv.is_reference())
    {
      throw assignment_error{};
    }


  bitwises::shr(lo,ro,proc);

  lv.get_reference()->get_value() = lo.get_value();

  lo = value(lv);
}




}}}




