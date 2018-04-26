#include"libgbstd/widget.hpp"



namespace gbstd{
namespace widgets{




struct
radio_button::
shared_data
{
  radio_button*  first;
  radio_button*   last;

  uint32_t  state;

  bool  touched;

  callback_prototype  callback;

};




radio_button::
radio_button(widget*  target, callback_prototype  cb) noexcept:
wrapper(new container),
m_data(new shared_data),
m_bit_id(1)
{
  m_icons = new icon_selector(get_icons());


  auto&  con = *static_cast<container*>(get_target());

  con.append_child(m_icons);
  con.append_child(target,icon::size,0);

  m_data->first    =  this;
  m_data->last     =  this;
  m_data->state    =     0;
  m_data->touched  = false;
  m_data->callback =    cb;
}


radio_button::
radio_button(widget*  target, radio_button&  first) noexcept:
wrapper(new container),
m_data(first.m_data),
m_bit_id(first.m_data->last->m_bit_id<<1)
{
  m_icons = new icon_selector(get_icons());


  auto&  con = *static_cast<container*>(get_target());

  con.append_child(m_icons);
  con.append_child(target,icon::size,0);

  m_data->last->m_next = this;
  m_data->last         = this;
}


radio_button::
~radio_button()
{
    if(m_data->first == this)
    {
      delete m_data;
    }
}




void
radio_button::
do_when_mouse_acted(int  x, int  y) noexcept
{
    if(m_data->touched)
    {
        if(!ctrl.is_mouse_lbutton_pressed())
        {
          m_data->touched = false;

          change_state();
        }
    }

  else
    {
        if(ctrl.is_mouse_lbutton_pressed())
        {
          m_data->touched = true;
        }
    }
}


const icon**
radio_button::
get_icons() const noexcept
{
  static const icon*
  icons[] =
  {
    &predefined_icon::radio_checked,
    &predefined_icon::radio_unchecked,
  };


  return icons;
}


bool
radio_button::
is_checked() const noexcept
{
  return m_data->state&m_bit_id;
}


uint32_t
radio_button::
get_state() const noexcept
{
  return m_data->state;
}


void
radio_button::
set_state(uint32_t  st) noexcept
{
    if(m_data->state != st)
    {
      auto  cur = m_data->first;

        while(cur)
        {
            if(cur->m_bit_id&st)
            {
              cur->m_icons->set_current(1);
            }

          else
            {
              cur->m_icons->set_current(0);
            }


          cur->need_to_redraw();

          cur = cur->m_next;
        }


      call(st);
    }
}




void
radio_button::
change_state() noexcept
{
    if(is_checked())
    {
      return;
    }



  auto  cur = m_data->first;

    while(cur)
    {
        if(cur->is_checked())
        {
          cur->m_icons->set_current(0);

          cur->need_to_redraw();
        }


      cur = cur->m_next;
    }


  m_icons->set_current(1);

  need_to_redraw();

  call(m_bit_id);
}


void
radio_button::
call(uint32_t  new_state) noexcept
{
  uint32_t  old_state = m_data->state            ;
                        m_data->state = new_state;

  auto  cb = m_data->callback;

    if(cb)
    {
      cb(*this,old_state,new_state);
    }
}




}}




