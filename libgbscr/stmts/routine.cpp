#include"libgbscr/object.hpp"
#include"libgbscr/stmt.hpp"


namespace gbscr{
namespace stmts{




routine::
routine(const token_string&  parals_src, const token_string&  blk_src) noexcept
{
  std::vector<gbstd::string>  buf;

  auto  it     = parals_src.begin();
  auto  it_end = parals_src.end();

    while(it != it_end)
    {
         if(it->is_identifier())
         {
           buf.emplace_back(it->get_string().view());

           ++it;

             if(it->is_operator_word())
             {
               ++it;
             }
         }

       else
         {
           printf("仮引数部分に不明な要素\n");

           break;
         }
    }


  m_parameter_list = parameter_list(buf.data(),buf.size());

  m_stmt_list = stmt_list(blk_src);
}




void
routine::
print() const noexcept
{
  printf("(");

    for(auto&  s: m_parameter_list)
    {
      printf("%s,",s.data());
    }


  printf(")");

  printf("\n{\n");

    for(auto&  stmt: m_stmt_list)
    {
      stmt.print();

      printf("\n");
    }


  printf("}\n\n\n");
}


}}




