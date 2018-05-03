#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{


namespace{
const background_style
bgst(color(6,6,6));


}


widget*
create_radio_menu(wls  ls, rbcb cb, uint32_t  initial_state, void*  userdata) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new radio_button(*it++,cb);

      first_btn->set_style(bgst);

      first_btn->set_userdata(userdata);

      col->container::append_child(first_btn);

        while(it != end)
        {
          auto  btn = new radio_button(*it++,*first_btn);

          btn->set_style(bgst);

          btn->set_userdata(userdata);

          col->container::append_child(btn);
        }


      first_btn->set_state(initial_state);
    }


  return col;
}


widget*
create_check_menu(wls  ls, rbcb  cb, uint32_t  initial_state, void*  userdata) noexcept
{
  auto  col = new table_column;

  auto   it = ls.begin();
  auto  end = ls.end();

    if(it != end)
    {
      auto  first_btn = new check_button(*it++,cb);

      first_btn->set_style(bgst);

      first_btn->set_userdata(userdata);

      col->container::append_child(first_btn);

        while(it != end)
        {
          auto  btn = new check_button(*it++,*first_btn);

          btn->set_style(bgst);

          btn->set_userdata(userdata);

          col->container::append_child(btn);
        }


      first_btn->set_state(initial_state);
    }


  return col;
}




}}




