#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>


using namespace fisa;

// Definition of a "SimpleState" state that correspond to the lamp ON.
class LampOn : public SimpleState
{
public:
  // Construction of a state named "Lamp ON".
  LampOn() : SimpleState("Lamp ON")
  {}

  // Method called when the machine reach the state "on".
  void entry() const
  {
#ifdef OPENSOURCE_PLATFORM_TIME
    std::cout << "Lamp is ON at " << OpenSourceTime::now().toIso8601() << std::endl;
#elif WINDOWS_PLATFORM_TIME
    std::cout << "Lamp is ON at " << WindowsTime::now().toIso8601() << std::endl;
#endif
  }
  // Method called when the machine quit the state "on".
  void exit() const
  {
    std::cout << std::endl;
  }
    
};

// Definition of a "SimpleState" that correspond to the lamp OFF.
class LampOff : public SimpleState
{
public:
  // Construction of a state named "Lamp OFF".
  LampOff() : SimpleState("Lamp OFF")
  {}

  // Method called when the machine reach the state "off".
  void entry() const
  {
#ifdef OPENSOURCE_PLATFORM_TIME
    std::cout << "Lamp is OFF at " << OpenSourceTime::now().toIso8601() << std::endl;
#elif WINDOWS_PLATFORM_TIME
    std::cout << "Lamp is OFF at " << WindowsTime::now().toIso8601() << std::endl;
#endif
  }
  // Method called when the machine quit the state "off".
  void exit() const
  {
    std::cout << std::endl;
  }
};

// Definition of the machine.
class MyMachine : public Machine
{
public:
  MyMachine() : Machine("Timed lamp machine") {}
  virtual ~MyMachine() {}

  // Method to build the machine.
  bool build()
  {
    // Adding a new region named "lamp" in the machine.
    this->newRegion("lamp");

    // Adding an "InitialState" named "Initial" in the region "lamp" of the machine.
    this->addState("lamp", std::make_shared<InitialState>("Initial"));

    // Adding the state "Lamp OFF" corresponding to the lamp OFF in the region "lamp" of the machine.
    this->addState("lamp", std::make_shared<LampOff>());

    // Adding the state "Lamp ON" corresponding to the lamp ON in the region "lamp" of the machine.
    this->addState("lamp", std::make_shared<LampOn>());

    // Adding a "FinalState" named "Final" in the region "lamp" of the machine. 
    this->addState("lamp", std::make_shared<FinalState>("Final"));

    // Adding a transtion from the state "Initial" to the state "Lamp OFF".
    this->addTransition(std::make_shared<Transition>("t0", "Initial", "Lamp OFF"));

    // Creation of a "TimeEvent" that trigger a transition.
    auto trigger1 = std::make_shared<TimeEvent>();

    // Setting the duration before triggering at 2s with a margin of 200ms.
    // If the machine won't change of state, you must increase the margin.
    trigger1->after(std::make_shared<DateTime>(0, 0, 0, 0, 2, 0), std::make_shared<DateTime>(0, 0, 0, 0, 0, 200000));

    // Adding a transition from state "Lamp OFF" to state "Lamp ON" with the trigger defined.
    auto t1 = std::make_shared<Transition>("t1", "Lamp OFF", "Lamp ON");
    t1->setTrigger(trigger1);
    this->addTransition(t1);

    // Adding a transition from state "Lamp OFF" to state "Final".
    auto t2 = std::make_shared<Transition>("t2", "Lamp OFF", "Final");
    trigger_end = std::make_shared<TimeEvent>();
    t2->setTrigger(trigger_end);
    this->addTransition(t2);

    // Creation of a "TimeEvent" that trigger a transition.
    auto trigger2 = std::make_shared<TimeEvent>();

    // Setting the duration before triggering at 1s with a margin of 200ms.
    // If the machine won't change of state, you must increase the margin.
    trigger2->after(std::make_shared<DateTime>(0, 0, 0, 0, 1, 0), std::make_shared<DateTime>(0, 0, 0, 0, 1, 200000));

    // Adding a transition from state "Lamp ON" to state "Lamp OFF" with the trigger defined.
    auto t3 = std::make_shared<Transition>("t3", "Lamp ON", "Lamp OFF");
    t3->setTrigger(trigger2);
    this->addTransition(t3);

    // Adding a transition from state "Lamp ON" to state "Final".
    auto t4 = std::make_shared<Transition>("t4", "Lamp ON", "Final");
    t4->setTrigger(trigger_end);
    this->addTransition(t4);
  
    return true;
  }

  // "TimeEvent" that trigger transitions to the state "final".
  std::shared_ptr<TimeEvent> trigger_end;
};


int main(void)
{
#if defined OPENSOURCE_PLATFORM_TIME || defined WINDOWS_PLATFORM_TIME
  
  // Building of the machine.
  MyMachine machine;
  machine.build();

  // Setting the event that trigger transitions to the state "Final" at 10s after the beginning.
#ifdef OPENSOURCE_PLATFORM_TIME 
  auto end = OpenSourceTime::now() + DateTime(0, 0, 0, 0, 10, 0);
#elif WINDOWS_PLATFORM_TIME
  auto end = WindowsTime::now() + DateTime(0, 0, 0, 0, 10, 0);
#endif
  machine.trigger_end->at(std::make_shared<DateTime>(end), std::make_shared<DateTime>(0, 0, 0, 0, 1, 0));

  std::cout << "Automaton that automatically switches a lamp (2s OFF and 1s ON) during 10s." << std::endl << std::endl;

  // Run the machine until the state "Final" is reached.
  while (machine.activeState("lamp") != std::string("Final"))
    {
      machine.run();
    }
#else
  std::cout << "example_lamp2: time not supported." << std::endl;
#endif
  
  return 0;
  
} // That's all!
