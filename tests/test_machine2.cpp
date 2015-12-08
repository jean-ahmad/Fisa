/*                                                                                    
*  FInite State Automata library                                                     
*                                                                                    
*  Copyright (c) 2015, Jean Ahmad (https://www.linkedin.com/in/jeanahmad)                 
*  All rights reserved.                                                              
*                                                                                    
*  Redistribution and use in source and binary forms, with or without modification,  
*  are permitted provided that the following conditions are met:                     
*                                                                                    
*  - Redistributions of source code must retain the above copyright notice, this     
*  list of conditions and the following disclaimer.                                  
*                                                                                    
*  - Redistributions in binary form must reproduce the above copyright notice, this  
*  list of conditions and the following disclaimer in the documentation and/or       
*  other materials provided with the distribution.                                   
*                                                                                    
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND   
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE            
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR  
*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON    
*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT           
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS     
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                      
*/

#include <machine.hpp>

#include <string>
#include <memory>

#include <iostream>


using namespace fisa;

class EventJoinstate1State2 : public ChangeEvent<bool>
{
public:
  EventJoinstate1State2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return value("a") && !value("b") && value("c");
  }
};

class EventState1State3 : public ChangeEvent<bool>
{
public:
  EventState1State3() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return value("a") && value("b") && !value("c");
  }
};

class EventSub1state1Sub1state2 : public ChangeEvent<bool>
{
public:
  EventSub1state1Sub1state2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }
  
  bool happened() const
  {
    return value("a") && !value("b") && value("c");
  }
};

class EventSub1state1Sub1terminate : public ChangeEvent<bool>
{
public:
  EventSub1state1Sub1terminate() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }
  
  bool happened() const
  {
    return value("a") && value("b") && value("c");
  }
};

class MyMachine : public Machine
{
public:
  
  virtual ~MyMachine() {}
  
  bool build();
  bool build1(std::shared_ptr<SimpleState> outermost_state);
  bool build2(std::shared_ptr<SimpleState> outermost_state);
  
  std::shared_ptr<CompositeState> state1;
  std::shared_ptr<EventJoinstate1State2> _trigger_joinstate1_state2;
  std::shared_ptr<EventState1State3> _trigger_state1_state3;
  std::shared_ptr<EventSub1state1Sub1state2> _trigger_sub1state1_sub1state2;
  std::shared_ptr<EventSub1state1Sub1terminate> _trigger_sub1state1_sub1terminate;
};

