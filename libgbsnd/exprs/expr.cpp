#include"libgbsnd/expr.hpp"
#include"libgbsnd/script.hpp"
#include<new>


namespace gbsnd{
namespace exprs{




struct
expr::
data
{
  size_t  reference_count;

  size_t  number_of_elements;

  expr_element  elements[0];

};




expr&
expr::
operator=(const expr&  rhs) noexcept
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


expr&
expr::
operator=(expr&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}




void
expr::
assign(const expr_element*  e, size_t  n) noexcept
{
  unrefer();


  size_t  size = sizeof(data)+(sizeof(expr_element)*n);

  m_data = static_cast<data*>(malloc(size));

  m_data->reference_count    = 1;
  m_data->number_of_elements = n;

  expr_element*  dst = m_data->elements;

    while(n--)
    {
      new(dst++) expr_element(*e++);
    }


//printf("%p is allocated\n",m_data);
}


void
expr::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->reference_count)
        {
//printf("%p is deleted\n",m_data);

            for(auto&  e: *this)
            {
              gbstd::destruct(e);
            }


          free(m_data)         ;
               m_data = nullptr;
        }
    }
}


expr_element*  expr::begin() const noexcept{return m_data->elements;}
expr_element*    expr::end() const noexcept{return m_data->elements+m_data->number_of_elements;}




expr::
operator bool() const noexcept
{
  return m_data && m_data->number_of_elements;
}


size_t
expr::
size() const noexcept
{
  return m_data->number_of_elements;
}


value
expr::
evaluate(execution_context*  ctx) const noexcept
{
  operand_stack  stack;

  auto  it     = begin();
  auto  it_end =   end();

    while(it != it_end)
    {
      operate_stack(stack,*it++,ctx);
    }


  return stack.top().evaluate(ctx);
}


void
expr::
print() const noexcept
{
    if(!*this)
    {
//      printf("expr::print  無効なdata\n");

      return;
    }


  std::vector<operand>  buf;

    for(auto&  e: *this)
    {
        if(e.is_operand())
        {
          buf.emplace_back(e.get_operand());
        }

      else
        if(e.is_prefix_unary_operator())
        {
            if(buf.size() < 1)
            {
              printf("単項演算の演算項が足りない\n");

              return;
            }


          operation  op(prefix_unary_operator{e.get_operator_word()},operand(buf.back()));

          buf.back() = std::move(op);
        }

      else
        if(e.is_postfix_unary_operator())
        {
            if(buf.size() < 1)
            {
              printf("単項演算の演算項が足りない\n");

              return;
            }


          operation  op(postfix_unary_operator{e.get_operator_word()},operand(buf.back()));

          buf.back() = std::move(op);
        }

      else
        if(e.is_binary_operator())
        {
            if(buf.size() < 2)
            {
              printf("二項演算の演算項が足りない\n");

              return;
            }


          operand  op2(std::move(buf.back()));

          buf.pop_back();

          operand  op1(std::move(buf.back()));

          operation  op(binary_operator{e.get_operator_word()},std::move(op1),std::move(op2));

          buf.back() = std::move(op);
        }
    }


    if(buf.size() != 1)
    {
      printf("結果が不正\n");

      return;
    }


  buf.front().print();
}


}}




