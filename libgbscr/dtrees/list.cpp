#include"libgbscr/dtree.hpp"
#include<cstring>
#include<vector>




namespace  gbscr{
namespace dtrees{


value
list::
read_number(tok::stream_reader&  r)
{
  bool  neg = false;

    if(r.get_char() == '-')
    {
      neg = true;

      r.advance();

      r.skip_spaces();

        if(!r.is_pointing_number())
        {
          printf("-の後ろに数列がない\n");

          throw r.get_context();
        }
    }


  auto  n = static_cast<int>(r.read_number());

  return value(neg? -n:n);
}


value*
list::
read_value_of_pair(tok::stream_reader&  r, char  cl)
{
  r.skip_spaces();

    if(r.get_char() == ':')
    {
      r.advance();

      r.skip_spaces();

      auto  v = read_value(r,cl);

        if(!v)
        {
          printf(":の後ろに値がない\n");

          throw r.get_context();
        }


      return new value(std::move(v));
    }


  return nullptr;
}


value
list::
read_value(tok::stream_reader&  r, char  cl)
{
RESTART:
  r.skip_spaces();

  auto  c = r.get_char();

    if(c == cl)
    {
      r.advance();

      return value();
    }

  else
    if(c == ',')
    {
      r.advance();

      goto RESTART;
    }


    if(c == '{')
    {
      r.advance();

      return value(list(r,'}'));
    }

  else
    if((c == '\'') ||
       (c == '\"'))
    {
      r.advance();

      string  s(r.read_quoted_string(c));

      s.set_value(read_value_of_pair(r,cl));

      return value(std::move(s));
    }

  else
    if(r.is_pointing_identifier())
    {
      string  s(r.read_identifier());

      s.set_value(read_value_of_pair(r,cl));

      return value(std::move(s));
    }

  else
    if(r.is_pointing_number() || (c == '-'))
    {
      return read_number(r);
    }

  else
    {
      printf("%c(%d)は処理できない\n",c,c);

      throw r.get_context();
    }


  return value();
}




void
list::
assign(tok::stream_reader&  reader, char  cl)
{
  clear();

  std::vector<value>  ls;

    for(;;)
    {
      auto  v = read_value(reader,cl);

        if(!v)
        {
          break;
        }


      ls.emplace_back(std::move(v));
    }


  m_data = new value[ls.size()];

  m_number_of_values = ls.size();

  auto  dst = m_data;

    for(auto&&  v: ls)
    {
      *dst++ = std::move(v);
    }
}




list&
list::
operator=(const list&  rhs) noexcept
{
  clear();

  m_data = new value[rhs.size()];

  m_number_of_values = rhs.size();

  auto  p = m_data;

    for(auto&  v: rhs)
    {
      *p++ = v;
    }


  return *this;
}


list&
list::
operator=(list&&  rhs) noexcept
{
  clear();

  std::swap(m_data,rhs.m_data);

  m_number_of_values = rhs.size();

  return *this;
}




const value&
list::
get_named_value(gbstd::string_view  name) const
{
  auto  v = find_named_value(name);

    if(!v)
    {
      throw value_was_not_found{name};
    }


  return *v;
}


const value*
list::
find_named_value(gbstd::string_view  name) const noexcept
{
    for(auto&  v: *this)
    {
        if(v.is_string() && (v.get_string() == name))
        {
          return v.get_string().get_value();
        }
    }


  return nullptr;
}


const value*
list::
access(std::initializer_list<const char*>  ls) const noexcept
{
  auto  it     = ls.begin();
  auto  it_end = ls.end();

  const value*  v = nullptr;

    if(it != it_end)
    {
      v = find_named_value(*it++);

        if(!v)
        {
          return nullptr;
        }


        while(it != it_end)
        {
            if(!v->is_list())
            {
              return nullptr;
            }


          v = v->get_list().find_named_value(*it++);

            if(!v)
            {
              return nullptr;
            }
        }
    }


  return v;
}




void
list::
open(gbstd::string_view  filepath)
{
  gbstd::string_copy  sc(filepath);

  auto  f = fopen(sc.data(),"rb");

    if(f)
    {
      std::string  s;

        for(;;)
        {
          auto  c = fgetc(f);

            if(feof(f) || ferror(f))
            {
              break;
            }


          s.push_back(c);
        }


      fclose(f);


      tok::stream_reader  r(s.data());

      assign(r,0);
    }

  else
    {
      printf("%sを開けない\n",sc.data());
    }
}


void
list::
clear() noexcept
{
  delete[] m_data          ;
           m_data = nullptr;

  m_number_of_values = 0;
}


const value*  list::begin() const noexcept{return m_data;}
const value*    list::end() const noexcept{return m_data+m_number_of_values;}


void
list::
print(int  indent) const noexcept
{
  printf("{\n");

    for(auto&  v: *this)
    {
        for(int  n = 0;  n < indent;  ++n)
        {
          printf("  ");
        }


      v.print(indent+1);

      printf("\n");
    }


    if(indent)
    {
        for(int  n = 0;  n < indent-1;  ++n)
        {
          printf("  ");
        }
    }


  printf("}\n");
}


}}




