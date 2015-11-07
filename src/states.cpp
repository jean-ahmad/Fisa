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

#include "states.hpp"

using namespace fisa;

//#########################################################################################################
/*
  SimpleState
*/

// -----------------------------------------------------------------------------------
SimpleState::SimpleState(const char *in_state_name)
{
  this->_stateName = std::make_shared<std::string>(in_state_name);
}

// -----------------------------------------------------------------------------------
SimpleState::~SimpleState()
{
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> SimpleState::name() const
{
  return this->_stateName;
}

// -----------------------------------------------------------------------------------
void SimpleState::setOwningRegion(std::shared_ptr<std::string> in_region_name)
{
  this->_regionName =  in_region_name;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> SimpleState::owningRegion() const
{
  return this->_regionName;
}

// -----------------------------------------------------------------------------------
bool SimpleState::addTransition(std::shared_ptr<Transition> in_transition)
{
  this->_transitions.push_back(in_transition);
  return true;
}

// -----------------------------------------------------------------------------------
bool SimpleState::addJoin(std::shared_ptr<JoinTransition> in_join)
{
  this->_joinTransitions.push_back(in_join);
  return true;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Transition> SimpleState::fireTransition() const
{
#ifdef WARNING
  std::shared_ptr<Transition> fired_transition = nullptr;
#endif
  for (std::vector<std::shared_ptr<Transition> >::const_iterator it = this->_transitions.begin(); it != this->_transitions.end(); it++)
    {
#ifndef WARNING
      if ((*it)->isActivated())
	{
	  return *it;
	}
#else
      if (!fired_transition && (*it)->isActivated()) fired_transition = *it;
      else if (fired_transition && (*it)->isActivated())
	{
	  std::cout << "WARNING: SimpleState::fireTransition, state \"" << *this->_stateName <<
	    "\" had fired more than one transition \"" << *(fired_transition->name()) << "\" and \"" << 
	    *((*it)->name()) << "\"." << std::endl;
	}
#endif
    }
#ifndef WARNING
  return nullptr;
#else
  return fired_transition;
#endif
}

// -----------------------------------------------------------------------------------
bool SimpleState::init()
{
  for (std::vector<std::shared_ptr<Transition>>::iterator it = this->_transitions.begin(); it != this->_transitions.end(); it++)
    if (!(*it)->init())
      {
	std::cout << "ERROR: SimpleState::init, transition \"" << *((*it)->name()) << "\" can't be initialized." << std::endl;
	return false;
      }
    else
      {
#ifdef DEBUG
	std::cout << "DEBUG: SimpleState::init, transition \"" << *((*it)->name()) << "\" initialization." << std::endl;
#endif
      }
  this->entry();
  return true;
}

// -----------------------------------------------------------------------------------
bool SimpleState::initFork(std::shared_ptr<std::vector<std::string> > in_states_names)
{
  return false;
}

// -----------------------------------------------------------------------------------
bool SimpleState::finalize()
{
  this->exit();
  return true;
}

// -----------------------------------------------------------------------------------
bool SimpleState::run(RegionInfo &in_region_info)
{
  return true;
}

// -----------------------------------------------------------------------------------
void SimpleState::entry() const
{
#ifdef DEBUG
  std::cout << "DEBUG: SimpleState::entry, state \"" << *this->_stateName << "\"." << std::endl;
#endif
}

// -----------------------------------------------------------------------------------
void SimpleState::exit() const
{
#ifdef DEBUG
  std::cout << "DEBUG: SimpleState::exit, state \"" << *this->_stateName << "\"." << std::endl;
#endif
}

// -----------------------------------------------------------------------------------
bool SimpleState::isCompleted() const
{
  return true;
}

// -----------------------------------------------------------------------------------
void SimpleState::completed() const
{
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Region> SimpleState::findRegion(std::shared_ptr<std::string> in_region_name) const
{
  return nullptr;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> SimpleState::findState(std::shared_ptr<std::string> in_state_name) const
{
  return nullptr;
}

// -----------------------------------------------------------------------------------
bool SimpleState::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller) const
{
  std::vector<std::string>::const_iterator it = std::find(in_states_names->begin(), in_states_names->end(), *this->_stateName);
  if (it != in_states_names->end())
    return true;
  else
    return false;								      
}

//#########################################################################################################
/*
  InitialState
*/

// -----------------------------------------------------------------------------------
InitialState::InitialState(const char *in_state_name) : SimpleState(in_state_name)
{
}

// -----------------------------------------------------------------------------------
InitialState::~InitialState()
{
}

// -----------------------------------------------------------------------------------
bool InitialState::addTransition(std::shared_ptr<Transition> in_transition)
{
  if (this->SimpleState::_transitions.size() != 0)
    {
      std::cout << "ERROR: InitialState::addTransition, initial state \"" << *this->_stateName <<
	"\" has already one transition and can't add transition \"" << *(in_transition->name()) << "\"." << std::endl;
      return false;
    }
  else if (in_transition->isTriggered())
    {
      std::cout << "ERROR: InitialState::addTransition, initial state \"" << *this->_stateName <<
	" can't add triggered transition \"" << *(in_transition->name()) << "\"." << std::endl;
      return false;
    }
  else
    {
      this->SimpleState::addTransition(in_transition);
      return true;
    }
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Transition> InitialState::fireTransition() const
{
  return this->SimpleState::_transitions[0];
}

// -----------------------------------------------------------------------------------
bool InitialState::init()
{
  if (this->SimpleState::_transitions.size() != 1)
    {
      std::cout << "ERROR: InitialState::init, initial state \"" << *this->_stateName <<
	"\" has no transition." << std::endl;
      return false;
    }
  else return true;
}

// -----------------------------------------------------------------------------------
bool InitialState::finalize()
{
  return true;
}

// -----------------------------------------------------------------------------------
bool InitialState::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_name, bool in_is_caller) const
{
  return false;
}

//#########################################################################################################
/*
  FinalState
*/

// -----------------------------------------------------------------------------------
FinalState::FinalState(const char *in_state_name) : SimpleState(in_state_name)
{
}

// -----------------------------------------------------------------------------------
FinalState::~FinalState()
{
}

// -----------------------------------------------------------------------------------
bool FinalState::addTransition(std::shared_ptr<Transition> in_transition)
{
  std::cout << "ERROR: FinalState::addTransition, final state \"" <<  *this->_stateName <<
    "\" can't have any transition starting from it." << std::endl;
  return false;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Transition> FinalState::fireTransition() const
{
  return nullptr;
}

// -----------------------------------------------------------------------------------
bool FinalState::init()
{
  return true;
}

// -----------------------------------------------------------------------------------
bool FinalState::finalize()
{
  return true;
}

// -----------------------------------------------------------------------------------
bool FinalState::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller) const
{
  return false;
}

//#########################################################################################################
/*
  TerminateState
*/

// -----------------------------------------------------------------------------------
TerminateState::TerminateState(const char *in_state_name) : SimpleState(in_state_name)
{
}

// -----------------------------------------------------------------------------------
TerminateState::~TerminateState()
{
}

// -----------------------------------------------------------------------------------
bool TerminateState::addTransition(std::shared_ptr<Transition> in_transition)
{
  std::cout << "ERROR: TerminateState::addTransition, terminate pseudostate \"" <<  *this->_stateName <<
    "\" can't have any transition starting from it." << std::endl;
  return false;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Transition> TerminateState::fireTransition() const
{
  return nullptr;
}

// -----------------------------------------------------------------------------------
bool TerminateState::init()
{
  return true;
}

// -----------------------------------------------------------------------------------
bool TerminateState::finalize()
{
  return true;
}

// -----------------------------------------------------------------------------------
bool TerminateState::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller) const
{
  return false;
}

//#########################################################################################################
/*
  Region
*/

// -----------------------------------------------------------------------------------
Region::Region(const char *in_region_name)
{
  this->_regionName = std::make_shared<std::string>(in_region_name);
  this->_activeState = nullptr;
  this->_startingState = nullptr;
  this->_is_startingState = false;
}

// -----------------------------------------------------------------------------------
Region::~Region()
{
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Region::name() const
{
  return this->_regionName;
}

// -----------------------------------------------------------------------------------
void Region::addState(std::shared_ptr<SimpleState> in_state)
{
  if (std::dynamic_pointer_cast<InitialState>(in_state))
    {
      this->_startingState = in_state->name();
      this->_is_startingState = true;
    }
  this->_states.push_back(in_state);
}

// -----------------------------------------------------------------------------------
bool Region::init()
{
  if (!this->_activeState && this->_is_startingState)
    {
      this->_activeState = this->_startingState;
      std::shared_ptr<SimpleState> active_state = this->activeState();
      if (!active_state)
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName << "\" state \"" <<
	    *(active_state->name()) << "\" not founded." << std::endl;
	  return false;
	}
      std::shared_ptr<Transition> fired_transition = active_state->fireTransition();
      if (!fired_transition)
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName <<
	    "\" initial pseudostate \"" << *(active_state->name()) << "\" firing transition failed." << std::endl;
	  return false;
	}
      fired_transition->effect();
      if (fired_transition->reachableStates() == 1) this->_activeState = fired_transition->reachableState(0);
      else this->initFork(fired_transition->reachableStatesNames());
      active_state = this->activeState();
      if (!active_state)
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName << "\" can't retrieve reached state \"" <<
	    *this->_activeState << "\", firing  transition \"" << *(fired_transition->name()) << "\"." << std::endl;
	  return false;
	}
      if (!active_state->init())
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName << "\" state \"" <<
	    *this->_activeState << "\" initialization failed." << std::endl;
	  return false;
	}
      return true;
    }
  else if (this->_activeState)
    {
      std::shared_ptr<SimpleState> active_state = this->activeState();
      if (!active_state)
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName << "\" state \"" <<
	    *this->_activeState << "\" not founded." << std::endl;
	  return false;
	}
      if (!active_state->init())
	{
	  std::cout << "ERROR: Region::init, region \"" << *this->_regionName << "\" state \"" <<
	    *this->_activeState << "\" initialization failed." << std::endl;
	  return false;
	}
      return true;
    }
  else
    {
      std::cout << "ERROR: Region::init, region \"" << *this->_regionName <<
	"\" doesn't have an initial pseudostate." << std::endl;
      return false;
    }
}

