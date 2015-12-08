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

class EventState1Final : public ChangeEvent<bool>
{
public:
  EventState1Final() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && value("b") && !value("c");
  }
};

class EventSub1state1Sub1State2 : public ChangeEvent<bool>
{
public:
  EventSub1state1Sub1State2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && !value("b") && value("c");
  }
};

class EventSub1state2Sub1State1 : public ChangeEvent<bool>
{
public:
  EventSub1state2Sub1State1() : ChangeEvent<bool>()
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

class EventSub1state2Sub1Final : public ChangeEvent<bool>
{
public:
  EventSub1state2Sub1Final() : ChangeEvent<bool>()
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

class EventSub2state1Sub2State2 : public ChangeEvent<bool>
{
public:
  EventSub2state1Sub2State2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && value("b") && !value("c");
  }
};

class EventSub2state2Sub2Final : public ChangeEvent<bool>
{
public:
  EventSub2state2Sub2Final() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && !value("b") && value("c");
  }
};

class EventSub3state1Sub3Final : public ChangeEvent<bool>
{
public:
  EventSub3state1Sub3Final() : ChangeEvent<bool>()
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

class EventSub4state1Sub4State2 : public ChangeEvent<bool>
{
public:
  EventSub4state1Sub4State2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return value("a") && !value("b") && !value("c");
  }
};

class EventSub4state2Sub4Final : public ChangeEvent<bool>
{
public:
  EventSub4state2Sub4Final() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && value("b") && value("c");
  }
};

class EventSub5state1Sub5State2 : public ChangeEvent<bool>
{
public:
  EventSub5state1Sub5State2() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && !value("b") && value("c");
  }
};

class EventSub5state2Sub5State1 : public ChangeEvent<bool>
{
public:
  EventSub5state2Sub5State1() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return !value("a") && value("b") && !value("c");
  }
};

class EventSub5state2Sub5Final : public ChangeEvent<bool>
{
public:
  EventSub5state2Sub5Final() : ChangeEvent<bool>()
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

class EventSub6state1Sub6Final : public ChangeEvent<bool>
{
public:
  EventSub6state1Sub6Final() : ChangeEvent<bool>()
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

class MyMachine : public Machine
{
public:
  
  virtual ~MyMachine() {}
  
  bool build();

