#include <machine.hpp>

#include <string>
#include <memory>

#ifdef OPENSOURCE_PLATFORM_TIME
#include <sys/time.h>
#endif

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
  // Construction of a state named "off".
  LampOff() : SimpleState("off")
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

    // Adding a "FinalState" named "final" in the region "main". 
    this->addState("main", std::make_shared<FinalState>("final"));

    // Adding a transtion from the state "initial" to the state "on".
    this->addTransition(std::make_shared<Transition>("t0", "initial", "on"));

    // Creation of a "TimeEvent" trigger.
    std::shared_ptr<TimeEvent> trigger1 = std::make_shared<TimeEvent>();

    // Setting the duration before triggering at 2s with a margin of 5ms.
    // If the machine won't change of state, you must increase the margin.
    trigger1->after(std::make_shared<DateTime>(0, 0, 0, 0, 2, 0), std::make_shared<DateTime>(0, 0, 0, 0, 0, 5000));

    // Adding a transition from state "on" to state "off" with the trigger defined.
    std::shared_ptr<Transition> t1 = std::make_shared<Transition>("t1", "on", "off");
    t1->setTrigger(trigger1);
    this->addTransition(t1);

    // Adding a transition from state "on" to state "final".
    std::shared_ptr<Transition> t2 = std::make_shared<Transition>("t2", "on", "final");
    trigger_end = std::make_shared<TimeEvent>();
    t2->setTrigger(trigger_end);
    this->addTransition(t2);

    // Creation of a "TimeEvent" trigger.
    std::shared_ptr<TimeEvent> trigger2 = std::make_shared<TimeEvent>();

    // Setting the duration before triggering at 1s with a margin of 5ms.
    // If the machine won't change of state, you must increase the margin.
    trigger2->after(std::make_shared<DateTime>(0, 0, 0, 0, 1, 0), std::make_shared<DateTime>(0, 0, 0, 0, 1, 5000));

    // Adding a transition from state "off" to state "on" with the trigger defined.
    std::shared_ptr<Transition> t3 = std::make_shared<Transition>("t3", "off", "on");
    t3->setTrigger(trigger2);
    this->addTransition(t3);

    // Adding a transition from state "off" to state "final".
    std::shared_ptr<Transition> t4 = std::make_shared<Transition>("t4", "off", "final");
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
  std::cout << "Machine that automatically switch a lamp (2s ON and 1s OFF) during 10s." << std::endl;

  // Building of the machine.
  MyMachine machine;
  machine.build();

  // Setting the event that trigger transitions to the state "final" at 10s after the beginning.
#ifdef OPENSOURCE_PLATFORM_TIME 
  DateTime end = OpenSourceTime::now() + DateTime(0, 0, 0, 0, 10, 0);
#elif WINDOWS_PLATFORM_TIME
  DateTime end = WindowsTime::now() + DateTime(0, 0, 0, 0, 10, 0);
#endif
  machine.trigger_end->at(std::make_shared<DateTime>(end), std::make_shared<DateTime>(0, 0, 0, 0, 15, 0));

  // Run the machine until the state "final" is reached.
  while (machine.activeState("main") != std::string("final"))
    {
      machine.run();
    }
#else
  std::cout << "example_lamp2: time not supported." << std::endl;
#endif
  
  return 0;
} // That's all!
