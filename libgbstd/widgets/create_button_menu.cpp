#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


widget*
create_radio_menu(std::initializer_list<widget*>  ls, radio_button::callback_prototype  cb, uint32_t  initial_state) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new radio_button(*it++,cb);

      col->container::append_child(first_btn);

        while(it != end)
        {
          auto  btn = new radio_button(*it++,*first_btn);

          col->container::append_child(btn);
        }


      first_btn->set_state(initial_state);
    }


  return col;
}


widget*
create_check_menu(std::initializer_list<widget*>  ls, radio_button::callback_prototype  cb, uint32_t  initial_state) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new check_button(*it++,cb);

      col->container::append_child(first_btn);

        while(it != end)
        {
          auto  btn = new check_button(*it++,*first_btn);

          col->container::append_child(btn);
        }


      first_btn->set_state(initial_state);
    }


  return col;
}




}}




