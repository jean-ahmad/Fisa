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
#include <algorithm> // find
#include <string>
#include <memory> // shared_ptr
#include <utility> // move

#include <iostream>

namespace fisa
{
  //! Structure of data to collect information when the Machine's "run" method is called.
  typedef struct
  {
    void init()
    {
      this->_transition_fired = false;
      this->_transition_firing_allowed = true;
      this->_is_terminated = false;
    }
    
    bool _transition_fired;
    bool _transition_firing_allowed;
    bool _is_terminated;
  } RegionInfo;

  class Region;
  
  //#########################################################################################################
  /* 
     SimpleState
  */
  //! Class to program an irreducible state in a Region.
  /**
   * To create automata, only the constructor and the overloadable "entry" and "exit" 
   * methods should be used.
   * When a transition is fired from a state, the method "exit" of that state is called, 
   * followed by the method "effect" of the transition and, finally, the method "entry" 
   * of the reached state.
   **/

  class SimpleState
  {
  public:    
    //! Construct a SimpleState with name specified in argument.
    SimpleState(const char *in_state_name);

    //! Destructor.
    virtual ~SimpleState();

    //! Retrieves the name of the state.
    std::shared_ptr<std::string> name() const;

    //! Sets the name of the Region that own the state.
    void setOwningRegion(std::shared_ptr<std::string> in_region_name);
    
    //! Retrieves the name of the Region that own the state.
    std::shared_ptr<std::string> owningRegion() const;

    //! Adds a transition within the state.
    /** To create automata, the Machine's class "addTransition" method should be used. **/
    virtual bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Adds a "Join" transition within the state.
    /** To create automata, the Machine's class "addJoin" method should be used. **/
    bool addJoin(std::shared_ptr<Join> in_join);

    //! Checks if an event has triggered a transition and returns the fired transition.
    virtual std::shared_ptr<Transition> fireTransition() const;

    //! Called when the state is reached. Initializes transitions within the state and calls the "entry" method.
    virtual bool init();

    //! Nothing to do for a SimpleState.
    virtual bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names);

    //! Called when the state is leaved. Calls the "exit" method.
    virtual bool finalize();

    //! Nothing to do for a SimpleState. 
    virtual bool run(RegionInfo &io_region_info);

    //! Overloadable method called when the state is reached.
    /** Should be overloaded for SimpleState and CompositeState only. **/    
    virtual void entry() const;
    
    //! Overloadable method called when the state is leaved.
    /** Should be overloaded for SimpleState and CompositeState only. **/
    virtual void exit() const;

    //! Nothing to do for a SimpleState.
    virtual std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! Nothing to do for a SimpleState.
    virtual std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;

    //! Checks if the state takes part to a join or a fork transition.
    virtual bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;

    //! Checks the kind of the state. Returns true for "SimpleState" in input argument.
    virtual bool isKind(const char *in_kind) const;
    
