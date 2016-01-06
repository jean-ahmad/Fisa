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


#ifndef TRANSITIONS_HPP
#define TRANSITIONS_HPP

#include "datetime.hpp"

#include <vector>
#include <map>
#include <string>
#include <memory>

#include <iostream>

namespace fisa
{ 
  //#######################################################################################
  /*  
      Event
  */
  //! Abstract class that must be inherited by an event that trigger a transition.

  class Event
  {
  public:
    //! \private
    virtual ~Event();

    //! Initializes the triggering conditions.  
    /** 
     * An event class that trigger a transition must implement this method.
     **/
    virtual bool init() = 0;
    
    //! Returns triggering conditions fulfilment status. 
    /**
     * Transitions are checked asynchronously with this method.   
     * An event class that trigger a transition must implement this method.
     **/
    virtual bool happened() const = 0;
  };
  
  
  //#######################################################################################
  /*
    ChangeEvent
  */
  //! Class to implement a transition triggering by changes of attribute values.
  /**
   * The class should be inherited and the "happened" method overloaded to implement the triggering conditions.
   **/

  template<typename T>
  class ChangeEvent : public Event
  {
  public:
    //! Constructor.
    ChangeEvent() {}

    //! Destructor.
    virtual ~ChangeEvent() {}

    //! Switching the value of the attribute with name specified in first input argument.
    bool switching(const char *in_attribute_name, const T in_attribute_value)
    {
      std::string attribute_name(in_attribute_name);
      auto it = this->_attributes.find(attribute_name);
      if (it != this->_attributes.end())
	{
	  (*it).second = in_attribute_value;
	  return true;
	}
      else
	{
	  std::cout << "ERROR: ChangeEvent::switching, attribute \"" << attribute_name <<
	    "\" not found." << std::endl;
	  return false;
	}
    }

    //! Specializes Event's "init" method.
    bool init() {return true;}
    
    //! Specializes Event's "happened" method.
    virtual bool happened() const = 0;

  protected:
    //! Adding an attribute with name specified in argument and his initial value.
    void add(const char *in_attribute_name, const T in_initial_value)
    {  
      this->_attributes[std::string(in_attribute_name)] = in_initial_value;
    }
    
    //! Returns the attribute value.
    T value(const char *in_attribute_name) const
    {
      std::string attribute_name(in_attribute_name);
      auto it = this->_attributes.find(attribute_name);
      if (it == this->_attributes.end())
	{
	  std::cout << "ERROR: ChangeEvent::status, attribute \"" << attribute_name <<
	    "\" does not exist." << std::endl;
	  // TODO (throw an exception)
	  return T();
	}
      return (*it).second;
    }

    std::map<std::string, T> _attributes; // {name, value}
  };

  //#######################################################################################
  /*
    TimeEvent
  */
  //! Class to implement a transition triggering by the passing of a time duration or the reaching of an absolute time.
  /**
   * Supported on Open-source and Windows platforms
   **/

  class TimeEvent : public Event
  {
  public:
    //! Constructor.
    TimeEvent();

    //! Destructor.
    ~TimeEvent();

    //! Sets the time duration until the triggering of the event.
    /**
     * The "in_exceeding" parameter allows to define an interval within which the event is 
     * considered as triggered.
     **/
    void after(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding);

    //! Sets the absolute time at which the event will trigger.
    /**
     * The "in_exceeding" parameter allows to define an interval within which the event is 
     * considered as triggered.
     **/
    void at(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding);

    //! Specializes Event's "init" method.
    bool init();

    //! Specializes Event's "happened" method.
    bool happened() const;
    
  private:
    std::shared_ptr<DateTime> _dateTime;
    std::shared_ptr<DateTime> _when;
    std::shared_ptr<DateTime> _exceeding;
    bool _isAfter;
  };

  //#########################################################################################################
  /*
    Transition
  */
  //! Class to program a transition between two states in a region of a machine.
  /** 
   * To program an automata, only the constructor, the overloadable method "effect" and 
   * the method "setTrigger" should be used.
   * A transition is activated by the triggering of an event associated with it.
   * The "setTrigger" method should be used to set the triggering Event. If no 
   * trigger has been defined, the transition is always activated.
   * The overloadable method "effect" is called when the machine has fired the transition 
   * and changes of state.
   **/

  class Transition
  {
  public:
    //! Constructor.
    /**
     * First parameter: the name of the transition.
     * Second parameter: the name of the starting state.
     * Third parameter: the name of the reachable state.
     **/
    Transition(const char *_in_transition_name, const char *in_starting_state_name, const char *in_reachable_state_name);

    //! Destructor.
    virtual ~Transition();

    //! Retrurns the name of the transition.
    std::shared_ptr<std::string> name() const;

    //! Sets the transition's triggering Event.
    void setTrigger(const std::shared_ptr<Event> in_trigger);

    //! Initializes the trigger.
    bool init();

    //! Returns the number of states that start from the transition.
    /**
     * This method is specialized by composite transitions with multiple starting states.
     **/
    virtual int startingStates() const;

    //! Returns the name of the starting state at the index specified in input argument.
    /** 
     * This method is specialized by composite transitions with multiple starting states.
     **/
    virtual std::shared_ptr<std::string> startingState(int in_state_index) const;

    //! Returns the number of states that are reachable by the transition.
    /**
     * The method is specialized by composite transitions with multiple reachable states.
     **/
    virtual int reachableStates() const;
    
