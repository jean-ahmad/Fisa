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



class MyMachine1 : public Machine
{
public:
  MyMachine1(const char *in_machine_name) : Machine(in_machine_name) {}
  virtual ~MyMachine1() {}
  
  bool build()
  {
    // Adding a region named "main1" in the machine:
    this->newRegion("main1");

    // States in region "main1":
    auto initial = std::make_shared<InitialState>("initial");
    this->addState("main1", initial);
    auto state1 = std::make_shared<SimpleState>("state1");
    this->addState("main1", state1);
    auto final = std::make_shared<FinalState>("final");
    this->addState("main1", final);
  
    // Transitions in region "main1":
    auto transition_initial_state1 = std::make_shared<Transition>("initial_to_state1", "initial", "state1");
    this->addTransition(transition_initial_state1);
  
    auto transition_state1_final = std::make_shared<Transition>("state1_to_final", "state1", "final");
    this->_trigger_state1_final = std::make_shared<EventState1Final>();
    transition_state1_final->setTrigger(this->_trigger_state1_final);
    this->addTransition(transition_state1_final);
  
    return true;
  }

  std::shared_ptr<EventState1Final> _trigger_state1_final;
};

class MyMachine2 : public Machine
{
public:
  MyMachine2(const char *in_machine_name) : Machine(in_machine_name) {}
  virtual ~MyMachine2() {}
  
  bool build()
  {
    // Adding a region named "main2" in the machine:
    this->newRegion("main2");
    
    // States in region "main2":
    auto initial = std::make_shared<InitialState>("initial");
    this->addState("main2", initial);
    
    MyMachine1 machine1("machine1");
    machine1.build();
    this->addSubmachine("main2", machine1);
    this->_trigger_state1_final = machine1._trigger_state1_final;
    
    // Transitions in region "main2":
    auto transition_initial_machine1 = std::make_shared<Transition>("initial_to_machine1", "initial", "machine1");
    this->addTransition(transition_initial_machine1);
    
    return true;
  }
  
  std::shared_ptr<EventState1Final> _trigger_state1_final;
};


int main(int argv, char **args)
{
  MyMachine2 test("machine2");

#ifdef DEBUG
  std::cout << "Building machine." << std::endl;
#endif
  test.build();
#ifdef DEBUG
  std::cout << "Machine builded." << std::endl;
#endif
  
  // Test 1
  if (!test.run())
    {
      std::cout << "ERROR: machine_test3, run failed." << std::endl;
      return -1;
    }

  if (test.activeState("main1") != std::string("state1") ||
      test.activeState("main2") != std::string("machine1"))
    {
      std::cout << "*** main1 current state: " << test.activeState("main1") << std::endl;
      std::cout << "*** main2 current state: " << test.activeState("main2") << std::endl;
      std::cout << ">>> TEST 1 FAILED" << std::endl;
      return -1;
    }

  // Test 2
  test._trigger_state1_final->switching("b", true);
  if (!test.run())
    {
      std::cout << "ERROR: machine_test3, run failed." << std::endl;
      return -1;
    }
  
  if (test.activeState("main1") != std::string("final") ||
      test.activeState("main2") != std::string("machine1"))
    {
      std::cout << "*** main1 current state: " << test.activeState("main1") << std::endl;
      std::cout << "*** main2 current state: " << test.activeState("main2") << std::endl;
      std::cout << ">>> TEST 2 FAILED" << std::endl;
      return -1;
    }
  
  // Result
  std::cout << ">>> TESTING \"Machine::addSubmachine\" SUCCESSED" << std::endl;
  
  return 0;
}
