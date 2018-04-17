#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


widget*
create_radio_menu(std::initializer_list<widget*>  ls, uint32_t  initial_state) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new radio_button(*it++,nullptr);

      col->append(first_btn);

        while(it != end)
        {
          auto  btn = new radio_button(*it++,*first_btn);

          col->append(btn);
        }
    }


  return col;
}


widget*
create_check_menu(std::initializer_list<widget*>  ls, uint32_t  initial_state) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new check_button(*it++,nullptr);

      col->append(first_btn);

        while(it != end)
        {
          auto  btn = new check_button(*it++,*first_btn);

          col->append(btn);
        }
    }


  return col;
}




}}




