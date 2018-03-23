#include"libgbscr/value.hpp"
#include"libgbscr/stmt.hpp"
#include"libgbstd/utility.hpp"
#include<new>


namespace gbscr{
namespace values{




class
value::
private_data
{
public:
  size_t  m_count=1;

  enum class kind{
    null,
    boolean,
    integer,
    string,
    reference,
    routine,
    table,

  } m_kind=kind::null;


  union data{
    bool             b;
    int              i;
    table          tbl;
    reference      ref;

    gbstd::string   s;
    stmts::routine  r;

    data(){}
   ~data(){}

  } m_data;


  void  clear() noexcept
  {
      switch(m_kind)
      {
    case(kind::string):
        gbstd::destruct(m_data.s);
        break;
    case(kind::reference):
        gbstd::destruct(m_data.ref);
        break;
    case(kind::routine):
        gbstd::destruct(m_data.r);
        break;
    case(kind::table):
        gbstd::destruct(m_data.tbl);
        break;
      }


    m_kind = kind::null;
  }

};


value::private_data*
value::
pop() noexcept
{
  auto  ptr =  new private_data;

  ptr->m_count = 1;

  return ptr;
}


void
value::
push(private_data*  data) noexcept
{
  data->clear();

  delete data;
}




value::
value() noexcept:
m_data(pop())
{
}




value&
value::
operator=(bool  b) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::boolean;

  m_data->m_data.b = b;

  return *this;
}


value&
value::
operator=(int  i) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::integer;

  m_data->m_data.i = i;

  return *this;
}


value&
value::
operator=(gbstd::string&&  s) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::string;

  new(&m_data->m_data) gbstd::string(std::move(s));

  return *this;
}


value&
value::
operator=(const reference&  r) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::reference;

  new(&m_data->m_data) reference(r);

  return *this;
}


value&
value::
operator=(stmts::routine&&  rt) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::routine;

  new(&m_data->m_data) stmts::routine(std::move(rt));

  return *this;
}


value&
value::
operator=(table&&  tbl) noexcept
{
  unrefer_if_not_unique();

  m_data->m_kind = private_data::kind::table;

  new(&m_data->m_data) table(std::move(tbl));

  return *this;
}


value&
value::
operator=(const value&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      m_data = rhs.m_data;

        if(m_data)
        {
          ++m_data->m_count;
        }
    }


  return *this;
}


value&
value::
operator=(value&&  rhs) noexcept
{
    if(this != &rhs)
    {
      unrefer();

      std::swap(m_data,rhs.m_data);
    }


  return *this;
}


value::
operator bool() const noexcept
{
  return m_data->m_kind != private_data::kind::null;
}




void
value::
unrefer() noexcept
{
    if(m_data)
    {
        if(!--m_data->m_count)
        {
          push(m_data);
        }


      m_data = nullptr;
    }
}


void
value::
unrefer_if_not_unique() noexcept
{
    if(m_data)
    {
        if(m_data->m_count == 1)
        {
          m_data->clear();
        }

      else
        {
          --m_data->m_count;

          m_data = pop();
        }
    }

  else
    {
      m_data = pop();
    }
}



bool  value::is_null()      const noexcept{return m_data->m_kind == private_data::kind::null;}
bool  value::is_boolean()   const noexcept{return m_data->m_kind == private_data::kind::boolean;}
bool  value::is_reference() const noexcept{return m_data->m_kind == private_data::kind::reference;}
bool  value::is_integer()   const noexcept{return m_data->m_kind == private_data::kind::integer;}
bool  value::is_string()    const noexcept{return m_data->m_kind == private_data::kind::string;}
bool  value::is_routine()   const noexcept{return m_data->m_kind == private_data::kind::routine;}
bool  value::is_table()     const noexcept{return m_data->m_kind == private_data::kind::table;}

bool                   value::get_boolean()   const noexcept{return m_data->m_data.b;}
int                    value::get_integer()   const noexcept{return m_data->m_data.i;}
const gbstd::string&   value::get_string()    const noexcept{return m_data->m_data.s;}
const reference&       value::get_reference() const noexcept{return m_data->m_data.ref;}
const stmts::routine&  value::get_routine()   const noexcept{return m_data->m_data.r;}
const table&           value::get_table()     const noexcept{return m_data->m_data.tbl;}


void
value::
print() const noexcept
{
    switch(m_data->m_kind)
    {
  case(private_data::kind::null):
      printf("null");
      break;
  case(private_data::kind::boolean):
      printf("%s",m_data->m_data.b? "true":"false");
      break;
  case(private_data::kind::integer):
      printf("%d",m_data->m_data.i);
      break;
  case(private_data::kind::string):
      printf("\"%s\"",m_data->m_data.s.data());
      break;
  case(private_data::kind::reference):
      printf("reference ");

      m_data->m_data.ref->print();
      break;
  case(private_data::kind::routine):
      printf("routine");
      m_data->m_data.r.print();
      break;
  case(private_data::kind::table):
      printf("table");
      m_data->m_data.tbl.print();
      break;
  default:
      printf("unknown value kind\n");
      break;
    }
}


}}




