#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>

using namespace fisa;

// Event that trigger the transiton from state "Booting" to state "OS running".
class StartOS : public ChangeEvent<bool>
{
public:
  StartOS() : ChangeEvent<bool>()
  {
    add("start OS", false);
  }

  bool happened() const
  {
    return value("start OS");
  }
};

// Event that trigger the fork composite transition from the state "Initial" to "Booting" and "Cooling".
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

// Event that trigger the join composite transition from states "OS running" and "Cooling", to the state "Final".
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

// Definition of the machine.
class MyMachine : public Machine
{
public:
  MyMachine() : Machine("Example machine 1") {}
  virtual ~MyMachine() {}
  
  bool build()
  {
    // Adding the region "Computer" in the machine.
    this->newRegion("Computer");

    // Adding an "InitialState" named "initial" in the region "Computer" of the machine.
    auto initial = std::make_shared<InitialState>("Initial");
    this->addState("Computer", initial);

    // Adding a "SimpleState" named "Computer powered" in the region "Computer" of the machine.
    auto powered = std::make_shared<SimpleState>("Computer powered");
    this->addState("Computer", powered);

    // Adding an untriggered transition from state "Initial" to state "Computer powered".
    auto t0 = std::make_shared<Transition>("t0", "Initial", "Computer powered");
    this->addTransition(t0);

    // Adding a "CompositeState" named "Computer ON" in the region "Computer" of the machine.
    auto computerOn = std::make_shared<CompositeState>("Computer ON");
    // New region named "Motherboard" in state "Computer ON".
    computerOn->newRegion("Motherboard");
    // New region named "Temperature control" in state "Computer ON".
    computerOn->newRegion("Temperature control"); 
    this->addState("Computer", computerOn);

    // Adding a "SimpleState" state named "Booting" in region "Motherboard".
    auto motherboard_booting = std::make_shared<SimpleState>("Booting");
    this->addState("Motherboard", motherboard_booting);

    // Adding a "SimpleState" state named "OS running" in region "Motherboard".
    auto motherboard_osrunning = std::make_shared<SimpleState>("OS running");
    this->addState("Motherboard", motherboard_osrunning);

    // Adding a transition triggered by event "StartOS" from state "Booting" to "OS running".
    auto t1 = std::make_shared<Transition>("t1", "Booting", "OS running");
    startOs = std::make_shared<StartOS>();
    t1->setTrigger(startOs);
    this->addTransition(t1);

    // Adding a "SimpleState" state name "Cooling" in region "Temperature control".
    auto temperature_cooling = std::make_shared<SimpleState>("Cooling");
    this->addState("Temperature control", temperature_cooling);

    // Adding a fork composite transition triggered by event "SwitchON" from state "Computer powered" to states
    // "Booting" and "Cooling".
    auto fork = std::make_shared<Fork>("fork", "Computer powered");
    fork->addOutgoing(std::make_shared<ForkOutgoing>("Booting"));
    fork->addOutgoing(std::make_shared<ForkOutgoing>("Cooling"));
    switchOn = std::make_shared<SwitchON>();
    fork->setTrigger(switchOn);
    this->addFork("Computer ON", fork);

    // Adding a "FinalState" state named "Final" in region "Computer" of the machine.
    auto final = std::make_shared<FinalState>("Final");
    this->addState("Computer", final);

    // Adding a join transtion tiggered by event "SwitchOFF" from states "OS running" and "Cooling" to state "Final".
    auto join = std::make_shared<Join>("join", "Final");
    join->addIncoming(std::make_shared<JoinIncoming>("OS running"));
    join->addIncoming(std::make_shared<JoinIncoming>("Cooling"));
    switchOff = std::make_shared<SwitchOFF>();
    join->setTrigger(switchOff);
    this->addJoin("Computer ON", join);

    return true;
  }

  // Declaration of the events.
  std::shared_ptr<StartOS> startOs;
  std::shared_ptr<SwitchON> switchOn;
  std::shared_ptr<SwitchOFF> switchOff;
};

int main(int argv, char **args)
{
  // Building the machine.
  MyMachine computer_machine;
  computer_machine.build();

  std::cout << "A simple modeling of a computer as a concurrent system." << std::endl << std::endl;
  
  // Firt run.
  if (!computer_machine.run())
    {
      std::cout << "example_computer, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** region \"Computer\" current state: " << computer_machine.activeState("Computer") << std::endl;
  std::cout << "*** region \"Motherboard\" current state: " << computer_machine.activeState("Motherboard") << std::endl;
  std::cout << "*** region \"Temperature control\" current state: " <<
    computer_machine.activeState("Temperature control") << std::endl;
  std::cout << std::endl;

  // Switching attribute of event "SwitchON" and second run.
  computer_machine.switchOn->switching("switch ON", true);
  if (!computer_machine.run())
    {
      std::cout << "example_computer, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** region \"Computer\" current state: " << computer_machine.activeState("Computer") << std::endl;
  std::cout << "*** region \"Moterboard\"  current state: " << computer_machine.activeState("Motherboard") << std::endl;
  std::cout << "*** region \"Temperature control\" current state: " <<
    computer_machine.activeState("Temperature control") << std::endl;
  std::cout << std::endl;

  // Switching attribute of event "StartOS" and third run.
  computer_machine.startOs->switching("start OS", true);
  if (!computer_machine.run())
    {
      std::cout << "example_computer, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** region \"Computer\" current state: " << computer_machine.activeState("Computer") << std::endl;
  std::cout << "*** region \"Motherboard\" current state: " << computer_machine.activeState("Motherboard") << std::endl;
  std::cout << "*** region \"Temperature control\" current state: " <<
    computer_machine.activeState("Temperature control") << std::endl;
  std::cout << std::endl;
  
  // Switching attribute of event "SwitchOFF" and fourth run.
  computer_machine.switchOff->switching("switch OFF", true);
  if (!computer_machine.run())
    {
      std::cout << "example_computer, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** region \"Computer\" current state: " << computer_machine.activeState("Computer") << std::endl;
  std::cout << "*** region \"Motherboard\" current state: " << computer_machine.activeState("Motherboard") << std::endl;
  std::cout << "*** region \"Temperature control\" current state: " <<
    computer_machine.activeState("Temperature control") << std::endl;
  std::cout << std::endl;
  
  return 0;
  
} // That's all!
