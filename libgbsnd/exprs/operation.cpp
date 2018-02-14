#include"libgbsnd/expr.hpp"
#include"libgbsnd/script.hpp"
#include<new>


namespace gbsnd{
namespace exprs{




enum class
operation_kind
{
  prefix_unary,
  postfix_unary,
  binary,
  conditional,
};


struct
operation::
data
{
  size_t  reference_count=1;

  operation_kind  kind;

  operator_word  word;

  operand   first_operand;
  operand  second_operand;
  operand   third_operand;

};




operation::
operation(prefix_unary_operator  op, operand&&  o) noexcept:
m_data(new data)
{
  m_data->kind = operation_kind::prefix_unary;

  m_data->word = op.word;

  m_data->first_operand = std::move(o);
}


operation::
operation(postfix_unary_operator  op, operand&&  o) noexcept:
m_data(new data)
{
  m_data->kind = operation_kind::postfix_unary;

  m_data->word = op.word;

  m_data->first_operand = std::move(o);
}


operation::
operation(binary_operator  op, operand&&  o1, operand&&  o2) noexcept:
m_data(new data)
{
  m_data->kind = operation_kind::binary;

  m_data->word = op.word;

  m_data->first_operand  = std::move(o1);
  m_data->second_operand = std::move(o2);
}


operation::
operation(operand&&  o1, operand&&  o2, operand&&  o3) noexcept:
m_data(new data)
{
  m_data->kind = operation_kind::conditional;

  m_data->first_operand  = std::move(o1);
  m_data->second_operand = std::move(o2);
  m_data->third_operand  = std::move(o3);
}




operation&
operation::
operator=(const operation&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->reference_count;
        }
    }


  return *this;
}


operation&
operation::
operator=(operation&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




void
operation::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
          delete m_data          ;
                 m_data = nullptr;
        }
    }
}




bool  operation::is_prefix_unary() const noexcept{return m_data->kind == operation_kind::prefix_unary;}
bool  operation::is_postfix_unary() const noexcept{return m_data->kind == operation_kind::postfix_unary;}
bool  operation::is_binary() const noexcept{return m_data->kind == operation_kind::binary;}
bool  operation::is_conditional() const noexcept{return m_data->kind == operation_kind::conditional;}




value
operation::
evaluate(execution_context*  ctx) const noexcept
{
  auto&  lo =  m_data->first_operand;
  auto&  ro = m_data->second_operand;

    switch(m_data->kind)
    {
  case(operation_kind::prefix_unary):
      operate_prefix_unary(lo,m_data->word,ctx);
      break;
  case(operation_kind::postfix_unary):
      operate_postfix_unary(lo,m_data->word,ctx);
      break;
  case(operation_kind::binary):
      operate_binary(lo,ro,m_data->word,ctx);
      break;
  case(operation_kind::conditional):
      operate_conditional(m_data->first_operand,
                          m_data->second_operand,
                          m_data->third_operand,ctx);
      break;
    }


  return lo.evaluate(ctx);
}


void
operation::
print() const noexcept
{
  short_string  ss(m_data->word);

    switch(m_data->kind)
    {
  case(operation_kind::prefix_unary):
      printf("%s",ss.data());
      m_data->first_operand.print();
      break;
  case(operation_kind::postfix_unary):
      m_data->first_operand.print();
      printf("%s",ss.data());
      break;
  case(operation_kind::binary):
        if(m_data->word == operator_word("()"))
        {
          m_data->first_operand.print();
          printf("(");
          m_data->second_operand.print();
          printf(")");
        }

      else
        if(m_data->word == operator_word("[]"))
        {
          m_data->first_operand.print();
          printf("[");
          m_data->second_operand.print();
          printf("]");
        }

      else
        {
          printf("(");
          m_data->first_operand.print();
          printf("%s",ss.data());
          m_data->second_operand.print();
          printf(")");
        }
      break;
  case(operation_kind::conditional):
      m_data->first_operand.print();
      printf("? ");
      m_data->second_operand.print();
      printf(":");
      m_data->third_operand.print();
      break;
    }
}


}}




