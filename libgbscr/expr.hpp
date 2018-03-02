#ifndef LIBGBSND_EXPR_HPP
#define LIBGBSND_EXPR_HPP


#include"libgbsnd/object.hpp"
#include"libgbstd/utility.hpp"
#include"libgbsnd/short_string.hpp"
#include"libgbsnd/list.hpp"


namespace gbsnd{


class execution_context;


namespace scripts{
class script_token_string;
class script_token_cursor;
}

namespace exprs{


class operand;
class expr_element;
class expr;
class operand_stack;




struct
prefix_unary_operator
{
  operator_word  word;

};


struct
postfix_unary_operator
{
  operator_word  word;

};


struct
binary_operator
{
  operator_word  word;

};


class
operation
{
  struct data;

  data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  operation(prefix_unary_operator  op, operand&&  o) noexcept;
  operation(postfix_unary_operator  op, operand&&  o) noexcept;
  operation(binary_operator  op, operand&&  o1, operand&&  o2) noexcept;
  operation(operand&&  o1, operand&&  o2, operand&&  o3) noexcept;
  operation(const operation&   rhs) noexcept{*this = rhs;}
  operation(      operation&&  rhs) noexcept{*this = std::move(rhs);}
 ~operation(){unrefer();}

  operation&  operator=(const operation&   rhs) noexcept;
  operation&  operator=(      operation&&  rhs) noexcept;

  bool  is_prefix_unary() const noexcept;
  bool  is_postfix_unary() const noexcept;
  bool  is_binary() const noexcept;
  bool  is_conditional() const noexcept;

  value  evaluate(execution_context*  ctx) const noexcept;

  void  print() const noexcept;

};


void  operate_prefix_unary( operand&  o, operator_word  opw, execution_context*  ctx) noexcept;
void  operate_postfix_unary(operand&  o, operator_word  opw, execution_context*  ctx) noexcept;
void  operate_binary(       operand&  lo, operand&  ro, operator_word  opw, execution_context*  ctx) noexcept;
void  operate_conditional(operand&  o1, operand&  o2, operand&  o3, execution_context*  ctx) noexcept;
void  operate_stack(operand_stack&  stack, const expr_element&  e, execution_context*  ctx) noexcept;


class
expr
{
  struct data;

  data*  m_data=nullptr;

  void  unrefer() noexcept;

public:
  expr() noexcept{}
  expr(const expr_element*  e, size_t  n) noexcept{assign(e,n);}
  expr(gbstd::string_view  sv) noexcept;
  expr(const expr&   rhs) noexcept{*this = rhs;}
  expr(      expr&&  rhs) noexcept{*this = std::move(rhs);}
 ~expr(){unrefer();}

  expr&  operator=(const expr&   rhs) noexcept;
  expr&  operator=(      expr&&  rhs) noexcept;

  operator bool() const noexcept;

  void  assign(const expr_element*  e, size_t  n) noexcept;

  size_t  size() const noexcept;

  value  evaluate(execution_context*  ctx) const noexcept;

  expr_element*  begin() const noexcept;
  expr_element*    end() const noexcept;

  void  print() const noexcept;

};


class
expr_list: public list<expr>
{
public:
  using list::list;
};


expr       make_expr(gbstd::string_view  sv) noexcept;
expr       make_expr(     scripts::script_token_cursor&  cur) noexcept;
expr_list  make_expr_list(scripts::script_token_cursor&  cur) noexcept;


class
paired_expr
{
  expr  m_left;
  expr  m_right;

public:
  paired_expr(const expr&  l, const expr&  r) noexcept:
  m_left(l), m_right(r){}

  const expr&  get_left()  const noexcept{return m_left;}
  const expr&  get_right() const noexcept{return m_right;}

};


class
operand
{
  enum class kind{
    null,
    boolean_literal,
    integer_literal,
    identifier,
    expression,
    expression_list,
    operation,

    paired_expression,

    value,

  } m_kind=kind::null; 

  union data{
    bool  b;

    uint64_t  i;

    identifier  id;

    expr       e;
    expr_list  els;

    paired_expr  pe;

    operation  op;

    value  v;

    data(){}
   ~data(){}

  } m_data;

public:
  operand() noexcept{}
  operand(bool  b) noexcept{*this = b;}
  operand(uint64_t  i) noexcept{*this = i;}
  operand(const identifier&  id) noexcept{*this = std::move(id);}
  operand(const expr&  e) noexcept{*this = e;}
  operand(expr_list&&  els) noexcept{*this = std::move(els);}
  operand(paired_expr&&  pe) noexcept{*this = std::move(pe);}
  operand(const operation&  op) noexcept{*this = op;}
  operand(const value&  v) noexcept{*this = v;}
  operand(const operand&   rhs) noexcept{*this = rhs;}
  operand(      operand&&  rhs) noexcept{*this = std::move(rhs);}
 ~operand(){clear();}

  operand&  operator=(bool  b) noexcept;
  operand&  operator=(uint64_t  i) noexcept;
  operand&  operator=(const identifier&  id) noexcept;
  operand&  operator=(const expr&  e) noexcept;
  operand&  operator=(expr_list&&  els) noexcept;
  operand&  operator=(paired_expr&&  pe) noexcept;
  operand&  operator=(const operation&  op) noexcept;
  operand&  operator=(const value&  v) noexcept;
  operand&  operator=(const operand&   rhs) noexcept;
  operand&  operator=(      operand&&  rhs) noexcept;


  void  clear() noexcept;

  bool  is_boolean_literal()   const noexcept{return m_kind == kind::boolean_literal;}
  bool  is_integer_literal()   const noexcept{return m_kind == kind::integer_literal;}
  bool  is_identifier()        const noexcept{return m_kind == kind::identifier;}
  bool  is_expression()        const noexcept{return m_kind == kind::expression;}
  bool  is_expression_list()   const noexcept{return m_kind == kind::expression_list;}
  bool  is_paired_expression() const noexcept{return m_kind == kind::paired_expression;}
  bool  is_operation()         const noexcept{return m_kind == kind::operation;}
  bool  is_value()             const noexcept{return m_kind == kind::value;}

  bool                get_boolean_literal()   const noexcept{return m_data.b;}
  uint64_t            get_integer_literal()   const noexcept{return m_data.i;}
  const identifier&   get_identifier()        const noexcept{return m_data.id;}
  const expr&         get_expression()        const noexcept{return m_data.e;}
  const expr_list&    get_expression_list()   const noexcept{return m_data.els;}
  const paired_expr&  get_paired_expression() const noexcept{return m_data.pe;}
  const operation&    get_operation()         const noexcept{return m_data.op;}
  const value&        get_value()             const noexcept{return m_data.v;}

  value*  get_value_pointer() noexcept{return &m_data.v;}

  value  evaluate(execution_context*  ctx) const noexcept;

  void  print() const noexcept;

};


class
operand_stack
{
  static constexpr size_t  max_length = 32;

  operand  m_operands[max_length];

  size_t  m_length=0;

public:
  void  push(const operand&  o) noexcept{m_operands[m_length++] = o;}

  operand&  pop() noexcept{return m_operands[--m_length];}

        operand&  top()       noexcept{return m_operands[m_length-1];}
  const operand&  top() const noexcept{return m_operands[m_length-1];}

  void  reset() noexcept{m_length = 0;}

  size_t  size() const noexcept{return m_length;}

  const operand*  begin() const noexcept{return m_operands         ;}
  const operand*    end() const noexcept{return m_operands+m_length;}

};


class
expr_element
{
  enum class kind
  {
    null,
    operand,
    prefix_unary_operator,
    postfix_unary_operator,
    binary_operator,

  } m_kind=kind::null;

  union data{
    operand          o;
    operator_word  opw;

    data(){}
   ~data(){}

  } m_data;

public:
  expr_element() noexcept{}
  expr_element(prefix_unary_operator  op) noexcept: m_kind(kind::prefix_unary_operator){m_data.opw = op.word;}
  expr_element(postfix_unary_operator  op) noexcept: m_kind(kind::postfix_unary_operator){m_data.opw = op.word;}
  expr_element(binary_operator  op) noexcept: m_kind(kind::binary_operator){m_data.opw = op.word;}
  expr_element(operand&&  o) noexcept: m_kind(kind::operand){new(&m_data) operand(std::move(o));}
  expr_element(const expr_element&   rhs) noexcept{*this = rhs;}
  expr_element(      expr_element&&  rhs) noexcept{*this = std::move(rhs);}
 ~expr_element(){clear();}

  expr_element&  operator=(const expr_element&   rhs) noexcept;
  expr_element&  operator=(      expr_element&&  rhs) noexcept;

  void  clear() noexcept;

  bool  is_prefix_unary_operator() const noexcept{return m_kind == kind::prefix_unary_operator;}
  bool  is_postfix_unary_operator() const noexcept{return m_kind == kind::postfix_unary_operator;}
  bool  is_binary_operator() const noexcept{return m_kind == kind::binary_operator;}
  bool  is_operand()  const noexcept{return m_kind == kind::operand;}

  operator_word  get_operator_word() const noexcept{return m_data.opw;}
  const operand&       get_operand() const noexcept{return m_data.o;}

};


}


using exprs::expr_element;
using exprs::expr;
using exprs::expr_list;
using exprs::paired_expr;
using exprs::operation;
using exprs::operand;
using exprs::operand_stack;
using exprs::make_expr;
using exprs::make_expr_list;
using exprs::operate_prefix_unary;
using exprs::operate_postfix_unary;
using exprs::operate_binary;
using exprs::operate_conditional;
using exprs::operate_stack;


}




#endif




