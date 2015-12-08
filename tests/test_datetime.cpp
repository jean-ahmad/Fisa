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

#include "datetime.hpp"

#include <iostream>

using namespace fisa;

int main(int argv, char **args) 
{
  DateTime date_time("2016-01-01T00:00:00");

  // Test 1
  if (!date_time.isLeapYear())
    {
      std::cout << "Test 1 failed." << std::endl;
      return -1;
    }

  // Test 2
  for (int i = 0; i < 366; ++i)
    {
      date_time = date_time + DateTime(0, 1, 0, 0, 0, 0);
    }
  if (date_time != DateTime("2017-01-01T00:00:00"))
    {
      std::cout << date_time.toIso8601() << std::endl;
      std::cout << "Test 2 failed." << std::endl;
      return -1;
    }

  // Test 3
  if (date_time.isLeapYear())
    {
      std::cout << "Test 3 failed." << std::endl;
      return -1;
    }

  // Test 4
  for (int i = 0; i < 365; ++i)
    {
      date_time = date_time + DateTime(0, 1, 0, 0, 0, 0);
    }
  if (date_time != DateTime("2018-01-01T00:00:00"))
    {
      std::cout << date_time.toIso8601() << std::endl;
      std::cout << "Test 4 failed." << std::endl;
      return -1;
    }

  // Test 5
  for (int i = 0; i < 1000000; ++i)
    {
      date_time = date_time + DateTime(0, 0, 0, 0, 0, 1);
    }
  if (date_time != DateTime("2018-01-01T00:00:01"))
    {
      std::cout << date_time.toIso8601() << " " << date_time.usecondString() << std::endl;
      std::cout << "Test 5 failed." << std::endl;
      return -1;
    }

  // Test 6
  for (int i = 0; i < 59; ++i)
    {
      date_time = date_time + DateTime(0, 0, 0, 0, 1, 0);
    }
  if (date_time != DateTime("2018-01-01T00:01:00"))
    {
      std::cout << date_time.toIso8601() << std::endl;
      std::cout << "Test 6 failed." << std::endl;
      return -1;
    }

  // Test 7
  for (int i = 0; i < 59; ++i)
    {
      date_time = date_time + DateTime(0, 0, 0, 1, 0, 0);
    }
  if (date_time != DateTime("2018-01-01T01:00:00"))
    {
      std::cout << date_time.toIso8601() << std::endl;
      std::cout << "Test 7 failed." << std::endl;
      return -1;
    }

  // Test 8
  for (int i = 0; i < 23; ++i)
    {
      date_time = date_time + DateTime(0, 0, 1, 0, 0, 0);
    }
  if (date_time != DateTime("2018-01-02T00:00:00"))
    {
      std::cout << date_time.toIso8601() << std::endl;
      std::cout << "Test 8 failed." << std::endl;
      return -1;
    }

  // Test 9
  if ((date_time < DateTime("2018-01-02T00:00:00")) || (date_time < DateTime(2018, 1, 23, 59, 59, 999999)))
    {
      std::cout << "Test 9 failed." << std::endl;
      return -1;
    }

  // Test 10
  if (!(date_time > DateTime(2018, 1, 23, 59, 59, 999999)))
    {
      std::cout << "Test 10 failed." << std::endl;
      return -1;
    }

  // Result
#ifdef OPENSOURCE_PLATFORM_TIME
  std::cout << "TESTING \"DateTime\" SUCCESSED AT SYSTEM TIME: " << OpenSourceTime::now().toIso8601() << std::endl;
#elif WINDOWS_PLATFORM_TIME
  std::cout << "TESTING \"DateTime\" SUCCESSED AT SYSTEM TIME: " << WindowsTime::now().toIso8601() << std::endl;
#endif
  
  return 0;
}