  std::shared_ptr<EventState1Final> _trigger_state1_final;
  std::shared_ptr<EventSub1state1Sub1State2> _trigger_sub1state1_sub1state2;
  std::shared_ptr<EventSub1state2Sub1State1> _trigger_sub1state2_sub1state1;
  std::shared_ptr<EventSub1state2Sub1Final> _trigger_sub1state2_sub1final;
  std::shared_ptr<EventSub2state1Sub2State2> _trigger_sub2state1_sub2state2;
  std::shared_ptr<EventSub2state2Sub2Final> _trigger_sub2state2_sub2final;
  std::shared_ptr<EventSub3state1Sub3Final> _trigger_sub3state1_sub3final;
  std::shared_ptr<EventSub4state1Sub4State2> _trigger_sub4state1_sub4state2;
  std::shared_ptr<EventSub4state2Sub4Final> _trigger_sub4state2_sub4final;
  std::shared_ptr<EventSub5state1Sub5State2> _trigger_sub5state1_sub5state2;
  std::shared_ptr<EventSub5state2Sub5State1> _trigger_sub5state2_sub5state1;
  std::shared_ptr<EventSub5state2Sub5Final> _trigger_sub5state2_sub5final;
  std::shared_ptr<EventSub6state1Sub6Final> _trigger_sub6state1_sub6final;
};

bool MyMachine::build() 
{
  // Adding a region named "main" in the machine:
  this->newRegion("main");

  // States in region "main":
  std::shared_ptr<InitialState> initial = std::make_shared<InitialState>("initial");
  this->addState("main", initial);
  std::shared_ptr<CompositeState> state1 = std::make_shared<CompositeState>("state1");
  this->addState("main", state1);
  std::shared_ptr<FinalState> final = std::make_shared<FinalState>("final");
  this->addState("main", final);
  
  // Transitions in region "main":
  std::shared_ptr<Transition> transition_initial_state1 = std::make_shared<Transition>("initial_to_state1", "initial", "state1");
  this->addTransition(transition_initial_state1);
  
  std::shared_ptr<Transition> transition_state1_final = std::make_shared<Transition>("state1_to_final", "state1", "final");
  this->_trigger_state1_final = std::make_shared<EventState1Final>();
  transition_state1_final->setTrigger(this->_trigger_state1_final);
  this->addTransition(transition_state1_final);
  
  // States in region "sub1" of state "state1" in region "main":
  state1->newRegion("sub1");
  std::shared_ptr<InitialState> sub1initial = std::make_shared<InitialState>("sub1_initial");
  this->addState("sub1", sub1initial);
  std::shared_ptr<CompositeState> sub1state1 = std::make_shared<CompositeState>("sub1_state1");
  this->addState("sub1", sub1state1);
  std::shared_ptr<SimpleState> sub1state2 = std::make_shared<SimpleState>("sub1_state2");
  this->addState("sub1", sub1state2);
  std::shared_ptr<FinalState> sub1final = std::make_shared<FinalState>("sub1_final");
  this->addState("sub1", sub1final);
  
  // Transitions in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> transition_sub1initial_sub1state1 = std::make_shared<Transition>("sub1initial_to_sub1state1", "sub1_initial", "sub1_state1");
  this->addTransition(transition_sub1initial_sub1state1);
  
  std::shared_ptr<Transition> transition_sub1state1_sub1state2 = std::make_shared<Transition>("sub1state1_to_sub1state2", "sub1_state1", "sub1_state2");
  this->_trigger_sub1state1_sub1state2 = std::make_shared<EventSub1state1Sub1State2>();
  transition_sub1state1_sub1state2->setTrigger(this->_trigger_sub1state1_sub1state2);
  this->addTransition(transition_sub1state1_sub1state2);

  std::shared_ptr<Transition> transition_sub1state2_sub1state1 = std::make_shared<Transition>("sub1state2_to_sub1state1", "sub1_state2", "sub1_state1");
  this->_trigger_sub1state2_sub1state1 = std::make_shared<EventSub1state2Sub1State1>();
  transition_sub1state2_sub1state1->setTrigger(this->_trigger_sub1state2_sub1state1);
  this->addTransition(transition_sub1state2_sub1state1);

  std::shared_ptr<Transition> transition_sub1state2_sub1final = std::make_shared<Transition>("sub1state2_to_sub1final", "sub1_state2", "sub1_final");
  this->_trigger_sub1state2_sub1final = std::make_shared<EventSub1state2Sub1Final>();
  transition_sub1state2_sub1final->setTrigger(this->_trigger_sub1state2_sub1final);
  this->addTransition(transition_sub1state2_sub1final);
  
  // States in region "sub2" of state "state1" in region "main":
  state1->newRegion("sub2");
  std::shared_ptr<InitialState> sub2initial = std::make_shared<InitialState>("sub2_initial");
  this->addState("sub2", sub2initial);
  std::shared_ptr<SimpleState> sub2state1 = std::make_shared<SimpleState>("sub2_state1");
  this->addState("sub2", sub2state1);
  std::shared_ptr<CompositeState> sub2state2 = std::make_shared<CompositeState>("sub2_state2");
  this->addState("sub2", sub2state2);
  std::shared_ptr<FinalState> sub2final = std::make_shared<FinalState>("sub2_final");
  this->addState("sub2", sub2final);

  // Transitions in region "sub2" of state "state1" in region "main":
  std::shared_ptr<Transition> transition_sub2initial_sub2state1 = std::make_shared<Transition>("sub2intial_to_sub2state1", "sub2_initial", "sub2_state1");
  this->addTransition(transition_sub2initial_sub2state1);
  
  std::shared_ptr<Transition> transition_sub2state1_sub2state2 = std::make_shared<Transition>("sub2state1_to_sub2state2", "sub2_state1", "sub2_state2");
  this->_trigger_sub2state1_sub2state2 = std::make_shared<EventSub2state1Sub2State2>();
  transition_sub2state1_sub2state2->setTrigger(this->_trigger_sub2state1_sub2state2);
  this->addTransition(transition_sub2state1_sub2state2);

  std::shared_ptr<Transition> transition_sub2state2_sub2final = std::make_shared<Transition>("sub2state2_to_sub2final", "sub2_state2", "sub2_final");
  this->_trigger_sub2state2_sub2final = std::make_shared<EventSub2state2Sub2Final>();
  transition_sub2state2_sub2final->setTrigger(this->_trigger_sub2state2_sub2final);
  this->addTransition(transition_sub2state2_sub2final);

  // States in region "sub3" of state "state1" in region "sub1" of state "state1" in region "main":
  sub1state1->newRegion("sub3");
  std::shared_ptr<InitialState> sub3initial = std::make_shared<InitialState>("sub3_initial");
  this->addState("sub3", sub3initial);
  std::shared_ptr<SimpleState> sub3state1 = std::make_shared<SimpleState>("sub3_state1");
  this->addState("sub3", sub3state1);
  std::shared_ptr<FinalState> sub3final = std::make_shared<FinalState>("sub3_final");
  this->addState("sub3", sub3final);

  // Transitions in region "sub3" of state "state1" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> transition_sub3initial_sub3state1 = std::make_shared<Transition>("sub3initial_to_sub3state1", "sub3_initial", "sub3_state1");
  this->addTransition(transition_sub3initial_sub3state1);

  std::shared_ptr<Transition> transition_sub3state1_sub3final = std::make_shared<Transition>("sub3state1_to_sub3final", "sub3_state1", "sub3_final");
  this->_trigger_sub3state1_sub3final = std::make_shared<EventSub3state1Sub3Final>();
  transition_sub3state1_sub3final->setTrigger(this->_trigger_sub3state1_sub3final);
  this->addTransition(transition_sub3state1_sub3final);

  // States in region "sub4" of state "state1" in region "sub1" of state "state1" in region "main":
  sub1state1->newRegion("sub4");
  std::shared_ptr<InitialState> sub4initial = std::make_shared<InitialState>("sub4_initial");
  this->addState("sub4", sub4initial);
  std::shared_ptr<SimpleState> sub4state1 = std::make_shared<SimpleState>("sub4_state1");
  this->addState("sub4", sub4state1);
  std::shared_ptr<SimpleState> sub4state2 = std::make_shared<SimpleState>("sub4_state2");
  this->addState("sub4", sub4state2);
  std::shared_ptr<FinalState> sub4final = std::make_shared<FinalState>("sub4_final");
  this->addState("sub4", sub4final);

  // Transitions in region "sub4" of state "state1" in region "sub1" of state "state1" in region "main":
  std::shared_ptr<Transition> transition_sub4initial_sub4state1 = std::make_shared<Transition>("sub4initial_to_sub4final", "sub4_initial", "sub4_state1");
  this->addTransition(transition_sub4initial_sub4state1);
  
  std::shared_ptr<Transition> transition_sub4state1_sub4state2 = std::make_shared<Transition>("sub4state1_to_sub4state2", "sub4_state1", "sub4_state2");
  this->_trigger_sub4state1_sub4state2 = std::make_shared<EventSub4state1Sub4State2>();
  transition_sub4state1_sub4state2->setTrigger(this->_trigger_sub4state1_sub4state2);
  this->addTransition(transition_sub4state1_sub4state2);

  std::shared_ptr<Transition> transition_sub4state2_sub4final = std::make_shared<Transition>("sub4state2_to_sub4final", "sub4_state2", "sub4_final");
  this->_trigger_sub4state2_sub4final = std::make_shared<EventSub4state2Sub4Final>();
  transition_sub4state2_sub4final->setTrigger(this->_trigger_sub4state2_sub4final);
  this->addTransition(transition_sub4state2_sub4final);

  // States in region "sub5" of state "state2" in region "sub2" of state "state1" in region "main":
  sub2state2->newRegion("sub5");
  std::shared_ptr<InitialState> sub5initial = std::make_shared<InitialState>("sub5_initial");
  this->addState("sub5", sub5initial);
  std::shared_ptr<SimpleState> sub5state1 = std::make_shared<SimpleState>("sub5_state1");
  this->addState("sub5", sub5state1);
  std::shared_ptr<SimpleState> sub5state2 = std::make_shared<SimpleState>("sub5_state2");
  this->addState("sub5", sub5state2);
  std::shared_ptr<FinalState> sub5final = std::make_shared<FinalState>("sub5_final");
  this->addState("sub5", sub5final);

  // Transitions in region "sub5" of state "state2" in region "sub2" of state "state1" in region "main":
  std::shared_ptr<Transition> transition_sub5initial_sub5state1 = std::make_shared<Transition>("sub5initial_to_sub5state1", "sub5_initial", "sub5_state1");
  this->addTransition(transition_sub5initial_sub5state1);
  
  std::shared_ptr<Transition> transition_sub5state1_sub5state2 = std::make_shared<Transition>("sub5state1_to_su5state2", "sub5_state1", "sub5_state2");
  this->_trigger_sub5state1_sub5state2 = std::make_shared<EventSub5state1Sub5State2>();
  transition_sub5state1_sub5state2->setTrigger(this->_trigger_sub5state1_sub5state2);
  this->addTransition(transition_sub5state1_sub5state2);

  std::shared_ptr<Transition> transition_sub5state2_sub5state1 = std::make_shared<Transition>("sub5state2_to_sub5state1", "sub5_state2", "sub5_state1");
  this->_trigger_sub5state2_sub5state1 = std::make_shared<EventSub5state2Sub5State1>();
  transition_sub5state2_sub5state1->setTrigger(this->_trigger_sub5state2_sub5state1);
  this->addTransition(transition_sub5state2_sub5state1);

  std::shared_ptr<Transition> transition_sub5state2_sub5final = std::make_shared<Transition>("sub5state2_to_sub5final", "sub5_state2", "sub5_final");
  this->_trigger_sub5state2_sub5final = std::make_shared<EventSub5state2Sub5Final>();
  transition_sub5state2_sub5final->setTrigger(this->_trigger_sub5state2_sub5final);
  this->addTransition(transition_sub5state2_sub5final);
  
  // States in region "sub6" of state "state2" in region "sub2" of state "state1" in region "main":
  sub2state2->newRegion("sub6");
  std::shared_ptr<InitialState> sub6initial = std::make_shared<InitialState>("sub6_initial");
  this->addState("sub6", sub6initial);
  std::shared_ptr<SimpleState> sub6state1 = std::make_shared<SimpleState>("sub6_state1");
  this->addState("sub6", sub6state1);
  std::shared_ptr<FinalState> sub6final = std::make_shared<FinalState>("sub6_final");
  this->addState("sub6", sub6final);

  // Transitions in region "sub6" of state "state2" in region "sub2" of state "state1" on region "main":
  std::shared_ptr<Transition> transition_sub6initial_sub6state1 = std::make_shared<Transition>("sub6final_to_sub6state1", "sub6_initial", "sub6_state1");
  this->addTransition(transition_sub6initial_sub6state1);

  std::shared_ptr<Transition> transition_sub6state1_sub6final = std::make_shared<Transition>("sub6state1_to_sub6final", "sub6_state1", "sub6_final");
  this->_trigger_sub6state1_sub6final = std::make_shared<EventSub6state1Sub6Final>();
  transition_sub6state1_sub6final->setTrigger(this->_trigger_sub6state1_sub6final);
  this->addTransition(transition_sub6state1_sub6final);

  return true;
}




int main(int argv, char **args)
{
  MyMachine test;

#ifdef DEBUG
  std::cout << "Building machine." << std::endl;
#endif
  test.build();
#ifdef DEBUG
  std::cout << "Machine builded." << std::endl;
#endif
  
  // Initial
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, initial run failed." << std::endl;
      return -1;
    }

