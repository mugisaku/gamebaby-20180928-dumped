#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace assignments{




void
st(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);
  auto  rv    = to_value(    ro,proc);

  lvref->get_value() = rv;
}


void
add(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  arithmetics::add(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
sub(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  arithmetics::sub(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
mul(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  arithmetics::mul(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
div(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  arithmetics::div(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
rem(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  arithmetics::rem(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
and_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  bitwises::and_(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
or_( operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  bitwises::or_(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
xor_(operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  bitwises::xor_(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
shl( operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  bitwises::shl(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}


void
shr( operand&  lo, operand&  ro, process*  proc)
{
  auto  lvref = to_reference(lo,proc);


  bitwises::shr(lo,ro,proc);

  lvref->get_value() = lo.get_value();

  lo = value(lvref);
}




}}}




