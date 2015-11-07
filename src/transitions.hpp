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

#ifdef GCCTIME_SUPPORT
#include <sys/time.h>
#endif

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

    //! \private
    virtual ~ChangeEvent() {}

    //! Switching the value of an attribute.
    bool switching(const char *in_attribute_name, const T in_attribute_value)
    {
      std::string attribute_name(in_attribute_name);
      typename std::map<std::string, T>::iterator it = this->_attributes.find(attribute_name);
      if (it != this->_attributes.end())
	{
	  (*it).second = in_attribute_value;
	  return true;
	}
      else
	{
	  std::cout << "ERROR: ChangeEvent::switching, cannot retrieve attribute \"" << attribute_name <<
	    "\"." << std::endl;
	  return false;
	}
    }

    //! Inherited from Event class.
    bool init() {return true;}
    
    //! Inherited from Event class.
    virtual bool happened() const = 0;

  protected:
    //! Adding an attribute and his initial value.
    void add(const char *in_attribute_name, const T in_initial_value)
    {  
      this->_attributes[std::string(in_attribute_name)] = in_initial_value;
    }
    
    //! Returns the attribute value.
    T value(const char *in_attribute_name) const
    {
      std::string attribute_name(in_attribute_name);
      typename std::map<std::string, T>::const_iterator it = this->_attributes.find(attribute_name);
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
   *  Currently, only supported by the GCC compiler.
   **/

  class TimeEvent : public Event
  {
  public:
    //! Constructor.
    TimeEvent();

    //! \private
    ~TimeEvent();

    //! Sets the time duration until the event happen.
    /**
     * The "in_exceeding" parameter allow to define a margin within which the event is valid.
     **/
    void after(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding);

    //! Sets the absolute time until the event happen.
    /**
     * The "in_exceeding" parameter allow to define a margin within which the event is valid.
     **/
    void at(std::shared_ptr<DateTime> in_date_time, std::shared_ptr<DateTime> in_exceeding);

    //! Inherited from Event class.
    bool init();

    //! Inherited from Event class.
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
  //! Class to program a transition between two states of the machine.
  /** 
   * When a transition is fired the "effect" method is called after the "exit" method of the starting 
   * state and before the "entry" method of the reached state. Use the "setTrigger" method to define 
   * a trigger. If no trigger is set, the transition is always activated. Transition from an "InitialState" 
   * state must no be triggered. The class should be inherited and the "effect" method overloaded. 
  **/

  class Transition
  {
  public:
    //! Constructor.
    Transition(const char *_in_transitoin_name, const char *in_starting_state_name, const char *in_reachable_state_name);

    //! \private
    virtual ~Transition();

    //! \private
    std::shared_ptr<std::string> name() const;

    //! Sets the transition trigger.
    void setTrigger(const std::shared_ptr<Event> in_trigger);

    //! \private
    bool init();

    //! \private
    virtual int startingStates() const;

    //! \private
    virtual std::shared_ptr<std::string> startingState(int in_state_index) const;

    //! \private
    virtual int reachableStates() const;

    //! \private
    virtual std::shared_ptr<std::string> reachableState(int in_state_index) const;

    //! \private
    virtual std::shared_ptr<std::vector<std::string> > reachableStatesNames() const;

    //! Executed when the transition is fired.
    /** Should be overloaded **/
    virtual void effect() const;
    
    //! \private
    bool isTriggered() const;

    //! \private
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
  //! Class to program an incoming part of a join transition.
  /**
   * When the join transition is fired, each method "effect" of incoming parts are called.
   * The class should ne inherited and the "effect" method overloaded. 
   **/

  class JoinIncoming
  {
  public:
    //! Constructor.
    JoinIncoming(const char *in_starting_state_name);

    //! \private
    virtual ~JoinIncoming();

    //! \private
    std::shared_ptr<std::string> startingState();

    //! Executed when the join transition is fired.
    /** This method should be overloaded. **/
    virtual void effect();

  private:
    std::shared_ptr<std::string> _startingStateName;
    };
  

  //#########################################################################################################
  /*
    JoinTransition    
  */
  //! Class to program a transition between states in a composite state and a state.
  /**
   * When a state in a region of a composite state take part to incomings, all others regions of the
   * composite state must have a state taking part to incomings.
   **/

  class JoinTransition : public Transition
  {
  public:
    //! Constructor.
    JoinTransition(const char *in_join_name, const char *in_reachable_state_name);

    //! Adding an incoming part to the join transition.
    void addIncoming(std::shared_ptr<JoinIncoming> in_incoming_transition);

    //! \private
     int startingStates() const;

    //! \private
    std::shared_ptr<std::vector<std::string> > startingStatesNames() const;

    //! \private
    std::shared_ptr<std::string> startingState(int in_state_index) const;

    //! Calls each method "effect" of incoming parts.
    void effect() const;

  private:
    std::vector<std::shared_ptr<JoinIncoming> > _incomingTransitions;
    std::shared_ptr<std::string> _reachableStateName;
  };

  //#########################################################################################################
  /*
    ForkOutgoing
  */
  //! Class to program an outgoing part of a fork transition.
  /**
   * Each method "effect" of incoming parts are called when the fork transition is fired.
   * The class should be inherited and the "effect" method overloaded.
   **/

  class ForkOutgoing
  {
  public:
    //! Constructor
    ForkOutgoing(const char *in_reachable_state_name);

    //! \private
    virtual ~ForkOutgoing();

    //! \private
    std::shared_ptr<std::string> reachableState();

    //! Executed when the fork transition is fired.
    /** This method should be overloaded. **/
    virtual void effect();

  private:
    std::shared_ptr<std::string> _reachableStateName;
    };
  
  //#########################################################################################################
  /*
    ForkTransition    
  */
  //! Class to program a transition from a state and states in a composite state.
  /**
   * When a state in region of a composite state take part to outgoings, all others regions of the
   * composite state must have a state taking part to outgoings.
   **/

  class ForkTransition : public Transition
  {
  public:
    //! Constructor.
    ForkTransition(const char *in_fork_name, const char *in_starting_state_name);

    //! Adding an outgoing part in the fork transition.
    void addOutgoing(std::shared_ptr<ForkOutgoing> in_outgoing_transition);

    //! \private
     int reachableStates() const;

    //! \private
    std::shared_ptr<std::vector<std::string> > reachableStatesNames() const;
    
    //! \private
    std::shared_ptr<std::string> reachableState(int in_state_index) const;

    //! Calls each method "effect" of outgoing parts.
    void effect() const;

  private:
    std::vector<std::shared_ptr<ForkOutgoing> > _outgoingTransitions;
    std::shared_ptr<std::string> _startingStateName;
  };  
}

#endif
