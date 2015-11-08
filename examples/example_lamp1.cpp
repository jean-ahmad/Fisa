#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>

using namespace fisa;

// Definition of a "SimpleState" state that correspond to the lamp ON.
class LampOn : public SimpleState
{
public:
  // Construction of a state named "on".
  LampOn() : SimpleState("on")
  {}

  // Method called when the machine reach the state "on".
  void entry() const
  {
    std::cout << "Lamp is ON." << std::endl;
  }
  // Method called when the machine quit the state "on".
  void exit() const
  {
    std::cout << std::endl;
  }
    
};

// Definition of a "SimpleState" state that correspond to the lamp OFF.
class LampOff : public SimpleState
{
public:
  // Construction of a state named "off".
  LampOff() : SimpleState("off")
  {}

  // Method called when the machine reach the state "off".
  void entry() const
  {
    std::cout << "Lamp is OFF." << std::endl;
  }
  // Method called when the machine quit the state "off".
  void exit() const
  {
    std::cout << std::endl;
  }
};

// Definition of the "ChangeEvent" event that trigger the lamp from ON to OFF.
class EventOnOff : public ChangeEvent<bool>
{
public:
  EventOnOff() : ChangeEvent<bool>()
  {
    add("off", false);
  }

  bool happened() const
  {
    return value("off");
  }
};

// Definition of the "ChangeEvent" event that trigger the lamp from OFF to ON.
class EventOffOn : public ChangeEvent<bool>
{
public:
  EventOffOn() : ChangeEvent<bool>()
  {
    add("on", false);
  }

  bool happened() const
  {
    return value("on");
  }
};

// Definition of the machine.
class MyMachine : public Machine
{
public:
  
  virtual ~MyMachine() {}

  // Method to build the machine.
  bool build()
  {
    // Adding a new region named "main" in the machine.
    this->newRegion("main");

    // Adding an "InitialState" named "initial" in the region "main".
    this->addState("main", std::make_shared<InitialState>("initial"));

    // Adding the state "on" corresponding to the lamp ON in the region "main".
    this->addState("main", std::make_shared<LampOn>());

    // Adding the state "on" corresponding to the lamp OFF in the region "main".
    this->addState("main", std::make_shared<LampOff>());

    // Adding a transtion from the state "initial" to the state "on".
    this->addTransition(std::make_shared<Transition>("t0", "initial", "on"));

    // Adding a transition from state "on" to state "off".
    std::shared_ptr<Transition> t1 = std::make_shared<Transition>("t1", "on", "off");
    trigger_onoff = std::make_shared<EventOnOff>();
    t1->setTrigger(trigger_onoff);
    this->addTransition(t1);

    // Adding a transition from state "off" to state "on" with the trigger defined.
    std::shared_ptr<Transition> t2 = std::make_shared<Transition>("t2", "off", "on");
    trigger_offon = std::make_shared<EventOffOn>();
    t2->setTrigger(trigger_offon);
    this->addTransition(t2);
  
    return true;
  }

  // Triggers.
  std::shared_ptr<EventOnOff> trigger_onoff;
  std::shared_ptr<EventOffOn> trigger_offon;
};


int main(void)
{
  std::cout << "Machine to command lamp ON and OFF." << std::endl;

  // Building of the machine.
  MyMachine machine;
  machine.build();

  // Running the machine from state "initial" to state "on".
  if (!machine.run())
    {
      std::cout << "example_lamp1: run failed." << std::endl;
      return -1;
    }

  // Switching 5 times from state "on" to state "off" and from state "off" to state "on".
  for (int i = 0; i < 5; i++)
    {
      // switching OFF
      machine.trigger_onoff->switching("off", true);
      machine.trigger_offon->switching("on", false);
      if (!machine.run())
	{
	  std::cout << "example_lamp1: run failed." << std::endl;
	  return -1;
	}

      // switching ON
      machine.trigger_onoff->switching("off", false);
      machine.trigger_offon->switching("on", true);
      if (!machine.run())
	{
	  std::cout << "example_lamp1: run failed." << std::endl;
	  return -1;
	}
    }  
  
  return 0;
} // That's all!
