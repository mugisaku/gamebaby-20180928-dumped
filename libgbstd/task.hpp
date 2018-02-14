#ifndef GMBB_task_HPP
#define GMBB_task_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/rw_ptr.hpp"
#include"libgbstd/ro_ptr.hpp"
#include"libgbstd/string.hpp"


namespace gbstd{
namespace tasks{


class group_task;


class
task
{
  gbstd::string  m_name;

  point  m_base_point;

  rw_ptr<group_task>  m_group;

  rw_ptr<task>  m_previous;
  rw_ptr<task>      m_next;

public:
  task(         ) noexcept{}
  task(point  pt) noexcept: m_base_point(pt){}

  void                  set_name(gbstd::string_view  name)       noexcept{       m_name = name;}
  gbstd::string const&  get_name(                        ) const noexcept{return m_name       ;}

  void   set_base_point(point  pt)       noexcept{       m_base_point = pt;}
  point  get_base_point(         ) const noexcept{return m_base_point     ;}

  rw_ptr<task>  get_previous() const noexcept{return m_previous;}
  rw_ptr<task>  get_next()     const noexcept{return m_next;}

  ro_ptr<task>  get_ro_previous() const noexcept{return m_previous.get_ro();}
  ro_ptr<task>  get_ro_next()     const noexcept{return m_next.get_ro();}

  void                set_group(group_task&  grp)       noexcept{       m_group = &grp;}
  rw_ptr<group_task>  get_group(                ) const noexcept{return m_group       ;}

  void  connect(task&  new_next) noexcept;

  void  disconnect() noexcept;

  virtual bool  is_group() const noexcept{return false;}

  virtual void  update() noexcept{}

  virtual void  render(image&  dst, point  offset) const noexcept{}

};


class
group_task: public task
{
  rw_ptr<task>  m_first;
  rw_ptr<task>   m_last;

  uint32_t  m_number_of_tasks=0;

public:
  void  push_to_first(task&  t) noexcept;
  void   push_to_last(task&  t) noexcept;
  void   push(task&  t) noexcept{push_to_last(t);}

  void  erase(task&  t) noexcept;

  uint32_t  get_number_of_tasks() const noexcept{return m_number_of_tasks;}

  rw_ptr<task>  get_first() const noexcept{return m_first;}
  rw_ptr<task>   get_last() const noexcept{return  m_last;}

  ro_ptr<task>  get_ro_first() const noexcept{return m_first.get_ro();}
  ro_ptr<task>   get_ro_last() const noexcept{return  m_last.get_ro();}

  void  update() noexcept override;

  void  render(image&  dst, point  offset) const noexcept override;

  rw_ptr<task>  find_by_name(gbstd::string_view  name) const noexcept;

  bool  is_group() const noexcept override{return true;}

  void  print() noexcept;

};


}


using tasks::task;
using tasks::group_task;


}




#endif




