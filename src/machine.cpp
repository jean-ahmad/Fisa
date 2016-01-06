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
Machine::Machine(const char *in_machine_name) : RegionsComponent()
{
  this->_machineName = std::make_shared<std::string>(in_machine_name);
  this->_isInitiated = false;
  this->_isTerminated = false;
}

// -----------------------------------------------------------------------------------
Machine::~Machine()
{
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Machine::name() const
{
  return this->_machineName;
}

// -----------------------------------------------------------------------------------
RegionsComponent Machine::regionsComponent()
{
  return std::move(*this);
}

// -----------------------------------------------------------------------------------
std::string Machine::activeState(const char *in_region_name) const
{
  auto region_name = std::make_shared<std::string>(in_region_name);
  auto region = this->findRegion(region_name);
  if (region)
    {
      auto active_state = region->activeState();
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
      std::cout << "DEBUG: Machine::run, machine execution terminated." << std::endl;
#endif
      return true;
    }
  else if (!this->_isInitiated)
    {
      if(!this->RegionsComponent::init())
	{
	  std::cout << "ERROR: Machine::run, run failed" << std::endl;
	  return false;
	}
      this->_isInitiated = true;
      return true;
    }
  else
    {
      RegionInfo region_info;
      region_info.init();
      if(!this->RegionsComponent::run(region_info))
	{
	  std::cout << "ERROR: Machine::run(), failed" << std::endl;
	  return false;
	}
      if (region_info._is_terminated) this->_isTerminated = true;
      return true;
    }
}

// -----------------------------------------------------------------------------------
void Machine::newRegion(const char *in_region_name)
{
  this->RegionsComponent::newRegion(in_region_name);
}

// -----------------------------------------------------------------------------------
bool Machine::addState(const char *in_region_name, std::shared_ptr<SimpleState> in_state)
{
  auto region_name = std::make_shared<std::string>(in_region_name);
  auto region = this->findRegion(region_name);
  if (!region)
    {
      std::cout << "ERROR: Machine::addState, region \"" << *region_name <<
	"\" not found." << std::endl;
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
      std::cout << "ERROR: Machine::addTransition, use Machine's \"addJoin\" method to add a join compound transition." << std::endl;
      return false;
    }
  if (in_transition->reachableStates() != 1)
    {
      std::cout << "ERROR: Machine::addTransition, use Machine's \"addFork\" method to add fork compound transition." << std::endl;
      return false;
    }
  auto starting_state_name = in_transition->startingState(0);
  auto starting_state = this->findState(starting_state_name);
  if (!starting_state)
    {
      std::cout << "ERROR: Machine::addTransition, state \"" << *starting_state_name <<
	"\" not found." << std::endl;
      std::cout << "Starting state of transition \"" << *(in_transition->name()) <<
	"\" not found." << std::endl;
      return false;
    }
  auto reachable_state_name = in_transition->reachableState(0);
  if (!starting_state->addTransition(in_transition))
    {
      std::cout << "ERROR: Machine::addTransition, adding transition \""
		<< *(in_transition->name()) << "\" failed." << std::endl;
      return false;
    }  
  return true;
}

// -----------------------------------------------------------------------------------
bool Machine::addJoin(const char *in_outermost_starting_state_name, std::shared_ptr<Join> in_join)
{
  auto outermost_starting_state_name = std::make_shared<std::string>(in_outermost_starting_state_name);
  if (in_join->startingStates() < 2)
    {
      std::cout << "ERROR: Machine::addJoin, join compound transition \"" << *(in_join->name()) <<
	"\" must have at least two incomings transition." << std::endl;
      return false;
    }
  auto outermost_starting_state = this->findState(outermost_starting_state_name);
  if (!outermost_starting_state)
    {
      std::cout << "ERROR: Machine::addJoin, adding join compound transition \"" << *(in_join->name()) <<
	"\" failed." << std::endl;
      std::cout << "Can't retrieve outermost starting state \"" << *outermost_starting_state_name <<
	"\"." << std::endl;
      
      return false;
    }
  if (!outermost_starting_state->checkForkOrJoin(in_join->startingStatesNames(), true))
    {
      std::cout << "ERROR: Machine::addJoin, bad incoming transitions specification for the join compound transition \"" <<
	*(in_join->name()) << "\"." << std::endl;
      return false;
    }
  return outermost_starting_state->addJoin(in_join);
}

// -----------------------------------------------------------------------------------
bool Machine::addFork(const char *in_outermost_reachable_state_name, std::shared_ptr<Fork> in_fork)
{
  auto outermost_reachable_state_name = std::make_shared<std::string>(in_outermost_reachable_state_name);
  if (in_fork->reachableStates() < 2)
    {
      std::cout << "ERROR: Machine::addFork, fork compound transition \"" << *(in_fork->name()) <<
	"\" must have at least two outgoings transition." << std::endl;
      return false;
    }
  auto outermost_reachable_state = this->findState(outermost_reachable_state_name);
  if (!outermost_reachable_state)
    {
      std::cout << "ERROR: Machine::addFork, adding fork compound transition \"" << *(in_fork->name()) <<
	"\" failed." << std::endl;
      std::cout << "Can't retrieve outermost reachable state \"" << *outermost_reachable_state_name <<
	"\"." << std::endl;
      return false;
    }
  if (!outermost_reachable_state->checkForkOrJoin(in_fork->reachableStatesNames(), true))
    {
      std::cout << "ERROR: Machine::addFork, bad outgoing transitions specification for the fork compound transition \"" <<
	*(in_fork->name()) << "\" ." << std::endl;
      return false;
    }
  auto starting_state_name = in_fork->startingState(0);
  auto starting_state = this->findState(starting_state_name);
  if (!starting_state)
    {
      std::cout << "ERROR: Machine::addFork, adding fork compound transition \"" << *(in_fork->name()) <<
	"\" failed." << std::endl;
      std::cout << "Can't retrieve starting state \"" << *starting_state_name << "\"." << std::endl;
      return false;
    }    
  return starting_state->addTransition(in_fork);
}

// -----------------------------------------------------------------------------------
bool Machine::addSubmachine(const char *in_region_name, Machine &in_machine)
{
  RegionsComponent regions_component = in_machine.regionsComponent();
  auto state = std::make_shared<CompositeState>(in_machine.name()->c_str(), regions_component);
  if (!this->addState(in_region_name, state))
    {
      std::cout << "ERROR: Machine::addSubmachine, adding submachine failed." << std::endl;
      return false;
    }
  return true;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Region> Machine::findRegion(std::shared_ptr<std::string> in_region_name) const
{
  auto region = this->RegionsComponent::findRegion(in_region_name);  

#ifdef WARNING
  if (!region)
    std::cout << "WARNING: Machine::findRegion, region \"" << *in_region_name << "\" not found." << std::endl;
#endif

  return region; 
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> Machine::findState(std::shared_ptr<std::string> in_state_name) const
{
  auto state = this->RegionsComponent::findState(in_state_name);
  
#ifdef WARNING
  if (!state)
    std::cout << "WARNING: Machine::findState, state \"" << *in_state_name << "\" not found." << std::endl;
#endif
  
  return state;
}