  protected:    
    std::shared_ptr<std::string> _stateName;
    std::shared_ptr<std::string> _regionName;
    std::vector<std::shared_ptr<Transition> > _transitions;
    std::vector<std::shared_ptr<Join> > _joinPseudostates;
  };

  //#########################################################################################################
  /*
    InitialState
  */
  //! Class to program a pseudostate indicating the beginning of a region.
  /**
   * To create automata, only the constructor should be used.
   * Any regions, in the machine or in composite states, should contain an initial pseudostate. 
   * When a composite state is reached by a fork transition, it is not necessary that his regions
   * have an initial pseudostate defined.
   **/

  class InitialState : public SimpleState
  {
  public:
    //! Construct an initial pseudostate with name specified in input argument.
    InitialState(const char *name);

    //! Destructor.
    ~InitialState();

    //! An initial pseudostate must has just one non triggered transition.
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Returns the transition which starts from the pseudostate.
    std::shared_ptr<Transition> fireTransition() const;

    //! Nothing to do for an initial pseudostate.
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_name, bool in_is_caller = false) const;

    //! Returns true for "InitialState" in input argument. 
    bool isKind(const char *in_kind) const;
  };

  //#########################################################################################################
  /*
    FinalState
  */
  //! Class to program a pseudostate indicating that no more transitions can be fired within the region.
  /**
   * To create automata, only the constructor should be used. 
   **/
  
  class FinalState : public SimpleState
  {
  public:
    //! Construct a final pseudostate with name specified in input argument.
    FinalState(const char *name);

    //! Destructor.
    ~FinalState();
    
    //! A final pseudostate can't have any transition starting from it.
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Returns a null pointer since there is no starting transition from a final pseudostate.
    std::shared_ptr<Transition> fireTransition() const;

    //! Nothing to initialize for a final pseudostate.
    bool init();

    //! Nothing to finalize for a final pseudostate.
    bool finalize();

    //! Nothing to do for a final pseudostate.
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;

    //! Returns true for "FinalState" in input argument.
    bool isKind(const char *in_kind) const;
  };

  //#########################################################################################################
  /*
    TerminateState
  */
  //! Class to program a pseudostate indicating the termination of the machine execution.
  /** 
   * To create automata, only the constructor should be used.
   * When a terminate pseudostate is reached, no more transition can be fired within the 
   * machine, and so no more state changing can occur. 
   * In the last machine "run" execution when a terminate pseudostate is reached, 
   * all concurrent regions are allowed to fire a transition and to change of state.
   **/

  class TerminateState : public SimpleState
  {
  public:
    //! Construct a terminate pseudostate with name specified in input argument.
    TerminateState(const char *name);

    //! Destructor.
    ~TerminateState();

    //! A terminate peudostate mustn't have any transition starting from it.
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Returns a null pointer since there is no transition that starts from a terminate pseudostate.
    std::shared_ptr<Transition> fireTransition() const;

    //! Nothing to initialize for a terminate pseudostate.
    bool init();

    //! Nothing to finalize for a terminate pseudostate.
    bool finalize();

    //! Nothing to do for a terminate pseudostate.
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;

    //! Returns true for "TerminateState" in input argument.
    bool isKind(const char *in_kind) const;
  };

  //#########################################################################################################
  /*
    Region
  */
  //! A region is where states and transitions are programmed.
  /**
   * To add a region in the machine use Machine's "newRegion" method, and CompositeState's "newRegion" 
   * method to add a region in a composite state.
   **/

  class Region
  {
  public:    
    //! Construct a region with name specified in input argument.
    Region(const char *in_region_name);
  
    //! Destructor.
    ~Region();

    //! Retrieves the region's name.
    std::shared_ptr<std::string> name() const;

    //! Adds a state within the region.
    void addState(std::shared_ptr<SimpleState> in_state);

    //! Intialization with the InitialState or by calling the "init" method of the active state.
    bool init();

    //! Initializes the active state in regions that takes part to the fork transition.
    bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names);

    //! Calls finalize for the active state.
    bool finalize();

    //! Checks of an active state's activated transition and changes of state consequently.
    /**
     * Do the job for this region, for the regions inside composite states of this region, 
     * and so on recursively.
     **/
    bool run(RegionInfo &io_region_info);

    //! Returns the active state.
    std::shared_ptr<SimpleState> activeState() const;

    //! Returns the state that has the name specified in argument.
    /** The method searches only among states within this region. **/
    std::shared_ptr<SimpleState> findStateHere(std::shared_ptr<std::string> state_name) const;

    //! Returns the region that has the name specified in argument.
    /** 
     * The method searches among composite states within this region, composites states in 
     * composites states within this region and so on recursively.
     **/
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! Returns the state that has the name specified in argument.
    /**
     * The method searches among states within this region, states in composite states within 
     * this region and so on recursively.
     **/
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> state_name) const;

    //! Calls the method to check fork or join transition in all region's states.
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_names, bool in_is_caller = false) const;

  private:
    std::shared_ptr<std::string> _regionName;
    std::vector<std::shared_ptr<SimpleState> > _states;
    std::shared_ptr<SimpleState> _startingState;
    std::shared_ptr<SimpleState> _activeState;
  };

  //#########################################################################################################
  /*
    RegionsComponent
  */
  //! Container class of Region objects.
  /** 
   * This class is used to build "CompositeState" or "Machine" objects and, 
   * particularly, it is part of the design that allows to create sub-machines.
   **/
  
  class RegionsComponent
  {
  public:
    //! Constructor.
    RegionsComponent();
    
    //! Destructor.
    virtual ~RegionsComponent();
    
    //! Move constructor.
    RegionsComponent(RegionsComponent &&in_regions_component);

    //! Assignment move operator.
    RegionsComponent& operator = (RegionsComponent &&in_regions_component);
    
    //! Creates a new region which has the name specified in argument.
    virtual void newRegion(const char *in_region_name);
    
    //! Initializes all regions depending on the initial state defined inside them.
    virtual bool init();
    
    //! Changes the states in all regions depending on the transitions fired.
    virtual bool run(RegionInfo &io_region_info);
    
    //! Searches, in the regions of the container, and returns the region with the name specified in argument.
    virtual std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;
    
    //! Searches, in the regions of the container, and returns the state with the name specified in argument.
    virtual std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;    
    
  protected:
    std::vector<std::shared_ptr<Region> > _regions;
  };

  //#########################################################################################################
  /*
    CompositeState
  */
  //! Class to program a state in a region and that can have regions inside it.
  /** 
   * To create automata, only the constructors and the overloadable "entry", "exit" 
   * "completed" methods should be used.
   * When all regions of the composite state reach a final pseudostate, the overloadable 
   * method "completed" is called.
   **/

  class CompositeState : public SimpleState, public RegionsComponent
  {
  public:
    //! Constructor.
    /**
     * Construct a composite state with name specified in argument.
     **/
    CompositeState(const char *in_state_name);
    
    //! Constructor.
    /** 
     * Construct a composite state with name specified in argument and from regions 
     * inside the RegionsComponent.
     * The RegionsComponent object, specified in argument, is left empty after calling this constructor.
     **/
    CompositeState(const char *in_state_name, RegionsComponent &in_regions_component);

    //! Destructor.
    ~CompositeState();

    //! Specializes RegionsComponent's "newRegion" method.
    void newRegion(const char *in_region_name);

    //! Specializes RegionsComponent's "init" method.
    bool init();

    //! Initializes the active state inside regions of this state, with state names specified in argument.
    bool initFork(std::shared_ptr<std::vector<std::string> > in_states_names);

    //! Calls the finalize method of this state and for all state's regions.
    bool finalize();

    //! Specializes RegionsComponent's "run" method.
    bool run(RegionInfo &io_region_info);

    //! Specializes SimpleState's "fireTransition" method.
    std::shared_ptr<Transition> fireTransition() const;

    //! Calls the overloadable method "completed" if all state's regions have reached a FinalState.
    bool isCompleted() const;

    //! Overloadable method called when all regions in the state have reached a FinalState.
    virtual void completed() const;

    //! Specializes RegionsComponent's "findRegion" method.
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! Specializes RegionsComponent's "findState" method.
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;

    //! Specializes SimpleState's "checkForkOrJoin" method.
    bool checkForkOrJoin(std::shared_ptr<std::vector<std::string> > in_states_name, bool in_is_caller = false) const;

    //! Returns true for "CompositeState" in input argument.
    bool isKind(const char *in_kind) const;
  };
}

#endif
