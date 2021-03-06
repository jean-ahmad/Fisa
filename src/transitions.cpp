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


using namespace fisa;

//#######################################################################################
/*
  Event
*/

// -----------------------------------------------------------------------------------
Event::~Event() {}

//#######################################################################################
/*
  TimeEvent
*/

// -----------------------------------------------------------------------------------
TimeEvent::TimeEvent()
{
  this->_isAfter = false;
  this->_dateTime = nullptr;
  this->_when = nullptr;
  this->_exceeding = nullptr;
}

// -----------------------------------------------------------------------------------
TimeEvent::~TimeEvent()
{
  
}

// -----------------------------------------------------------------------------------
void TimeEvent::after(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding)
{
  this->_isAfter = true;
  this->_dateTime = in_date_time;
  this->_exceeding = in_exceeding;
}

// -----------------------------------------------------------------------------------
void TimeEvent::at(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding)
{
  this->_isAfter = false;
  this->_dateTime = in_date_time;
  this->_exceeding = in_exceeding;
}

// -----------------------------------------------------------------------------------
bool TimeEvent::init()
{

#if defined OPENSOURCE_PLATFORM_TIME || defined WINDOWS_PLATFORM_TIME
  if (this->_isAfter)
    { 
#ifdef OPENSOURCE_PLATFORM_TIME
      this->_when = std::make_shared<DateTime>(OpenSourceTime::now() + *this->_dateTime);
#elif WINDOWS_PLATFORM_TIME
      this->_when = std::make_shared<DateTime>(WindowsTime::now() + *this->_dateTime);
#endif
      return true;
    }
  else
    {
      this->_when = this->_dateTime;  
      return true;
    }
#else
  std::cout << "ERROR: TimeEvent::init, time not supported." << std::endl;
  return false;
#endif
}

// -----------------------------------------------------------------------------------
bool TimeEvent::happened() const
{
#if defined OPENSOURCE_PLATFORM_TIME || defined WINDOWS_PLATFORM_TIME

#ifdef OPENSOURCE_PLATFORM_TIME
  DateTime now(OpenSourceTime::now());
#elif WINDOWS_PLATFORM_TIME
  DateTime now(WindowsTime::now());
#endif

  if ((*this->_when <= now) && (now <= *this->_when + *this->_exceeding))
    {
#ifdef DEBUG
      std::cout << "DEBUG: TimeEvent::happened, now is " << now.toIso8601() << " and when is " << this->_when->toIso8601() <<
	"." << std::endl;
#endif
      return true;
    }
  else return false;
#else 
  std::cout << "ERROR: TimeEvent::init, time not supported." << std::endl;
  return false;
#endif
}


//#######################################################################################
/*
  Transition 
*/

// -----------------------------------------------------------------------------------
Transition::Transition(const char *in_transition_name, const char *in_starting_state_name, const char *in_reachable_state_name)
{
  this->_transitionName = std::make_shared<std::string>(in_transition_name);
  this->_startingStateName = std::make_shared<std::string>(in_starting_state_name);
  this->_reachableStateName = std::make_shared<std::string>(in_reachable_state_name);
  this->_trigger = nullptr;
}

// -----------------------------------------------------------------------------------
Transition::~Transition() {}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Transition::name() const
{
  return this->_transitionName;
}

// -----------------------------------------------------------------------------------
void Transition::setTrigger(std::shared_ptr<Event> in_trigger)
{
  this->_trigger = in_trigger;
}

// -----------------------------------------------------------------------------------
bool Transition::init()
{
  if (!this->_trigger)
    {
#ifdef WARNING
      std::cout << "WARNING: Transition::init, transition \"" << *this->_transitionName << "\" doesn't have any trigger." <<
	std::endl;
#endif
      return true;
    }
  else return this->_trigger->init();
}

