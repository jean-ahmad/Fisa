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

#include "transitions.hpp"

#include <vector>
#include <iostream>
#include <memory>

using namespace fisa;

class ChangeEvent1 : public ChangeEvent<bool>
{
public:
  ChangeEvent1() : ChangeEvent<bool>()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return (!value("a") && value("b") && !value("c"));
  }
};

class ChangeEvent2 : public ChangeEvent<bool>
{
public:
  ChangeEvent2()
  {
    add("a", false);
    add("b", false);
    add("c", false);
  }

  bool happened() const
  {
    return (!value("a") && value("b") && !value("c"));
  }
};

class ChangeEvent3 : public ChangeEvent<std::string>
{
public:
  ChangeEvent3()
  {
    add("a", std::string("stop"));
  }

  bool happened() const
  {
    return (value("a") == std::string("marche"));
  }
};



int main(void)
{

  // Tests for the ChangeEvent class
  ChangeEvent1 trigger1;

  // Test 1
  if (trigger1.happened())
    {
      std::cout << "Test 1 failed." << std::endl;
      return -1;
    }

  // Test 2
  trigger1.switching("b", true);
  if (!trigger1.happened())
    {
      std::cout << "Test 2 failed." << std::endl;
      return -1;
    }

  // Test 3
  trigger1.switching("c", true);
  if (trigger1.happened())
    {
      std::cout << "Test 3 failed." << std::endl;
      return -1;
    }

  // Test 4
  ChangeEvent3 trigger2;
  if (trigger2.happened())
    {
      std::cout << "Test 4 failed." << std::endl;
      return -1;
    }

  // Test 5
  trigger2.switching("a", std::string("marche"));
  if (!trigger2.happened())
    {
      std::cout << "Test 5 failed." << std::endl;
      return -1;
    }

  // Tests for the Transition class
  std::shared_ptr<ChangeEvent2> trigger3 = std::make_shared<ChangeEvent2>();

  // Test 6
  Transition t1("t1", "state1", "state2");
  t1.setTrigger(trigger3);
  if (!t1.isTriggered())
    {
      std::cout << "Test 6 failed." << std::endl;
      return -1;
    }

  // Test 7
  if (t1.isActivated() || (*(t1.reachableState(0)) != std::string("state2")))
    {
      std::cout << "Test 7 failed." << std::endl;
      return -1;
    }

  // Test 8
  trigger3->switching("b", true);
  if (!t1.isActivated())
    {
      std::cout << "Test 8 failed." << std::endl;
      return -1;
    }

  // Test 9
  trigger3->switching("c", true);
  if (t1.isActivated())
    {
      std::cout << "Test 9 failed." << std::endl;
      return -1;
    }
  
  // Result
  std::cout << ">>> TESTING \"ChangeEvent\" and \"Transition\" SUCCESSED" << std::endl;

  return 0;
}