    //! Returns the name of the reachable state at the index specified in input argument.
    /**
     * The method is specialized by composite transitions with multiple reachable states.
     **/
    virtual std::shared_ptr<std::string> reachableState(int in_state_index) const;

    //! Returns the names of the reachable states.
    virtual std::shared_ptr<std::vector<std::string> > reachableStatesNames() const;

    //! Overloadable method which is called when the transition is activated and the machine changes of state.
    virtual void effect() const;
    
    //! Asks if a triggering Event has been defined.
    bool isTriggered() const;

    //! Asks if the transition's Event has been triggered.
    bool isActivated() const;

  protected:
    std::shared_ptr<std::string> _transitionName;
    std::shared_ptr<Event> _trigger;

  private:    
    std::shared_ptr<std::string> _startingStateName;
    std::shared_ptr<std::string> _reachableStateName;
  };

  //#########################################################################################################
  /*
    JoinIncoming
  */
  //! Class to program an incoming path of a join composite transition with multiple starting states.
  /**
   * To program automata, only the constructor and the overloadable method "effect" should be used.
   * When a join composite transition is activated, each method "effect" of the incoming paths are called.
   **/

  class JoinIncoming
  {
  public:
    //! Construct and incoming path with the name of the starting state specified in input argument.
    JoinIncoming(const char *in_starting_state_name);

    //! Destructor.
    virtual ~JoinIncoming();

    //! Returns the name of the state starting from the incoming path.
    std::shared_ptr<std::string> startingState();

    //! Overloadable method executed the the join composite transition that own the incoming is activated and the machine changes of state.
    virtual void effect();
    
  private:
    std::shared_ptr<std::string> _startingStateName;
  };
  
  
  //#########################################################################################################
  /*
    Join    
  */
  //! Class to program a pseudostate that act as a transition with multiple starting states and one reachable state.
  /**
   * To program automata, only the constructor and the "addIncoming" method should be used.
   * When a state in a region of a CompositeState takes part to incomings, all others regions of the
   * CompositeState must have a state that takes part to incomings.
   **/

  class Join : public Transition
  {
  public:
    //! Constructor.
    /**
     * First argument: the name of the join composite transition.
     * Second argument: the name of the reachable state.
     **/
    Join(const char *in_join_name, const char *in_reachable_state_name);

    //! Adding an incoming path (JoinIncoming) to the join composite transition.
    void addIncoming(std::shared_ptr<JoinIncoming> in_incoming_transition);

    //! Specializes Transition's "startingStates" method.
     int startingStates() const;

    //! Specializes Transition's "startingStatesNames" method.
    std::shared_ptr<std::vector<std::string> > startingStatesNames() const;

    //! Specializes Transition's "startingState" method.
    std::shared_ptr<std::string> startingState(int in_state_index) const;

    //! Calls each "effect" method of incoming paths.
    void effect() const;

  private:
    std::vector<std::shared_ptr<JoinIncoming> > _incomingTransitions;
    std::shared_ptr<std::string> _reachableStateName;
  };

  //#########################################################################################################
  /*
    ForkOutgoing
  */
  //! Class to program an outgoing path of a fork composite transition with multiple reachable states.
  /**
   * To program automata, only the constructor and the overloadable method "effect" should be used.
   * When a fork composite transition is activated, each method "effect" of the outgoing paths are called.
   **/

  class ForkOutgoing
  {
  public:
    //! Construct an outgoing path with the name of the reachable state specified in input argument.
    ForkOutgoing(const char *in_reachable_state_name);

    //! Destructor.
    virtual ~ForkOutgoing();

    //! Returns the name of the reachable state by the outgoind path.
    std::shared_ptr<std::string> reachableState();

    //! Executed when the fork transition is fired.
    /** This method should be overloaded. **/

    //! Overloadable method executed the the fork composite transition that own the outgoing is activated and the machine changes of state.
    virtual void effect();
    
  private:
    std::shared_ptr<std::string> _reachableStateName;
  };
  
  //#########################################################################################################
  /*
    Fork    
  */
  //! Class to program a pseudostate that act as a transition with one starting state and multiple reachable states.
  /**
   * To program automata, only the constructor and the "addOutgoing" method should be used.
   * When a state in a region of a CompositeState takes part to outgoings, all others regions of the
   * CompositeState must have a state that takes part to outgoings.
   **/

  class Fork : public Transition
  {
  public:
    //! Constructor.
    /**
     * First argument: the name of the fork composite transition.
     * Second argument: the name of the starting state.
     **/
    Fork(const char *in_fork_name, const char *in_starting_state_name);
    
    //! Adding an outgoing path to the fork composite transition.
    void addOutgoing(std::shared_ptr<ForkOutgoing> in_outgoing_transition);

    //! Specializes Transition's "reachableStates" method.
     int reachableStates() const;

    //! Specializes Transition's "reachableStatesNames" method.
    std::shared_ptr<std::vector<std::string> > reachableStatesNames() const;
    
    //! Specializes Transition's "reachableState" method.
    std::shared_ptr<std::string> reachableState(int in_state_index) const;

    //! Calls each method "effect" of outgoing paths.
    void effect() const;

  private:
    std::vector<std::shared_ptr<ForkOutgoing> > _outgoingTransitions;
    std::shared_ptr<std::string> _startingStateName;
  };  
}

#endif
