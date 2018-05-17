#ifndef GMBB_task_HPP
#define GMBB_task_HPP


#include"libgbstd/image.hpp"
#include"libgbstd/string.hpp"
#include<list>


namespace gbstd{
namespace tasks{


class task_manager;


class
task
{
  bool  m_need_to_remove=false;

public:
  virtual ~task(){}

  virtual gbstd::string_view  get_task_name() const noexcept{return gbstd::string_view("task");}

  virtual void  do_when_remove() noexcept{delete this;}

  virtual void  update() noexcept{}

  void    need_to_remove() noexcept{m_need_to_remove =  true;}
  void  unneed_to_remove() noexcept{m_need_to_remove = false;}

  bool  is_needed_to_remove() const noexcept{return m_need_to_remove;}

};


class
task_manager
{
  struct node;

  node*  m_first=nullptr;
  node*  m_last =nullptr;

  node*  m_trash=nullptr;

  node*   get_node(         ) noexcept;
  void   dump_node(node*  nd) noexcept;

public:
 ~task_manager(){clear();}

  void  clear() noexcept;

  void  push_front(task*  t) noexcept;
  void   push_back(task*  t) noexcept;

  void  update() noexcept;

};


}


using tasks::task;
using tasks::task_manager;


}




#endif