// -----------------------------------------------------------------------------------
bool Region::initFork(std::shared_ptr<std::vector<std::string> > in_states_names)
{
  bool is_initfork = false;
  std::vector<std::string>::iterator jt = in_states_names->end();
  std::vector<std::shared_ptr<SimpleState> >::iterator it = this->_states.begin();  
  while ((it != this->_states.end()) && !is_initfork && (jt == in_states_names->end()))
    {      
      if ((*it)->initFork(in_states_names)) is_initfork = true;
      else
	{
	  jt = std::find(in_states_names->begin(), in_states_names->end(), *((*it)->name()));
	  if (jt == in_states_names->end()) it++;
	}
    }  
  if (is_initfork || jt != in_states_names->end())
    {      
      this->_activeState = (*it)->name();
#ifdef DEBUG
      std::cout << "DEBUG: Region::initFork, \"" << *this->_activeState << "\"." << std::endl;
#endif
      return true;
    }
  else return false;
}

// -----------------------------------------------------------------------------------
bool Region::finalize()
{
  if (!this->activeState()->finalize())
    {
      std::cout << "ERROR: Region::finalize, region \"" << *this->_regionName << "\" state \"" <<
	*this->_activeState << "\" finalization failed." << std::endl;
      return false;
    }
  this->_activeState = nullptr;
  return true;
}

