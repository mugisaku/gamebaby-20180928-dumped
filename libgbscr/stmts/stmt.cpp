#include"libgbsnd/stmt.hpp"


namespace gbsnd{
namespace stmts{




void
stmt::
print() const noexcept
{
    switch(m_kind)
    {
  case(stmt_kind::evaluate_and_dump):
      printf("evaluate and dump ");
      m_expr.print();
      break;
  case(stmt_kind::evaluate_and_save):
      printf("evaluate and save ");
      m_expr.print();
      break;
  case(stmt_kind::evaluate_and_zero):
      printf("evaluate and zero ");
      m_expr.print();
      break;
  case(stmt_kind::evaluate_and_not_zero):
      printf("evaluate and not zero ");
      m_expr.print();
      break;
  case(stmt_kind::evaluate_and_equal):
      printf("evaluate and equal ");
      m_expr.print();
      break;
  case(stmt_kind::evaluate_and_not_equal):
      printf("evaluate and not equal ");
      m_expr.print();
      break;
  case(stmt_kind::label):
      printf("label %s",m_label.data());
      break;
  case(stmt_kind::return_):
      printf("return ");
      m_expr.print();
      break;
  case(stmt_kind::print):
      printf("print ");
      m_expr.print();
      break;
  case(stmt_kind::sleep):
      printf("sleep ");
      m_expr.print();
      break;
  case(stmt_kind::exit):
      printf("exit ");
      m_expr.print();
      break;
  case(stmt_kind::jump_by_condition):
      printf("jump by condition");
      printf(", to %s",m_label.data());
      break;
  case(stmt_kind::jump):
      printf("jump to %s",m_label.data());
      break;
    }


  printf(";");
}


}}




