#include "Engine.h"

namespace Shipping {
  Fleet::Fleet() :
    truckSpeed_(1),
    boatSpeed_(1),
    planeSpeed_(1),
    truckCost_(1),
    boatCost_(1),
    planeCost_(1),
    truckCapacity_(1),
    boatCapacity_(1),
    planeCapacity_(1) 
  {}
  
  void Fleet::truckSpeedIs(Fleet::Speed _truckSpeed) {
    truckSpeed_ = _truckSpeed;
  }
  
  void Fleet::boatSpeedIs(Fleet::Speed _boatSpeed) {
    boatSpeed_ = _boatSpeed;
  }
  
  void Fleet::planeSpeedIs(Fleet::Speed _planeSpeed) {
    planeSpeed_ = _planeSpeed;
  }
  
  void Fleet::truckCostIs(Fleet::Cost _truckCost) {
    truckCost_ = _truckCost;
  }
  
  void Fleet::boatCostIs(Fleet::Cost _boatCost) {
    boatCost_ = _boatCost;
  }
  
  void Fleet::planeCostIs(Fleet::Cost _planeCost) {
    planeCost_ = _planeCost;
  }
  
  void Fleet::truckCapacityIs(Fleet::Capacity _truckCapacity) {
    truckCapacity_ = _truckCapacity;
  }
  
  void Fleet::boatCapacityIs(Fleet::Capacity _boatCapacity) {
    boatCapacity_ = _boatCapacity;
  }
  
  void Fleet::planeCapacityIs(Fleet::Capacity _planeCapacity) {
    planeCapacity_ = _planeCapacity;
  }
}