// -----------------------------------------------------------------------------------
bool Region::run(RegionInfo &in_region_info)
{
  std::shared_ptr<SimpleState> active_state = this->activeState();
  if (!active_state->run(in_region_info))
    {
      std::cout << "ERROR: Region::run, region \"" << *this->_regionName << "\" state \"" <<
	*this->_activeState << "\" run failed." << std::endl;
      return false;
    }

  if (!in_region_info._transition_firing_allowed || in_region_info._is_terminated) return true;
  std::shared_ptr<Transition> fired_transition = active_state->fireTransition();
  if (!fired_transition) return true;
  if (!active_state->finalize())
    {
      std::cout << "ERROR: Region::run, region \"" << *this->_regionName << "\" state \"" <<
	*this->_activeState <<	"\" finalization failed." << std::endl;
      return false;
    }
  fired_transition->effect();
  if (fired_transition->reachableStates() == 1)
    {
#ifdef DEBUG
      std::cout << "DEBUG: Region::run, transition \"" << *(fired_transition->name()) << "\" fired." << std::endl;
#endif
      this->_activeState = fired_transition->reachableState(0);
    }
  else
    {
#ifdef DEBUG
      std::cout << "DEBUG: Region::run, fork transition fired." << std::endl;
#endif
      this->initFork(fired_transition->reachableStatesNames());
    }
  active_state = activeState();
  if (std::dynamic_pointer_cast<TerminateState>(active_state)) in_region_info._is_terminated = true;
  if (active_state)
    {
      if (!active_state->init())
	{
	  std::cout << "ERROR: Region::run, region \"" << *this->_regionName << "\" state \"" <<
	    *this->_activeState << "\" initialization failed." << std::endl;
	  return false;
	}
      in_region_info._transition_fired = true;
      return true;
    }
  else
    {
      std::cout << "ERROR: Region::run, region \"" << *this->_regionName << "\" can't retrieve reached state \"" <<
	*this->_activeState << "\", firing  transition \"" << *(fired_transition->name()) << "\"." << std::endl;
      return false;
    }
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> Region::activeState() const
{
  if (!this->_activeState) return nullptr;
  
  for (std::vector<std::shared_ptr<SimpleState> >::const_iterator it = this->_states.begin(); it != this->_states.end(); it++)
    {
      if (*((*it)->name()) == *this->_activeState) return *it;
    }
  
  std::cout << "ERROR: Region::activeState, region \"" << *this->_regionName << "\" state \"" <<
    *this->_activeState << "\" not founded." << std::endl;
  return nullptr;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Region> Region::findRegion(std::shared_ptr<std::string> in_region_name) const
{
  std::shared_ptr<Region> region;
  for (std::vector<std::shared_ptr<SimpleState> >::const_iterator it = this->_states.begin(); it != this->_states.end(); it++)
    {
      region = (*it)->findRegion(in_region_name);
      if (region) return region;
    }
  return nullptr;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> Region::findState(std::shared_ptr<std::string> in_state_name) const
{  
  std::shared_ptr<SimpleState> state;
  for (std::vector<std::shared_ptr<SimpleState> >::const_iterator it = this->_states.begin();
       it != this->_states.end(); it++)
    {
      if (*((*it)->name()) == *in_state_name) return *it;
      else
	{
	  state = (*it)->findState(in_state_name);
	  if (state) return state;
	}
    }
  return nullptr;
}

// -----------------------------------------------------------------------------------
bool Region::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller) const
{
  for (std::vector<std::shared_ptr<SimpleState> >::const_iterator it = this->_states.begin(); it != this->_states.end(); it++)
    if ((*it)->checkForkOrJoin(in_states_names)) return true;
  
  return false;
}

//#########################################################################################################
/*
  CompositeState
*/

// -----------------------------------------------------------------------------------
CompositeState::CompositeState(const char *in_state_name) : SimpleState(in_state_name)
{
}

// -----------------------------------------------------------------------------------
CompositeState::~CompositeState() 
{ 
}

// -----------------------------------------------------------------------------------
void CompositeState::newRegion(const char *in_region_name)
{
  std::shared_ptr<Region> new_region = std::make_shared<Region>(in_region_name);  
  this->_regions.push_back(new_region);
}

// -----------------------------------------------------------------------------------
bool CompositeState::init()
{
  this->SimpleState::init();
  
  for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    if (!(*it)->init())
      {
	std::cout << "ERROR: CompositeState::init, state \"" << *this->_stateName <<
	  "\" region \"" << *((*it)->name()) << "\" initialization failed." << std::endl;
	
	return false;
      }
  return true;
}

// -----------------------------------------------------------------------------------
bool CompositeState::initFork(std::shared_ptr<std::vector<std::string> > in_states_names)
{
  bool is_regions_initialized = true;
  for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    is_regions_initialized = is_regions_initialized && (*it)->initFork(in_states_names);
  if (is_regions_initialized) return true;
  else return false;
}

// -----------------------------------------------------------------------------------
bool CompositeState::finalize()
{
  this->SimpleState::finalize();
  
  for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    if (!(*it)->finalize())
      {
	std::cout << "ERROR: CompositeState::finalize, state \"" << *this->_stateName <<
	  "\" region \"" << *((*it)->name()) << "\" finalization failed." << std::endl;
	return false;
      }
  return true;
}

// -----------------------------------------------------------------------------------
bool CompositeState::run(RegionInfo &in_region_info)
{
  for (std::vector<std::shared_ptr<Region> >::iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      RegionInfo region_info;
      if (!(*it)->run(region_info))
	{
	  std::cout << "ERROR: CompositeState::run, state \"" << *this->_stateName <<
	    "\" run failed." << std::endl;
	  return false;
	}
      if (region_info._transition_fired || !region_info._transition_firing_allowed) in_region_info._transition_firing_allowed = false;
      if (region_info._is_terminated) in_region_info._is_terminated = true;
    }
  this->isCompleted();
  return true;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Transition> CompositeState::fireTransition() const
{
  std::shared_ptr<Transition> fired_transition = this->SimpleState::fireTransition();
#ifndef WARNING
  if (fired_transition) return fired_transition;
#endif

  bool is_join_ok;
  int i;
  for (std::vector<std::shared_ptr<JoinTransition> >::const_iterator it = this->SimpleState::_joinTransitions.begin();
       it != this->SimpleState::_joinTransitions.end(); it++)
    {
      if ((*it)->isActivated())
	{
	  is_join_ok = true;
	  i = 0;
	  while (i < (*it)->startingStates() && is_join_ok)
	    {
	      std::shared_ptr<SimpleState> state = this->findState((*it)->startingState(i));
	      if (*(this->findRegion(state->owningRegion())->activeState()->name()) != *(state->name()))
		is_join_ok = false;
	      i++;
	    }
#ifndef WARNING
	  if (is_join_ok) return *it;
#else
	  if (is_join_ok && !fired_transition) fired_transition = *it;
	  else if (is_join_ok && fired_transition)
	    {
	      std::cout << "WARNING: CompositeState::fireTransition(), state \"" << *this->_stateName <<
		"\" had fired more than one transition \"" << *(fired_transition->name()) << "\" and \"" <<
		*((*it)->name()) << "\"." << std::endl;
	    }
#endif
	}
    }  
  return fired_transition;
}

// -----------------------------------------------------------------------------------
bool CompositeState::isCompleted() const
{
  std::shared_ptr<SimpleState> active_state;
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      active_state = (*it)->activeState();
      if (!active_state)
	{
	  std::cout << "ERROR: CompositeState::isJoined, state \"" << *this->_stateName <<
	    "\" can't retrieve the active state of region \"" << *((*it)->name()) << "\"." << std::endl;
	}
      else if (!std::dynamic_pointer_cast<FinalState>(active_state)) return false;
    }
  this->completed();
  return true;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<Region> CompositeState::findRegion(std::shared_ptr<std::string> in_region_name) const
{
  std::shared_ptr<Region> region;
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      if ((*((*it)->name()) == *in_region_name)) return *it;
      else
	{
	  region = (*it)->findRegion(in_region_name);
	  if (region) return region;
	}
    }  
  return nullptr;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<SimpleState> CompositeState::findState(std::shared_ptr<std::string> in_state_name) const
{
  std::shared_ptr<SimpleState> state;
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      state = (*it)->findState(in_state_name);
      if (state) return state;
    }  
  return nullptr;
}

// -----------------------------------------------------------------------------------
bool CompositeState::checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller) const
{
  bool is_ok = true;
  for (std::vector<std::shared_ptr<Region> >::const_iterator it = this->_regions.begin(); it != this->_regions.end(); it++)
    {
      is_ok = (*it)->checkForkOrJoin(in_states_names) && is_ok;
      if (!is_ok && in_is_caller)
	{
	  std::cout << "ERROR: CompositeState::checkForkOrJoin, state \"" << *this->_stateName <<
	    "\" has incomings/outgoings missing inside region \"" << *((*it)->name()) << "\"." << std::endl;
	  return false;
	}
    }
  
  std::vector<std::string>::iterator it = std::find(in_states_names->begin(), in_states_names->end(), *this->_stateName);
  if (it != in_states_names->end())
    {
      if (is_ok)
	{
	  std::cout << "ERROR: CompositeState::checkForkOrJoin, state \"" << *this->_stateName <<
	    "\" has incomings/outgoings badly specified inside regions and in the state." << std::endl;
	  return false;
	}
      else return true;
    }
  else return is_ok;      
}
