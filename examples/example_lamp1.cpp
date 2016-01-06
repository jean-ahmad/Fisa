#include "lamp_machine.hpp"

int main(void)
{
  // Building of the machine.
  LampMachine lamp_machine;
  lamp_machine.build();

  std::cout << "Automaton that switches a lamp ON and OFF." << std::endl << std::endl;

  // Running the machine from state "Initial" to state "Lamp OFF".
  if (!lamp_machine.run())
    {
      std::cout << "example_lamp1: run failed." << std::endl;
      return -1;
    }
  std::cout << std::endl;
  
  // Switching 5 times from state "Lamp OFF" to state "Lamp ON" and from state "Lamp ON" to state "Lamp OFF".
  for (int i = 0; i < 5; i++)
    {
      // switching ON
      lamp_machine.switchOn->switching("switch ON", true);
      lamp_machine.switchOff->switching("switch OFF", false);
      if (!lamp_machine.run())
	{
	  std::cout << "example_lamp1: run failed." << std::endl;
	  return -1;
	}
      std::cout << std::endl;

      // switching OFF
      lamp_machine.switchOn->switching("switch ON", false);
      lamp_machine.switchOff->switching("switch OFF", true);
      if (!lamp_machine.run())
	{
	  std::cout << "example_lamp1: run failed." << std::endl;
	  return -1;
	}
      std::cout << std::endl;
    }  
  
  return 0;
  
} // That's all!
