#include"libgbscr/stmt.hpp"


namespace gbscr{
namespace stmts{




routine::
routine(const block&  parals_src, const block&  impl_src, table&  tbl) noexcept
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

             if(it->is_punctuations(","))
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

  m_stmt_list = stmt_list(impl_src,tbl);
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




