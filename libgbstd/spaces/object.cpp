#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
object::
update_core() noexcept
{
  auto  ene = get_kinetic_energy();

    if(ene.x < 0)
    {
        if(m_left_contacted_square)
        {
          ene.x = 0;
        }
    }

  else
    if(ene.x > 0)
    {
        if(m_right_contacted_square)
        {
          ene.x = 0;
        }
    }


    if(ene.y < 0)
    {
        if(m_up_contacted_square)
        {
          ene.y = 0;
        }
    }

  else
    if(ene.y > 0)
    {
        if(m_down_contacted_square)
        {
          ene.y = 0;
        }
    }


  add_base_point(ene);


  auto  env = get_environment();

    if(env)
    {
      ene.y += env->get_gravitation();

      ene *= (1.0-env->get_fluid_viscosity());

      ene += env->get_fluid_kinetic_energy();

      set_kinetic_energy(ene);
    }

}


void
object::
print() const noexcept
{
  printf("object: {\n");

  body::print();

  printf("}\n\n");
}




}}




