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

#ifndef STATES_HPP
#define STATES_HPP

#include "transitions.hpp"

#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <memory>

#include <iostream>

namespace fisa
{
  //! \private
  typedef struct
  {
    bool _transition_fired = false;
    bool _transition_firing_allowed = true;
    bool _is_terminated = false;
  } RegionInfo;
  
  //#########################################################################################################
  /* 
     SimpleState
  */
  //! Class to program an irreducible state in a region.
  /**
   * The method "entry" is called when a transition reaching the state is fired after the 
   * method "effect" of the transition. The method "exit" is called when a transition 
   * starting from the state is fired before the method "effect" of the transition.
   * The class should be inherited and the "entry" and "exit" methods overloaded.
   **/

  class Region;

  class SimpleState
  {
  public:    
    //! Constructor.
    SimpleState(const char *in_state_name);

    //! \private
    virtual ~SimpleState();

    //! \private
    std::shared_ptr<std::string> name() const;

    //! \private
    void setOwningRegion(std::shared_ptr<std::string> in_region_name);

    //! \private
    std::shared_ptr<std::string> owningRegion() const;
    
    //! \private
    virtual bool addTransition(std::shared_ptr<Transition> in_transition); // polymorphic

    //! \private
    bool addJoin(std::shared_ptr<JoinTransition> in_join);

    //! \private
    virtual std::shared_ptr<Transition> fireTransition() const; // polymorphic

    //! \private
    virtual bool init(); // polymorphic

    //! \private
    virtual bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names); // polymorphic

    //! \private
    virtual bool finalize(); // polymorphic

    //! \private
    virtual bool run(RegionInfo &in_region_info); // polymorphic
    
    //! Called when a transition reaching the state is fired after the method "effect" of the transition.
    /** This method should be overloaded for "SimpleState" states or "CompositeState" states only. **/    
    virtual void entry() const; // polymorphic for users
    
    //! Called when a transition starting from the state is fired before the method "effect" of the transition.
    /** This method should be overloaded for "SimpleState" states or "CompositeState" states only. **/
    virtual void exit() const; // polymorphic for users

    //! \private
    virtual bool isCompleted() const; // polymorphic

    //! Called when all regions in a composite state have reached a final pseudostate.
    /** This method should be overloaded for "CompositeState" states only. **/
    virtual void completed() const;

    //! \private
    virtual std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const; // polymorphic

    //! \private
    virtual std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const; // polymorphic

    //! \private
    virtual bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;
   
  protected:
    // Protected data
    std::shared_ptr<std::string> _stateName;
    std::shared_ptr<std::string> _regionName;
    std::vector<std::shared_ptr<Transition> > _transitions;
    std::vector<std::shared_ptr<JoinTransition> > _joinTransitions;
  };

  //#########################################################################################################
  /*
    InitialState
  */
  //! Class to program a pseudostate indicating the beginning of a region.
  /**
   * Any regions, in the machine or in "CompositeState" states, should contain an initial pseudostate. 
   * It is not necessary that a "CompositeState" state reached by a "ForkTransition" transition haves 
   * "InitialState" pseudostates in its regions.
   **/

  class InitialState : public SimpleState
  {
  public:
    //! Constructor.
    InitialState(const char *name);

    //! \private
    ~InitialState();

    //! \private
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! \private
    std::shared_ptr<Transition> fireTransition() const;

    //! \private
    bool init();

    //! \private
    bool finalize(); 

    //! \private
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_name, bool in_is_caller = false) const;
  };

  //#########################################################################################################
  /*
    FinalState
  */
  //! Class to program a pseudostate indicating the ending of a region.
  /** 
   * When a region reach a "FinalState" pseudostate, no more transition can be fired within the 
   * region. 
   **/

  class FinalState : public SimpleState
  {
  public:
    //! Constructor
    FinalState(const char *name);

    //! \private
    ~FinalState();

    //! \private
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! \private
    std::shared_ptr<Transition> fireTransition() const;

    //! \private
    bool init();

    //! \private
    bool finalize();

    //!private
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;
  };

  //#########################################################################################################
  /*
    TerminateState
  */
  //! Class to program a pseudostate indicating the termination of the machine execution.
  /** 
   * When a "TerminateState" pseudostate is reached, no more transition can be fired within the 
   * machine. In the last "Machine::run" execution when a terminate pseudostate is reached, 
   * all concurrent regions are allowed to fire a transition and to change of state.
   **/

  class TerminateState : public SimpleState
  {
  public:
    //! Constructor
    TerminateState(const char *name);

    //! \private
    ~TerminateState();

    //! \private
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! \private
    std::shared_ptr<Transition> fireTransition() const;

    //! \private
    bool init();

    //! \private
    bool finalize();

    //!private
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;
  };

  //#########################################################################################################
  /*
    Region
  */
  //! \private Class to program a region in the machine or in a composite state.
  /**
   * To add a region in the machine use "Machine::newRegion" and to add a
   * region in a composite state use "Composite::newRegion".
   **/

  class Region
  {
  public:    
    //! Constructors.
    Region(const char *in_region_name);
  
    //! \private
    ~Region();

    //! \private
    std::shared_ptr<std::string> name() const;

    //! \private
    void addState(std::shared_ptr<SimpleState> in_state);

    //! \private
    bool init();

    //! \private
    bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names);

    //! \private
    bool finalize();

    //! \private
    bool run(RegionInfo &in_region_info);

    //! \private
    std::shared_ptr<SimpleState> activeState() const;

    //! \private
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! \private
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> state_name) const;

    //! \private
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;

  private:
    std::shared_ptr<std::string> _regionName;
    std::vector<std::shared_ptr<SimpleState> > _states;
    bool _is_startingState;
    std::shared_ptr<std::string> _startingState;
    std::shared_ptr<std::string> _activeState;  
  };

  //#########################################################################################################
  /*
    CompositeState
  */
  //! Class to program a state in a region and that can have regions inside it.
  /** 
   * When all regions in the composite state reach a "FinalState" pseudostate, the overloadable 
   * method "completed" is called.
   * The method "entry" is called when a transition reaching the composite state is fired after the method 
   * "effect" of the transition. The method "exit" is called when a transition starting from the composite 
   * state is fired before the method "effect" of the transition.
   * The class should be inherited and the "entry" and "exit" methods overloaded.
   **/

  class CompositeState : public SimpleState
  {
  public:
    //! Constructor
    CompositeState(const char *in_state_name);

    //! \private
    virtual ~CompositeState();

    //! Adding a new region within the composite state.
    void newRegion(const char *in_region_name);

    //! \private
    bool init();

    //! \private
    bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names);

    //! \private
    bool finalize();

    //! \private
    bool run(RegionInfo &in_region_info);

    //! \private
    std::shared_ptr<Transition> fireTransition() const;

    //! \private
    bool isCompleted() const;

    //! \private
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! \private
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;

    //! \private
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_name, bool in_is_caller = false) const;
    
  private:
    std::vector<std::shared_ptr<Region> > _regions;
  };
}

#endif
