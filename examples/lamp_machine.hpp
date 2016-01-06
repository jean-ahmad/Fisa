#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>

using namespace fisa;

// Definition of a "SimpleState" state that correspond to the lamp ON.
class LampOn : public SimpleState
{
public:
  // Constructor.
  // The state is named "Lamp ON".
  LampOn() : SimpleState("Lamp ON")
  {}

  // Method called when the machine reach a state "Lamp ON".
  void entry() const
  {
    std::cout << "Lamp is ON." << std::endl;
  }
  // Method called when the machine quit a state "Lamp ON".
  void exit() const
  {}
  
};

// Definition of a "SimpleState" state that correspond to the lamp OFF.
class LampOff : public SimpleState
{
public:
  // Constructor.
  // The state is named "Lamp OFF".
  LampOff() : SimpleState("Lamp OFF")
  {}

  // Method called when the machine reach a state "Lamp OFF".
  void entry() const
  {
    std::cout << "Lamp is OFF." << std::endl;
  }
  // Method called when the machine quit the state "Lamp OFF".
  void exit() const
  {}
};

// Definition of the event that trigger the Lamp from ON to OFF.
class SwitchOFF : public ChangeEvent<bool>
{
public:
  SwitchOFF() : ChangeEvent<bool>()
  {
    add("switch OFF", false);
  }

  bool happened() const
  {
    return value("switch OFF");
  }
};

// Definition of the event that trigger the Lamp from OFF to ON.
class SwitchON : public ChangeEvent<bool>
{
public:
  SwitchON() : ChangeEvent<bool>()
  {
    add("switch ON", false);
  }

  bool happened() const
  {
    return value("switch ON");
  }
};

// Definition of the machine.
class LampMachine : public Machine
{
public:
  LampMachine() : Machine("Lamp machine") {}  
  virtual ~LampMachine() {}

  // Building of the machine.
  bool build()
  {
    // Adding a new region named "lamp" in the machine.
    this->newRegion("Lamp");

    // Adding an "InitialState" named "Initial" in the region "Lamp".
    this->addState("Lamp", std::make_shared<InitialState>("Initial"));

    // Adding the state "Lamp OFF" corresponding to the lamp OFF in the region "Lamp".
    this->addState("Lamp", std::make_shared<LampOff>());

    // Adding the state "Lamp ON" corresponding to the lamp ON in the region "Lamp".
    this->addState("Lamp", std::make_shared<LampOn>());

    // Adding a transtion named "t0" from the state "Initial" to the state "Lamp OFF".
    this->addTransition(std::make_shared<Transition>("t0", "Initial", "Lamp OFF"));

    // Adding a triggered transition named "t1" from state "Lamp OFF" to the state "Lamp ON".
    auto t1 = std::make_shared<Transition>("t1", "Lamp OFF", "Lamp ON");
    switchOn = std::make_shared<SwitchON>();
    t1->setTrigger(switchOn);
    this->addTransition(t1);

    // Adding a triggered transition named "t2" from state "Lamp ON" to the state "Lamp OFF".
    auto t2 = std::make_shared<Transition>("t2", "Lamp ON", "Lamp OFF");
    switchOff = std::make_shared<SwitchOFF>();
    t2->setTrigger(switchOff);
    this->addTransition(t2);
  
    return true;
  }

  // Triggers.
  std::shared_ptr<SwitchOFF> switchOff;
  std::shared_ptr<SwitchON> switchOn;
  
}; // That's all!
