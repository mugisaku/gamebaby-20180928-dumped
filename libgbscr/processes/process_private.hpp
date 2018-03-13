#ifndef LIBGBscr_process_private_HPP
#define LIBGBscr_process_private_HPP


#include"libgbscr/process.hpp"
#include"libgbscr/expr.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbscr/stream.hpp"


namespace gbscr{
namespace processes{


struct
calling_preparation
{
  calling_preparation*  previous;

  const stmts::routine*  routine;

  exprs::operand_stack  operand_stack;

  exprs::expr_list  expr_list;

  const expr*  expr_it;
  const expr*  expr_it_end;

  const expr_element*  eval_it;
  const expr_element*  eval_it_end;

  value*  return_value;

};


struct
process::
frame
{
  frame*  previous;

  calling_preparation*  calling;

  gbstd::string  routine_name;

  const stmt*    begin;
  const stmt*  current;
  const stmt*      end;

  values::table  table;

  int  saved_value;

  bool  condition;

  exprs::operand_stack  operand_stack;

  const expr_element*  eval_it;
  const expr_element*  eval_it_end;

  value*  return_value;

  void  jump(gbstd::string_view  label) noexcept
  {
    auto  it = begin;

      while(it != end)
      {
          if(it->is_label() && (it->get_label() == label))
          {
            current = it;

            return;
          }


        ++it;
      }


    printf("jump error: %sというラベルが見つからない\n",label.data());
  }

};




}}




#endif




