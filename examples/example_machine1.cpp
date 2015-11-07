#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>

using namespace fisa;

// Event that trigger the transiton from state "sub1_state1" to state "sub1_state2".
class Event1 : public ChangeEvent<bool>
{
public:
  Event1() : ChangeEvent<bool>()
  {
    add("a", false);
  }

  bool happened() const
  {
    return value("a");
  }
};

// Event that trigger the join transition from states "sub1_state2" and "sub2_state1", to state "sub1_state2".
class Event2 : public ChangeEvent<bool>
{
public:
  Event2() : ChangeEvent<bool>()
  {
    add("b", false);
  }

  bool happened() const
  {
    return value("b");
  }
};

// Definition of the machine.
class MyMachine : public Machine
{
public:
  
  virtual ~MyMachine() {}
  
  bool build()
  {
    // Adding the region "main" in the machine.
    this->newRegion("main");

    // Adding an "InitialState" named "initial" in the region "main".
    std::shared_ptr<InitialState> initial = std::make_shared<InitialState>("initial");
    this->addState("main", initial);

    // Adding a "CompositeState" named "state1" in the region "main".
    std::shared_ptr<CompositeState> state1 = std::make_shared<CompositeState>("state1");
    // New region named "sub1" in state "state1".
    state1->newRegion("sub1");
    // New region named "sub2" in state "state1".
    state1->newRegion("sub2"); 
    this->addState("main", state1);

    // Adding a "SimpleState" state named "sub1_state1" in region "sub1".
    std::shared_ptr<SimpleState> sub1_state1 = std::make_shared<SimpleState>("sub1_state1");
    this->addState("sub1", sub1_state1);

    // Adding a "SimpleState" state named "sub1_state2" in region "sub1".
    std::shared_ptr<SimpleState> sub1_state2 = std::make_shared<SimpleState>("sub1_state2");
    this->addState("sub1", sub1_state2);

    // Adding a transition triggered by "Event1" event from state "sub1_state1" to "sub1_state2".
    std::shared_ptr<Transition> t1 = std::make_shared<Transition>("t1", "sub1_state1", "sub1_state2");
    event_1 = std::make_shared<Event1>();
    t1->setTrigger(event_1);
    this->addTransition(t1);

    // Adding a "SimpleState" state name "sub2_state1" in region "sub2".
    std::shared_ptr<SimpleState> sub2_state1 = std::make_shared<SimpleState>("sub2_state1");
    this->addState("sub2", sub2_state1);

    // Adding a fork transition which is not triggered from state "initial" to states "sub1_state1" and "sub2_state1".
    std::shared_ptr<ForkTransition> fork = std::make_shared<ForkTransition>("fork", "initial");
    fork->addOutgoing(std::make_shared<ForkOutgoing>("sub1_state1"));
    fork->addOutgoing(std::make_shared<ForkOutgoing>("sub2_state1"));
    this->addFork("state1", fork);

    // Adding a "FinalState" state named "final" in region "main".
    std::shared_ptr<FinalState> final = std::make_shared<FinalState>("final");
    this->addState("main", final);

    // Adding a join transtion tiggered by "Event2" event from states "sub1_state2" and "sub2_state1" to state "final".
    std::shared_ptr<JoinTransition> join = std::make_shared<JoinTransition>("join", "final");
    join->addIncoming(std::make_shared<JoinIncoming>("sub1_state2"));
    join->addIncoming(std::make_shared<JoinIncoming>("sub2_state1"));
    event_2 = std::make_shared<Event2>();
    join->setTrigger(event_2);
    this->addJoin("state1", join);

    return true;
  }

  // Declaration of the events.
  std::shared_ptr<Event1> event_1;
  std::shared_ptr<Event2> event_2;
};

int main(int argv, char **args)
{
  // Building the machine.
  MyMachine test;
  test.build();
  
  // Firt run.
  if (!test.run())
    {
      std::cout << "example_machine1, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** main current state: " << test.activeState("main") << std::endl;
  std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
  std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
  std::cout << std::endl;

  // Switching attribute of "Event1" event and second run.
  test.event_1->switching("a", true);
  if (!test.run())
    {
      std::cout << "example_machine1, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** main current state: " << test.activeState("main") << std::endl;
  std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
  std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
  std::cout << std::endl;

  // Switching attribute of "Event2" event and third run.
  test.event_2->switching("b", true);
  if (!test.run())
    {
      std::cout << "example_machine1, run failed." << std::endl;
      return -1;
    }  
  std::cout << "*** main current state: " << test.activeState("main") << std::endl;
  std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
  std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
  std::cout << std::endl;
 
  
  return 0;
} // That's all!
