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

#include <memory>

namespace fisa
{
  //#########################################################################################################
  /* 
     Machine
  */
  //! Class for the modeling and the simulation of automata or concurrent systems.
  /**
   * A machine is made of many regions. A region is where states and transitions are programmed.
   * The regions in the machine or in "CompositeState" states are concurrent and only one transition can
   * fired in a region.
   * The class must be inherited and the method "build" overloaded to program the machine. 
   **/

  class Machine
  {
  public:    
    //! Constructor.
    Machine();

    //! \private
    virtual ~Machine();

    //! This method must be overloaded to put instructions that program the machine.
    virtual bool build() = 0;

    //! Changes states of the machine.
    /** 
     * Once the machine is builded, the method should be called for the initialization of machine.
     * Each time it is called, the method check reception of events that fire transitions and 
     * changes states consequently.
     **/
    bool run();
    
    //! Returns the name of the state that is active within the region given in argument.
    //** If the region has no active state, the method returns an empty string. **/
    std::string activeState(const char *in_region_name) const;

  protected: 
    //! Adding a new region within the machine.
    /** This method should be used to build the machine. **/
    void newRegion(const char *in_region_name);

    //! Adding a state in the machine.
    /** This method should be used to build the machine. **/
    bool addState(const char *in_region_name, std::shared_ptr<SimpleState> in_state);

    //! Adding a transition in the machine.
    /** This method should be used to build the machine. **/
    bool addTransition(std::shared_ptr<Transition> in_transition);

    //! Adding a join transition in the machine.
    /** This method should be used to build the machine. **/
    bool addJoin(const char *in_outermost_starting_state_name, std::shared_ptr<JoinTransition> in_join);

    //! Adding a fork transition in the machine.
    /** This method should be used to build the machine. **/
    bool addFork(const char *in_outermost_reachable_state_name, std::shared_ptr<ForkTransition> in_fork);

  private:
    //! \private
    std::shared_ptr<Region> findRegion(std::shared_ptr<std::string> in_region_name) const;

    //! \private
    std::shared_ptr<SimpleState> findState(std::shared_ptr<std::string> in_state_name) const;
    
    bool _isInitiated;
    bool _isTerminated;
    std::vector<std::shared_ptr<Region> > _regions;
  };
}

#endif
