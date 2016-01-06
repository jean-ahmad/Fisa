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

#ifndef MACHINE_HPP
#define MACHINE_HPP

#include "transitions.hpp"
#include "states.hpp"

#include <utility> // move
#include <memory>

namespace fisa
{
  //#########################################################################################################
  /* 
     Machine
  */
  //! Class to build and simulate automata.
  /**
   * A machine is made of many regions which contain states and transitions.
   * The class must be inherited and the method "build" overloaded to create automata. 
   * To program the machine, the methods "newRegion", "addState", "addTransition", "addJoin", 
   * "addFork", and "addSubmachine" should be used.
   **/

  class Machine : private RegionsComponent
  {
  public:    
    //! Construct of machine with name specified in input argument.
    Machine(const char *in_machine_name);

    //! Destructor.
    virtual ~Machine();

    //! This method must be overloaded to program the machine.
    virtual bool build() = 0;

    //! Returns the name of the machine.
    std::shared_ptr<std::string> name() const;

    //! Returns regions, that contain states and transitions, of the machine as a RegionComponent object.
    /**
     * The machine is left empty after calling this method.
     **/
    RegionsComponent regionsComponent();

    //! Returns the name of the state that is active within the region specified in input argument.
    /**
     * Returns an empty string if the Region has no active state.
     **/
    std::string activeState(const char *in_region_name) const;

    //! The method checks, each time it is called, fired transitions and changes machine's regions active state consequently.
    bool run();

  protected: 
    //! Adding a new region within the machine.
    /**
     * See also Region.
     **/
    void newRegion(const char *in_region_name);

    //! Adding a state within a region of the machine.
    /**
     * See also SimpleState, InitialState, FinalState, TerminateState and CompositeState.
     **/
    bool addState(const char *in_region_name, std::shared_ptr<SimpleState> in_state);

    //! Adding a transition within a region of the machine.
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Adding a join compound transition within the machine.
    bool addJoin(const char *in_outermost_starting_state_name, std::shared_ptr<Join> in_join);

    //! Adding a fork compound transition within the machine.
    bool addFork(const char *in_outermost_reachable_state_name, std::shared_ptr<Fork> in_fork);
    
    //! Adding a submachine within a region of the machine.
    /**
     * The Machine specified in input argument must contains a least one Region with states and transitions 
     * inside it. The Machine specified in input argument is left empty after calling this method.
     **/
    bool addSubmachine(const char *in_region_name, Machine &in_machine);

  private:    
    //! Specializes RegionsComponent's "findRegion" method.
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;    
    
    //! Specializes RegionsComponent's "findState" method.
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;
    
    bool _isInitiated;
    bool _isTerminated;
    std::shared_ptr<std::string> _machineName;
  };
}

#endif
