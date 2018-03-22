#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace accesses{


void
call(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_reference(lo,proc)->get_operand().evaluate(proc);

    if(!lv.is_routine())
    {
      printf("call error: 関数ではない\n");

      throw operation_error{};
    }


    if(!ro.is_expression_list())
    {
      printf("call error: 引数リストではない\n");

      throw operation_error{};
    }


  auto&  r = lv.get_routine();

  lo = value();

  proc.prepare_call(r,ro.get_expression_list(),lo.get_value_pointer());
}


void
member_access(operand&  lo, operand&  ro, process&  proc)
{
    if(!ro.is_identifier())
    {
      printf("member access error: 識別子がない\n");

      throw operation_error{};
    }


  auto  lv = to_reference(lo,proc)->get_operand().evaluate(proc);

    if(!lv.is_table())
    {
      printf("member access error: テーブルではない値に、メンバーアクセスしようとした\n");

      throw operation_error{};
    }


  lo = value(lv.get_table()[ro.get_string().view()]);
}


void
subscript(operand&  lo, operand&  ro, process&  proc)
{
  auto  lv = to_reference(lo,proc)->get_operand().evaluate(proc);
  auto  rv =     to_value(ro,proc);

    if(!rv.is_integer() ||
       !rv.is_string())
    {
      printf("subscript error: keyがない\n");

      throw operation_error{};
    }
}




}}}