  // Test 1
  // state1 (sub1_state1 (sub3_state1 and sub4_state1) and sub2_state1)
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state1") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state1") ||
      test.activeState("sub5") != std::string("") ||
      test.activeState("sub6") != std::string(""))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 1 FAILED" << std::endl;
      return -1;
    }

  // Test 2
  // state1 (sub1_state1 (sub3_state1 and sub4_state1) and sub2_state2 (sub5_state1 and sub6_state1))
  test._trigger_sub2state1_sub2state2->switching("a", false);
  test._trigger_sub2state1_sub2state2->switching("b", true);
  test._trigger_sub2state1_sub2state2->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 2 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state1") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 2 FAILED" << std::endl;
      return -1;
    }

  // Test 3
  // state1 (sub1_state1 (sub3_state1 and sub4_state2) and sub2_state2 (sub5_state2 and sub6_state1))
  test._trigger_sub4state1_sub4state2->switching("a", true);
  test._trigger_sub4state1_sub4state2->switching("b", false);
  test._trigger_sub4state1_sub4state2->switching("c", false);
  test._trigger_sub5state1_sub5state2->switching("a", false);
  test._trigger_sub5state1_sub5state2->switching("b", false);
  test._trigger_sub5state1_sub5state2->switching("c", true);
  test._trigger_state1_final->switching("a", false);
  test._trigger_state1_final->switching("b", true);
  test._trigger_state1_final->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 3 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state2") ||
      test.activeState("sub5") != std::string("sub5_state2") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 3 FAILED" << std::endl;
      return -1;
    }

  // Test 4
  // state1 (sub1_state1 (sub3_final and sub4_final) and sub2_state2 (sub5_state2 and sub6_state1))
  test._trigger_sub4state1_sub4state2->switching("a", false);
  test._trigger_sub5state1_sub5state2->switching("c", false);
  test._trigger_state1_final->switching("b", false);  
  test._trigger_sub3state1_sub3final->switching("a", true);
  test._trigger_sub3state1_sub3final->switching("b", false);
  test._trigger_sub3state1_sub3final->switching("c", true);
  test._trigger_sub4state2_sub4final->switching("a", false);
  test._trigger_sub4state2_sub4final->switching("b", true);
  test._trigger_sub4state2_sub4final->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 4 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_final") ||
      test.activeState("sub4") != std::string("sub4_final") ||
      test.activeState("sub5") != std::string("sub5_state2") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 4 FAILED" << std::endl;
      return -1;
    }

  // Test 5
  // state1 (sub1_state2 and sub2_state2 (sub5_state1 and sub6_state1))
  test._trigger_sub3state1_sub3final->switching("a", false);
  test._trigger_sub3state1_sub3final->switching("c", false);
  test._trigger_sub4state2_sub4final->switching("b", false);
  test._trigger_sub4state2_sub4final->switching("c", false);  
  test._trigger_sub1state1_sub1state2->switching("a", false);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", true);
  test._trigger_sub5state2_sub5state1->switching("a", false);
  test._trigger_sub5state2_sub5state1->switching("b", true);
  test._trigger_sub5state2_sub5state1->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 5 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state2") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 5 FAILED" << std::endl;
      return -1;
    }

  // Test 6
  // state1 (sub1_state1 (sub3_state1 and sub4_state1) and sub2_state2 (sub5_state1 and sub6_state1))
  test._trigger_sub1state1_sub1state2->switching("c", false);
  test._trigger_sub5state2_sub5state1->switching("b", false);  
  test._trigger_sub1state2_sub1state1->switching("a", true);
  test._trigger_sub1state2_sub1state1->switching("b", true);
  test._trigger_sub1state2_sub1state1->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 6 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state1") ||
      test.activeState("sub5") != std::string("sub5_state1") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 6 FAILED" << std::endl;
      return -1;
    }

  // Test 7
  // state1 (sub1_state1 (sub3_state1 and sub4_state2) and sub2_state2 (sub5_state2 and sub6_state1))
  test._trigger_sub1state2_sub1state1->switching("a", false);
  test._trigger_sub1state2_sub1state1->switching("b", false);  
  test._trigger_sub4state1_sub4state2->switching("a", true);
  test._trigger_sub4state1_sub4state2->switching("b", false);
  test._trigger_sub4state1_sub4state2->switching("c", false);
  test._trigger_sub5state1_sub5state2->switching("a", false);
  test._trigger_sub5state1_sub5state2->switching("b", false);
  test._trigger_sub5state1_sub5state2->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 7 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_state1") ||
      test.activeState("sub4") != std::string("sub4_state2") ||
      test.activeState("sub5") != std::string("sub5_state2") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 7 FAILED" << std::endl;
      return -1;
    }
  
  // Test 8
  // state1 (sub1_state1 (sub3_final and sub4_final) and sub2_state2 (sub5_state2 and sub6_state1))
  test._trigger_sub4state1_sub4state2->switching("a", false);
  test._trigger_sub5state1_sub5state2->switching("c", false);  
  test._trigger_sub3state1_sub3final->switching("a", true);
  test._trigger_sub3state1_sub3final->switching("b", false);
  test._trigger_sub3state1_sub3final->switching("c", true);
  test._trigger_sub4state2_sub4final->switching("a", false);
  test._trigger_sub4state2_sub4final->switching("b", true);
  test._trigger_sub4state2_sub4final->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 8 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state1") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("sub3_final") ||
      test.activeState("sub4") != std::string("sub4_final") ||
      test.activeState("sub5") != std::string("sub5_state2") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 8 FAILED" << std::endl;
      return -1;
    }

  // Test 9
  // state1 (sub1_state2 and sub2_state2 (sub5_state2 and sub6_state1))
  test._trigger_sub3state1_sub3final->switching("a", false);
  test._trigger_sub3state1_sub3final->switching("c", false);
  test._trigger_sub4state2_sub4final->switching("b", false);
  test._trigger_sub4state2_sub4final->switching("c", false);  
  test._trigger_sub1state1_sub1state2->switching("a", false);
  test._trigger_sub1state1_sub1state2->switching("b", false);
  test._trigger_sub1state1_sub1state2->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 9 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_state2") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_state2") ||
      test.activeState("sub6") != std::string("sub6_state1"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 9 FAILED" << std::endl;
      return -1;
    }

  // Test 10
  // state1 (sub1_final and sub2_state2 (sub5_final and sub6_final))
  test._trigger_sub1state1_sub1state2->switching("c", false);
  test._trigger_sub1state2_sub1final->switching("a", true);
  test._trigger_sub1state2_sub1final->switching("b", true);
  test._trigger_sub1state2_sub1final->switching("c", true);
  test._trigger_sub5state2_sub5final->switching("a", true);
  test._trigger_sub5state2_sub5final->switching("b", true);
  test._trigger_sub5state2_sub5final->switching("c", false);
  test._trigger_sub6state1_sub6final->switching("a", true);
  test._trigger_sub6state1_sub6final->switching("b", false);
  test._trigger_sub6state1_sub6final->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 10 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_final") ||
      test.activeState("sub2") != std::string("sub2_state2") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("sub5_final") ||
      test.activeState("sub6") != std::string("sub6_final"))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 10 FAILED" << std::endl;
      return -1;
    }

  // Test 11
  // state1 (sub1_final and sub2_final)
  test._trigger_sub1state2_sub1final->switching("a", false);
  test._trigger_sub1state2_sub1final->switching("b", false);
  test._trigger_sub1state2_sub1final->switching("c", false);
  test._trigger_sub5state2_sub5final->switching("a", false);
  test._trigger_sub5state2_sub5final->switching("b", false);
  test._trigger_sub6state1_sub6final->switching("a", false);
  test._trigger_sub6state1_sub6final->switching("c", false);
  test._trigger_sub2state2_sub2final->switching("a", false);
  test._trigger_sub2state2_sub2final->switching("b", false);
  test._trigger_sub2state2_sub2final->switching("c", true);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 11 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("state1") ||
      test.activeState("sub1") != std::string("sub1_final") ||
      test.activeState("sub2") != std::string("sub2_final") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("") ||
      test.activeState("sub6") != std::string(""))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 11 FAILED" << std::endl;
      return -1;
    }

  // Test 12
  // Final
  test._trigger_sub2state2_sub2final->switching("c", false);
  test._trigger_state1_final->switching("a", false);
  test._trigger_state1_final->switching("b", true);
  test._trigger_state1_final->switching("c", false);
  if (!test.run())
    {
      std::cout << "ERROR: test_machine1, test 12 run failed." << std::endl;
      return -1;
    }
  if (test.activeState("main") != std::string("final") ||
      test.activeState("sub1") != std::string("") ||
      test.activeState("sub2") != std::string("") ||
      test.activeState("sub3") != std::string("") ||
      test.activeState("sub4") != std::string("") ||
      test.activeState("sub5") != std::string("") ||
      test.activeState("sub6") != std::string(""))
    {
      std::cout << "*** main current state: " << test.activeState("main") << std::endl;
      std::cout << "*** sub1 current state: " << test.activeState("sub1") << std::endl;
      std::cout << "*** sub2 current state: " << test.activeState("sub2") << std::endl;
      std::cout << "*** sub3 current state: " << test.activeState("sub3") << std::endl;
      std::cout << "*** sub4 current state: " << test.activeState("sub4") << std::endl;
      std::cout << "*** sub5 current state: " << test.activeState("sub5") << std::endl;
      std::cout << "*** sub6 current state: " << test.activeState("sub6") << std::endl;
      std::cout << ">>> TEST 12 FAILED" << std::endl;
      return -1;
    }

  // Result
  std::cout << ">>> TESTING \"SimpleState\", \"IntialState\", \"FinalState\", \"CompositeState\", \"ChangeEvent\" and  \"Transition\" SUCCESSED" << std::endl;
  
  return 0;
}
