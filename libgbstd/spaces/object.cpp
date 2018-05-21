#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
object::
update() noexcept
{
  auto&  env = m_space->get_environment();

    if(!is_fixed())
    {
      m_kinetic_energy.y += env.get_gravitation();

      m_kinetic_energy *= (1.0-env.get_fluid_viscosity());

      m_kinetic_energy += env.get_fluid_kinetic_energy();

      add_base_point(m_kinetic_energy);
    }


//    if(is_landing())
    {
      m_kinetic_energy.x /= 2;
    }
}


void
object::
print() const noexcept
{
  printf("object: {\n");

  body::print();

  printf("kinetic energy: {%8f,%8f}\n",m_kinetic_energy.x,m_kinetic_energy.y);

  printf("}\n\n");
}




}}