bool MyMachine::build() 
{
  this->newRegion("main");

  // States in region "main" of the machine:
  std::shared_ptr<InitialState> initial = std::make_shared<InitialState>("initial");
  this->addState("main", initial);
  state1 = std::make_shared<CompositeState>("state1");
  state1->newRegion("sub1");
  state1->newRegion("sub2");
  this->addState("main", state1);
  std::shared_ptr<SimpleState> state2 = std::make_shared<SimpleState>("state2");
  this->addState("main", state2);
  std::shared_ptr<SimpleState> state3 = std::make_shared<SimpleState>("state3");
  this->addState("main", state3);
  
  // Transitions in region "main" of the machine:
  std::shared_ptr<Transition> initial_to_state1 = std::make_shared<Transition>("initial_to_sate1", "initial", "state1");
  this->addTransition(initial_to_state1);
  std::shared_ptr<Transition> state2_to_state1 = std::make_shared<Transition>("state2_to_state1", "state2", "state1");
  this->addTransition(state2_to_state1);
  std::shared_ptr<Transition> state1_to_state3 = std::make_shared<Transition>("state1_to_state3", "state1", "state3");
  this->_trigger_state1_state3 = std::make_shared<EventState1State3>();
  state1_to_state3->setTrigger(this->_trigger_state1_state3);
  this->addTransition(state1_to_state3);
  
  // States in region "sub1" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub1initial = std::make_shared<InitialState>("sub1_initial");
  this->addState("sub1", sub1initial);
  std::shared_ptr<SimpleState> sub1state1 = std::make_shared<SimpleState>("sub1_state1");
  this->addState("sub1", sub1state1);
  std::shared_ptr<CompositeState> sub1state2 = std::make_shared<CompositeState>("sub1_state2");
  sub1state2->newRegion("sub3");
  sub1state2->newRegion("sub4");
  this->addState("sub1", sub1state2);
  std::shared_ptr<TerminateState> sub1terminate = std::make_shared<TerminateState>("sub1_terminate");
  this->addState("sub1", sub1terminate);

  // Transitions in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> sub1initial_to_sub1state1 = std::make_shared<Transition>("sub1initial_to_sub1state1", "sub1_initial", "sub1_state1");
  this->addTransition(sub1initial_to_sub1state1);
  std::shared_ptr<Transition> sub1state1_to_sub1state2 = std::make_shared<Transition>("sub1state1_to_sub1state2", "sub1_state1", "sub1_state2");
  this->_trigger_sub1state1_sub1state2 = std::make_shared<EventSub1state1Sub1state2>();
  sub1state1_to_sub1state2->setTrigger(this->_trigger_sub1state1_sub1state2);
  this->addTransition(sub1state1_to_sub1state2);
  std::shared_ptr<Transition> sub1state1_to_sub1terminate = std::make_shared<Transition>("sub1state1_to_sub1terminate", "sub1_state1", "sub1_terminate");
  this->_trigger_sub1state1_sub1terminate = std::make_shared<EventSub1state1Sub1terminate>();
  sub1state1_to_sub1terminate->setTrigger(this->_trigger_sub1state1_sub1terminate);
  this->addTransition(sub1state1_to_sub1terminate);

  // States in region "sub3" of state "state2" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub3initial = std::make_shared<InitialState>("sub3_initial");
  this->addState("sub3", sub3initial);
  std::shared_ptr<SimpleState> sub3state1 = std::make_shared<SimpleState>("sub3_state1");
  this->addState("sub3", sub3state1);

  // Transitions in region "sub3" of state "state2" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> sub3initial_to_sub3state1 = std::make_shared<Transition>("sub3initial_to_sub3state1", "sub3_initial", "sub3_state1");
  this->addTransition(sub3initial_to_sub3state1);

  // States in region "sub4" of state "state2" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub4initial = std::make_shared<InitialState>("sub4_initial");
  this->addState("sub4", sub4initial);
  std::shared_ptr<SimpleState> sub4state1 = std::make_shared<SimpleState>("sub4_state1");
  this->addState("sub4", sub4state1);

  // Transitions in region "sub4" of state "state2" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> sub4initial_to_sub4state1 = std::make_shared<Transition>("sub4initial_to_sub4state1", "sub4_initial", "sub4_state1");
  this->addTransition(sub4initial_to_sub4state1);

  // States in region "sub2" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub2initial = std::make_shared<InitialState>("sub2_initial");
  this->addState("sub2", sub2initial);
  std::shared_ptr<CompositeState> sub2state1 = std::make_shared<CompositeState>("sub2_state1");
  sub2state1->newRegion("sub5");
  sub2state1->newRegion("sub6");
  this->addState("sub2", sub2state1);
 
  // Transitions in region "sub2" of state "state1" in region "main":
  std::shared_ptr<Transition> sub2initial_to_sub2state1 = std::make_shared<Transition>("sub2initial_to_sub2state1", "sub2_initial", "sub2_state1");
  this->addTransition(sub2initial_to_sub2state1);

  // States in region "sub5" of state "state1" in region "sub2" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub5initial = std::make_shared<InitialState>("sub5_initial");
  this->addState("sub5", sub5initial);
  std::shared_ptr<CompositeState> sub5state1 = std::make_shared<CompositeState>("sub5_state1");
  sub5state1->newRegion("sub7");
  sub5state1->newRegion("sub8");
  this->addState("sub5", sub5state1);

  // Transitions in region "sub5" of state "state1" in region "sub2" of state "state1" in region "main":
  std::shared_ptr<Transition> sub5initial_to_sub5state1 = std::make_shared<Transition>("sub5initial_to_sub5state1", "sub5_initial", "sub5_state1");
  this->addTransition(sub5initial_to_sub5state1);

  // States in region "sub7" of state "state1" in region "sub5" of state "state1" in region "sub2" of state "state1"
  // in region "main":
  std::shared_ptr<InitialState> sub7initial = std::make_shared<InitialState>("sub7_initial");
  this->addState("sub7", sub7initial);
  std::shared_ptr<SimpleState> sub7state1 = std::make_shared<SimpleState>("sub7_state1");
  this->addState("sub7", sub7state1);
  
  // Transitions in region "sub7" of state "state1" in region "sub5" of state "state1" in region "sub2" of state "state1"
  // in region "main":
  std::shared_ptr<Transition> sub7initial_to_sub7state1 = std::make_shared<Transition>("sub7initial_to_sub7state1", "sub7_initial", "sub7_state1");
  this->addTransition(sub7initial_to_sub7state1);

  // States in region "sub8" of state "state1" in region "sub5" of state "state1" in region "sub2" of state "state1"
  // in region "main":
  std::shared_ptr<InitialState> sub8initial = std::make_shared<InitialState>("sub8_initial");
  this->addState("sub8", sub8initial);
  std::shared_ptr<SimpleState> sub8state1 = std::make_shared<SimpleState>("sub8_state1");
  this->addState("sub8", sub8state1);

  // Transitions in region "sub8" of state "state1" in region "sub5" of state "state1" in region "sub2" of state
  // "state1" in region "main":
  std::shared_ptr<Transition> sub8initial_to_sub8state1 = std::make_shared<Transition>("sub8initial_to_sub8state1", "sub8_initial", "sub8_state1");
  this->addTransition(sub8initial_to_sub8state1);

  // States in region "sub6" of state "state1" in region "sub2" of state "state1" in region "main":
  std::shared_ptr<InitialState> sub6initial = std::make_shared<InitialState>("sub6_initial");
  this->addState("sub6", sub6initial);
  std::shared_ptr<SimpleState> sub6state1 = std::make_shared<SimpleState>("sub6_state1");
  this->addState("sub6", sub6state1);

  // Transitions in region "sub6" of state "state1" in region "sub2" of state "state1" in region "main":
  std::shared_ptr<Transition> sub6initial_to_sub6state1 = std::make_shared<Transition>("sub6initial_to_sub6state1", "sub6_initial", "sub6_state1");
  this->addTransition(sub6initial_to_sub6state1);

  return true;
}

