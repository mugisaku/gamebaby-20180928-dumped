#include"libgbscr/expr.hpp"


namespace gbscr{
namespace exprs{




void
variable::
print() const noexcept
{
  printf("%s:",m_name.data());

  m_operand.print();
}




bool
read_variable(cursor&  cur, variable*&  var, process&  proc)
{
    if(!cur->is_identifier() &&
       !cur->is_string())
    {
      printf("テーブルの要素は、識別子がなくてはならない\n");

      return false;
    }


  auto  name = cur++->get_string().view();

    if(!cur->is_punctuations(":"))
    {
      printf("テーブルの要素は、識別子の後の値は\':\'で区切られなくてはならない\n");

      return false;
    }


  ++cur;

  operand  o;

    if(read_operand(cur,o,proc))
    {
      var = variable::create_instance(o.evaluate(proc),name);

      return true;
    }


  return false;
}




}}




