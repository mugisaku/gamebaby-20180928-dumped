#include"libgbstd/space.hpp"




namespace gbstd{
namespace spaces{




void
object::
update() noexcept
{
  auto&  env = m_space->get_environment();

  m_body.save_base_point();

    if(!is_fixed())
    {
      m_kinetic_energy.y += env.get_gravitation();

      m_kinetic_energy *= (1.0-env.get_fluid_viscosity());

      m_kinetic_energy += env.get_fluid_kinetic_energy();

      m_body.add_base_point(m_kinetic_energy);
    }


    if(is_landing())
    {
//      m_kinetic_energy.x /= 1.2;
    }
}


void
object::
print() const noexcept
{
  printf("object: {\n");

  m_body.print();

  printf("kinetic energy: {%8f,%8f}\n",m_kinetic_energy.x,m_kinetic_energy.y);

  printf("}\n\n");
}




}}