bool MyMachine::build1(std::shared_ptr<SimpleState> outermost_state) 
{
  bool all_ok = true;
  // build an incomplete "join" transition to check "checkJoinOrFork" method:
  std::shared_ptr<JoinTransition> join = std::make_shared<JoinTransition>("joinstate1_to_state2", "state2");
  join->addIncoming(std::make_shared<JoinIncoming>("sub3_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub4_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub7_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub8_state1"));
  //join->addIncoming(std::make_shared<JoinIncoming>("sub6_state1"));
  all_ok = all_ok && this->addJoin("state1", join);
  std::cout << std::endl;
  
  return all_ok;
}

bool MyMachine::build2(std::shared_ptr<SimpleState> outermost_state) 
{
  // build "fork" and "join" transitions:
  bool all_ok = true;
  
  std::shared_ptr<JoinTransition> join = std::make_shared<JoinTransition>("joinstate1_to_state2", "state2");
  join->addIncoming(std::make_shared<JoinIncoming>("sub3_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub4_state1"));
  //join->addIncoming(std::make_shared<JoinIncoming>("sub1_state2"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub7_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub8_state1"));
  join->addIncoming(std::make_shared<JoinIncoming>("sub6_state1"));
  this->_trigger_joinstate1_state2 = std::make_shared<EventJoinstate1State2>();
  //join->setTrigger(this->_trigger_joinstate1_state2);
  all_ok = all_ok && this->addJoin("state1", join);
  
  std::shared_ptr<ForkTransition> fork = std::make_shared<ForkTransition>("state3_to_forkstate1", "state3");
  fork->addOutgoing(std::make_shared<ForkOutgoing>("sub1_state1"));
  fork->addOutgoing(std::make_shared<ForkOutgoing>("sub2_state1"));
  all_ok = all_ok && this->addFork("state1", fork);
  
  return all_ok;
}

