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


void
radio_button::
common_callback(wrapper&  wr, event_kind  k, int  x, int  y) noexcept
{
  auto&  radbtn = *reinterpret_cast<radio_button*>(&wr);

    if(k == event_kind::mouse_acted)
    {
      auto&  dat = *radbtn.m_data;

        if(dat.touched)
        {
            if(!ctrl.is_mouse_lbutton_pressed())
            {
              dat.touched = false;

              radbtn.change_state();
            }
        }

      else
        {
            if(ctrl.is_mouse_lbutton_pressed())
            {
              dat.touched = true;
            }
        }
    }
}




radio_button::
radio_button(widget*  target, callback_prototype  cb) noexcept:
wrapper(target,common_callback),
m_data(new shared_data),
m_bit_id(1)
{
  m_data->first    =  this;
  m_data->last     =  this;
  m_data->state    =     0;
  m_data->touched  = false;
  m_data->callback =    cb;
}


radio_button::
radio_button(widget*  target, radio_button&  first) noexcept:
wrapper(target,common_callback),
m_data(first.m_data),
m_bit_id(first.m_data->last->m_bit_id<<1)
{
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
change_state() noexcept
{
    if(is_checked())
    {
      return;
    }


  auto  cur = m_data->first;

    while(cur)
    {
        if(cur->m_bit_id&m_data->state)
        {
          cur->need_to_redraw();
        }


      cur = cur->m_next;
    }


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


void
radio_button::
render(image_cursor  cur) noexcept
{
  wrapper::render(cur);

  cur.draw_rectangle(is_checked()? predefined::red:predefined::yellow,0,0,get_width(),get_height());
}




}}




