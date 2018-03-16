#include"libgbscr/exprs/operate.hpp"




namespace gbscr{
namespace exprs{
namespace accesses{


void
call(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_reference(lo,proc);
  auto  rv =     to_value(ro,proc);

//  lo = value(lv.convert_to_integer()*rv.convert_to_integer());
}


void
member_access(operand&  lo, operand&  ro, process*  proc)
{
    if(!ro.is_identifier())
    {
      printf("member access error: 識別子がない\n");

      throw operation_error{};
    }


  auto  lv = to_value(lo,proc);

    if(!lv.is_table())
    {
      printf("member access error: テーブルではない値に、メンバーアクセスしようとした\n");

      throw operation_error{};
    }


  lo = value(lv.get_table()[ro.get_string().view()]);
}


void
subscript(operand&  lo, operand&  ro, process*  proc)
{
  auto  lv = to_reference(lo,proc);
  auto  rv =     to_value(ro,proc);

//  lo = value(lv.convert_to_integer()*rv.convert_to_integer());
}




}}}




