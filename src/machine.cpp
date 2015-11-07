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

#include "machine.hpp"

using namespace fisa;

//#########################################################################################################
/*
  Machine
*/

// -----------------------------------------------------------------------------------
Machine::Machine()
{
  this->_isInitiated = false;
  this->_isTerminated = false;
}

// -----------------------------------------------------------------------------------
Machine::~Machine()
{
}

// -----------------------------------------------------------------------------------
std::string Machine::activeState(const char *in_region_name) const
{
  std::shared_ptr<std::string> region_name = std::make_shared<std::string>(in_region_name);
  std::shared_ptr<Region> region = this->findRegion(region_name);
  if (region)
    {
      std::shared_ptr<SimpleState> active_state = region->activeState();
      if (active_state) return *(active_state->name());
      else
	{
#ifdef WARNING
	  std::cout << "WARNING: Machine::activeState, region \"" << *region_name <<
	    "\" doesn't have any active state." << std::endl;
#endif
	  return std::string("");
	}
    }
  else 
    {
#ifdef WARNING
      std::cout << "WARNING: Machine::activeState, region \"" << *region_name <<
	"\" doesn't exist." << std::endl;
#endif
      return std::string("");
    }
}

// -----------------------------------------------------------------------------------
bool Machine::run()
{
  if (this->_isTerminated)
    {
#ifdef DEBUG
      std::cout << "DEBUG: Machine::run, terminated." << std::endl;
#endif
      return true;
    }
  else if (!this->_isInitiated)
    {
      for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin();
	   it != this->_regions.end(); it++)
	if(!(*it)->init())
	  {
	    std::cout << "ERROR: Machine::run, region \"" << *((*it)->name()) <<
	      "\" initialization failed" << std::endl;
	    return false;
	  }
      this->_isInitiated = true;
      return true;
    }
  else
    {
      for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
	{
	  RegionInfo region_info;
	  region_info.init();
	  if(!(*it)->run(region_info))
	    {
	      std::cout << "ERROR: Machine::run(), region \"" << *((*it)->name()) <<
		"\" run failed" << std::endl;
	      return false;
	    }
	  if (region_info._is_terminated) this->_isTerminated = true;
	}  
      return true;
    }
}

// -----------------------------------------------------------------------------------
void Machine::newRegion(const char *in_region_name)
{
  this->_regions.push_back(std::make_shared<Region>(in_region_name));
}

// -----------------------------------------------------------------------------------
bool Machine::addState(const char *in_region_name, std::shared_ptr<SimpleState> in_state)
{
  std::shared_ptr<std::string> region_name = std::make_shared<std::string>(in_region_name);
  std::shared_ptr<Region> region = this->findRegion(region_name);
  if (!region)
    {
      std::cout << "ERROR: Machine::addState, region \"" << *region_name <<
	"\" not founded." << std::endl;
      return false;
    }
  in_state->setOwningRegion(region_name);
  region->addState(in_state);      
  return true;  
}

// -----------------------------------------------------------------------------------
bool Machine::addTransition(std::shared_ptr<Transition> in_transition)
{
  if (in_transition->startingStates() != 1)
    {
      std::cout << "ERROR: Machine::addTransition, use \"Machine::addJoin\" to add join transitions." << std::endl;
      return false;
    }
  if (in_transition->reachableStates() != 1)
    {
      std::cout << "ERROR: Machine::addTransition, use \"Machine::addFork\" to add fork transitions." << std::endl;
      return false;
    }
  std::shared_ptr<std::string> starting_state_name = in_transition->startingState(0);
  std::shared_ptr<SimpleState> starting_state = this->findState(starting_state_name);
  if (!starting_state)
    {
      std::cout << "ERROR: Machine::addTransition, transition \"" << *(in_transition->name()) <<
	"\" starting state \"" << *starting_state_name << "\" not founded." << std::endl;
      return false;
    }
  std::shared_ptr<std::string> reachable_state_name = in_transition->reachableState(0);
  if (!starting_state->addTransition(in_transition))
    {
      std::cout << "ERROR: Machine::addTransition, can't add transition \""
		<< *(in_transition->name()) << "\"." << std::endl;
      return false;
    }  
  return true;
}

