#include "lamp_machine.hpp"

// Definition of a "SimpleState" state that correspond to the car's door opened.
class DoorOpened : public SimpleState
{
public:
  // Constructor.
  // The state is named "Door opened".
  DoorOpened() : SimpleState("Door opened")
  {}

  // Method called when the machine reach a state "Door opened".
  void entry() const
  {
    std::cout << "Door is opened." << std::endl;
  }
  // Method called when the machine quit a state "Door opened".
  void exit() const
  {}
  
};

// Definition of a "SimpleState" state that correspond to the car's door closed.
class DoorClosed : public SimpleState
{
public:
  // Constructor.
  // The state is named "Door closed".
  DoorClosed() : SimpleState("Door closed")
  {}

  // Method called when the machine reach a state "Door closed".
  void entry() const
  {
    std::cout << "Door is closed." << std::endl;
  }
  // Method called when the machine quit the state "Door closed".
  void exit() const
  {}
};

// Definition of the event that trigger the car's door from state "Door opened" to state "Door closed".
class CloseDoor : public ChangeEvent<bool>
{
public:
  CloseDoor() : ChangeEvent<bool>()
  {
    add("close door", false);
  }

  bool happened() const
  {
    return value("close door");
  }
};

// Definition of the event that trigger the car's door from state "Door closed" to state "Door opened".
class OpenDoor : public ChangeEvent<bool>
{
public:
  OpenDoor() : ChangeEvent<bool>()
  {
    add("open door", false);
  }

  bool happened() const
  {
    return value("open door");
  }
};

// Definition of the car machine.
class CarMachine : public Machine
{
public:
  CarMachine() : Machine("Car machine") {}  
  virtual ~CarMachine() {}

  // Building of the machine.
  bool build()
  {
    // Adding a new region named "Car" in the machine.
    this->newRegion("Car");

    // Adding an InitialState named "Initial" in the region "Car" of the machine.
    this->addState("Car", std::make_shared<InitialState>("Initial"));

    // Adding a CompositeState named "Car parked" in the region "Car" of the machine.
    auto car_parked = std::make_shared<CompositeState>("Car parked");
    this->addState("Car", car_parked);

    // Adding two new regions named "Car door" and "Interior lamp" in the state "Car parked".
    car_parked->newRegion("Car door");
    car_parked->newRegion("Interior lamp");

    // Adding a untriggered transition from state "Initial" to state "Car parked".
    this->addTransition(std::make_shared<Transition>("k0", "Initial", "Car parked"));

    // Adding the states named "Door initial", "Door opened" and "Door closed" in the region "Car door"
    // of the state "Car parked".
    this->addState("Car door", std::make_shared<InitialState>("Door initial"));
    this->addState("Car door", std::make_shared<DoorOpened>());
    this->addState("Car door", std::make_shared<DoorClosed>());

    // Adding an untriggered transition from the state "Door initial" to the state "Door closed".
    this->addTransition(std::make_shared<Transition>("k1", "Door initial", "Door closed"));

    // Adding a triggered transition from the state "Door closed" to the state "Door opened".
    auto k2 = std::make_shared<Transition>("k2", "Door closed", "Door opened");
    this->_openDoor = std::make_shared<OpenDoor>();
    k2->setTrigger(this->_openDoor);
    this->addTransition(k2);

    // Adding a triggered transition from the state "Door opened" to the state "Door closed".
    auto k3 = std::make_shared<Transition>("k3", "Door opened", "Door closed");
    this->_closeDoor = std::make_shared<CloseDoor>();
    k3->setTrigger(this->_closeDoor);
    this->addTransition(k3);

    // Building the car's interior lamp machine
    LampMachine lamp_machine;
    lamp_machine.build();
    this->_switchLampOff = lamp_machine.switchOff;
    this->_switchLampOn = lamp_machine.switchOn;

    // Adding the lamp machine as a submachine in the region "Interior lamp" of the state "Car parked".
    this->addSubmachine("Interior lamp", lamp_machine);

    // Adding an InitialState name "Interior lamp initial" in the region "Interior lamp" of the state "Car parked".
    this->addState("Interior lamp", std::make_shared<InitialState>("Interior lamp initial"));

    // Adding a transition from the state "Interior lamp initial" to the submachine state "Lamp machine".
    this->addTransition(std::make_shared<Transition>("k4", "Interior lamp initial", "Lamp machine"));
  
    return true;
  }

  // Method that trigger the events to open the car's door.
  void openDoor()
  {
    this->_closeDoor->switching("close door", false);
    this->_openDoor->switching("open door", true);    
    this->_switchLampOff->switching("switch OFF", false);
    this->_switchLampOn->switching("switch ON", true);
  }

  // Method that trigger the events to close the car's door.
  void closeDoor()
  {
    this->_closeDoor->switching("close door", true);
    this->_openDoor->switching("open door", false);    
    this->_switchLampOff->switching("switch OFF", true);
    this->_switchLampOn->switching("switch ON", false);
  }
  

  // Triggers.
  std::shared_ptr<SwitchOFF> _switchLampOff;
  std::shared_ptr<SwitchON> _switchLampOn;
  std::shared_ptr<CloseDoor> _closeDoor;
  std::shared_ptr<OpenDoor> _openDoor;
};


int main(void)
{
  // Building the machine.
  CarMachine car_machine;
  car_machine.build();

  std::cout << "Reusing of an automaton." << std::endl << std::endl;
  
  // Firt run.
  if (!car_machine.run())
    {
      std::cout << "example_car, run failed." << std::endl;
      return -1;
    }
  std::cout << std::endl;

  // Switching 5 times from state "Door closed" to state "Door opened" and from state "Door opened" to state "Door closed".
  for (int i = 0; i < 5; i++)
    {
      // Opening door
      car_machine.openDoor();
      if (!car_machine.run())
	{
	  std::cout << "example_car: run failed." << std::endl;
	  return -1;
	}
      std::cout << std::endl;
      
      // Closing door
      car_machine.closeDoor();
      if (!car_machine.run())
	{
	  std::cout << "example_car: run failed." << std::endl;
	  return -1;
	}
      std::cout << std::endl;
    }
  
  return 0;
  
} // That's all!