int main(int argv, char **args)
{
  MyMachine test;
  test.build();

  // Test 1
  if (test.build1(test.state1))
    {
      std::cout << ">>> TEST 1 FAILED" << std::endl;
      return -1;
    }

  // Test 2
  if (!test.build2(test.state1))
    {
      std::cout << ">>> TEST 2 FAILED" << std::endl;
      return -1;
    }

  // Test 3
  // state1 (sub1_state1 and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1)
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 3 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 3 FAILED" << std::endl;
      return -1;
    }

  // Test 4
  // state1 (sub1_state2 (sub3_state1 and sub4_state1) and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1))
  test._trigger_sub1state1_sub1state2->switching("a", true);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 4 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state2") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state1") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 4 FAILED" << std::endl;
      return -1;
    }

  // Test 5
  // state2
  test._trigger_sub1state1_sub1state2->switching("a", false);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", false);
  test._trigger_joinstate1_state2->switching("a", true);
  test._trigger_joinstate1_state2->switching("b", false);
  test._trigger_joinstate1_state2->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 5 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state2") ||
      test.activeState("sub1") != std::string("") ||
      test.activeState("sub2") != std::string("") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("") ||
      test.activeState("sub6") != std::string("") ||
      test.activeState("sub7") != std::string("") ||
      test.activeState("sub8") != std::string(""))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 5 FAILED" << std::endl;
      return -1;
    }

  // Test 6
  // state1 (sub1_state1 and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1)
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 6 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 6 FAILED" << std::endl;
      return -1;
    }

  // Test 7
  // state1 (sub1_state2 (sub3_state1 and sub4_state1) and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1))
  test._trigger_sub1state1_sub1state2->switching("a", true);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", true);
  test._trigger_joinstate1_state2->switching("a", false);
  test._trigger_joinstate1_state2->switching("b", false);
  test._trigger_joinstate1_state2->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 7 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state2") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state1") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 7 FAILED" << std::endl;
      return -1;
    }

  // Test 8
  // state3
  test._trigger_sub1state1_sub1state2->switching("a", false);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", false);
  test._trigger_state1_state3->switching("a", true);
  test._trigger_state1_state3->switching("b", true);
  test._trigger_state1_state3->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 8 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state3") ||
      test.activeState("sub1") != std::string("") ||
      test.activeState("sub2") != std::string("") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("") ||
      test.activeState("sub6") != std::string("") ||
      test.activeState("sub7") != std::string("") ||
      test.activeState("sub8") != std::string(""))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 8 FAILED" << std::endl;
      return -1;
    }

  // Test 9
  // state1 (sub1_state1 and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1)
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 9 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 9 FAILED" << std::endl;
      return -1;
    }

  // Test 10
  // state1 (sub1_terminate and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1)
  test._trigger_sub1state1_sub1terminate->switching("a", true);
  test._trigger_sub1state1_sub1terminate->switching("b", true);
  test._trigger_sub1state1_sub1terminate->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 10 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_terminate") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 10 FAILED" << std::endl;
      return -1;
    }

  // Test 11
  // state1 (sub1_terminate and sub2_state1 (sub5_state1 (sub7_state1 and sub8_state1) and sub6_state1)
  test._trigger_state1_state3->switching("a", true);
  test._trigger_state1_state3->switching("b", true);
  test._trigger_state1_state3->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine2, test 11 run failed." << std::endl;
      return -1;
    }  
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_terminate") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1") ||
      test.activeState("sub7") != std::string("sub7_state1") ||
      test.activeState("sub8") != std::string("sub8_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << "*** sub7 current state: " << test.activeState("sub7") << std::endl;
      std::cout << "*** sub8 current state: " << test.activeState("sub8") << std::endl;
      std::cout << ">>> TEST 11 FAILED" << std::endl;
      return -1;
    }

  // Result
  std::cout << ">>> TESTING \"TerminateState\", \"ForkTransition\" and \"JoinTransition\" SUCCESSED" << std::endl;
  
  return 0;
}