// -----------------------------------------------------------------------------------
bool Machine::addJoin(const char *in_outermost_starting_state_name, std::shared_ptr<JoinTransition> in_join)
{
  std::shared_ptr<std::string> outermost_starting_state_name = std::make_shared<std::string>(in_outermost_starting_state_name);
  if (in_join->startingStates() < 2)
    {
      std::cout << "ERROR: Machine::addJoin, join transition \"" << *(in_join->name()) <<
	"\" must have at least two incomings." << std::endl;
      return false;
    }
  std::shared_ptr<SimpleState> outermost_starting_state = this->findState(outermost_starting_state_name);
  if (!outermost_starting_state)
    {
      std::cout << "ERROR: Machine::addJoin, join transition \"" << *(in_join->name()) <<
	"\" can't retrieve outermost starting state \"" << *outermost_starting_state_name << "\"." << std::endl;
      return false;
    }
  if (!outermost_starting_state->checkForkOrJoin(in_join->startingStatesNames(), true))
    {
      std::cout << "ERROR: Machine::addJoin, join transition \"" << *(in_join->name()) <<
	"\" bad incoming transitions specification." << std::endl;
      return false;
    }
  return outermost_starting_state->addJoin(in_join);
}

// -----------------------------------------------------------------------------------
bool Machine::addFork(const char *in_outermost_reachable_state_name, std::shared_ptr<ForkTransition> in_fork)
{
  std::shared_ptr<std::string> outermost_reachable_state_name = std::make_shared<std::string>(in_outermost_reachable_state_name);
  if (in_fork->reachableStates() < 2)
    {
      std::cout << "ERROR: Machine::addFork, fork transition \"" << *(in_fork->name()) <<
	"\" must have at least two outgoings." << std::endl;
      return false;
    }
  std::shared_ptr<SimpleState> outermost_reachable_state = this->findState(outermost_reachable_state_name);
  if (!outermost_reachable_state)
    {
      std::cout << "ERROR: Machine::addFork, fork transition \"" << *(in_fork->name()) <<
	"\" can't retrieve outermost reachable state \"" << *outermost_reachable_state_name << "\"." << std::endl;
      return false;
    }
  if (!outermost_reachable_state->checkForkOrJoin(in_fork->reachableStatesNames(), true))
    {
      std::cout << "ERROR: Machine::addFork, fork transition \"" << *(in_fork->name()) <<
	"\" bad outgoing transitions specification." << std::endl;
      return false;
    }
  std::shared_ptr<std::string> starting_state_name = in_fork->startingState(0);
  std::shared_ptr<SimpleState> starting_state = this->findState(starting_state_name);
  if (!starting_state)
    {
      std::cout << "ERROR: Machine::addFork, fork transition \"" << *(in_fork->name()) <<
	"\" can't retrieve starting state \"" << *starting_state_name << "\"." << std::endl;
      return false;
    }    
  return starting_state->addTransition(in_fork);
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Region> Machine::findRegion(std::shared_ptr<std::string> in_region_name) const
{
  std::shared_ptr<Region> region;  
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      if (*((*it)->name()) == *in_region_name) return *it;
      else
	{
	  region = (*it)->findRegion(in_region_name);
	  if (region) return region;
	}
    }
  
#ifdef WARNING
  std::cout << "WARNING: Machine::findRegion, region \"" << *in_region_name << "\" not founded." << std::endl;
#endif
  return nullptr;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> Machine::findState(std::shared_ptr<std::string> in_state_name) const
{
  std::shared_ptr<SimpleState> state;
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      state = (*it)->findState(in_state_name);
      if (state) return state;
    }

#ifdef WARNING
  std::cout << "WARNING: Machine::findState, state \"" << *in_state_name << "\" not founded." <<
    std::endl;
#endif
  return nullptr;
}

