#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace others{


void
seq_eval( operand&  lo, operand&  ro, process*  proc)
{
}


void
cond_eval( operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_value(lo,proc);

  auto  rv = ro.evaluate(proc);

    if(!rv.is_paired_expression())
    {
      throw operation_error{};
    }


  auto&  paired = rv.get_paired_expression();

  auto  v = (lv.convert_to_integer()? paired.get_left()
                                    : paired.get_right()).evaluate(proc);

  lo = v;
}


}}}