// -----------------------------------------------------------------------------------
int Transition::startingStates() const
{
  return 1;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Transition::startingState(int in_state_index) const
{
  return this->_startingStateName;
}

// -----------------------------------------------------------------------------------
int Transition::reachableStates() const
{
  return 1;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Transition::reachableState(int in_state_index) const
{
  return this->_reachableStateName;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::vector<std::string> > Transition::reachableStatesNames() const
{
#ifdef WARNING
  std::cout << "WARNING: Transition::reachableStatesNames, transition \"" << *this->_transitionName <<
    "\", method is not defined." << std::endl;
#endif
  return nullptr;
}

// -----------------------------------------------------------------------------------
void Transition::effect() const
{
#ifdef DEBUG
  std::cout << "DEBUG: Transition::effect, transition \"" << *this->_transitionName << "\"." << std::endl;
#endif
}

// -----------------------------------------------------------------------------------
bool Transition::isTriggered() const
{
  if (this->_trigger) return true;
  else return false;
}

// -----------------------------------------------------------------------------------
bool Transition::isActivated() const
{
  if (!this->_trigger)
    {
#ifdef WARNING
      std::cout << "WARNING: Transition::isActivated, transition \"" << *this->_transitionName <<
	"\" doesn't have any trigger." << std::endl;
#endif
      return true;
    }
  else if (this->_trigger->happened()) return true;
  else return false;
}

//#########################################################################################################
/*
  JoinIncoming
*/
   
JoinIncoming::JoinIncoming(const char *in_starting_state_name)
{
  this->_startingStateName = std::make_shared<std::string>(in_starting_state_name);
}
    
JoinIncoming::~JoinIncoming()
{
}

std::shared_ptr<std::string> JoinIncoming::startingState()
{
  return this->_startingStateName;
}
    
void JoinIncoming::effect()
{
}

//#######################################################################################
/*
  Join
*/

// -----------------------------------------------------------------------------------
Join::Join(const char *in_join_name, const char *in_reachable_state_name) :
  Transition(in_join_name, "", in_reachable_state_name)
{
}

// -----------------------------------------------------------------------------------
void Join::addIncoming(std::shared_ptr<JoinIncoming> in_incoming_transition)
{
  this->_incomingTransitions.push_back(in_incoming_transition);
}

// -----------------------------------------------------------------------------------
int Join::startingStates() const
{
  return this->_incomingTransitions.size();
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::vector<std::string> > Join::startingStatesNames() const
{
  auto starting_states_names = std::make_shared<std::vector<std::string> >();
  for (auto it = this->_incomingTransitions.begin(); it != this->_incomingTransitions.end(); it++)
    starting_states_names->push_back(*((*it)->startingState()));
  
  return starting_states_names;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Join::startingState(int in_state_index) const
{
  return this->_incomingTransitions[in_state_index]->startingState();
}

// -----------------------------------------------------------------------------------
void Join::effect() const
{
#ifdef DEBUG
  std::cout << "DEBUG: Join::effect, join transition \"" << *this->_transitionName << "\"." << std::endl;
#endif
  
  for (auto it = this->_incomingTransitions.begin(); it != this->_incomingTransitions.end(); it++)
    (*it)->effect();
}

//#########################################################################################################
/*
  ForkOutgoing
*/
   
ForkOutgoing::ForkOutgoing(const char *in_reachable_state_name)
{
  this->_reachableStateName = std::make_shared<std::string>(in_reachable_state_name);
}
    
ForkOutgoing::~ForkOutgoing()
{
}

std::shared_ptr<std::string> ForkOutgoing::reachableState()
{
  return this->_reachableStateName;
}
    
void ForkOutgoing::effect()
{
}

//#######################################################################################
/*
  Fork
*/

// -----------------------------------------------------------------------------------
Fork::Fork(const char *in_fork_name, const char *in_starting_state_name) :
  Transition(in_fork_name, in_starting_state_name, "")
{
}

// -----------------------------------------------------------------------------------
void Fork::addOutgoing(std::shared_ptr<ForkOutgoing> in_outgoing_transition)
{
  this->_outgoingTransitions.push_back(in_outgoing_transition);
}

// -----------------------------------------------------------------------------------
int Fork::reachableStates() const
{
  return this->_outgoingTransitions.size();
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::vector<std::string> > Fork::reachableStatesNames() const
{
  auto reachable_states_names = std::make_shared<std::vector<std::string> >();
  for (auto it = this->_outgoingTransitions.begin(); it != this->_outgoingTransitions.end(); it++)
    reachable_states_names->push_back(*((*it)->reachableState()));
  
  return reachable_states_names;
}

// -----------------------------------------------------------------------------------
std::shared_ptr<std::string> Fork::reachableState(int in_state_index) const
{
  return this->_outgoingTransitions[in_state_index]->reachableState();
}

// -----------------------------------------------------------------------------------
void Fork::effect() const
{
#ifdef DEBUG
  std::cout << "DEBUG: Fork::effect, fork transition \"" << *this->_transitionName << "\"." << std::endl;
#endif
  
  for (auto it = this->_outgoingTransitions.begin(); it != this->_outgoingTransitions.end(); it++)
    (*it)->effect();
}


