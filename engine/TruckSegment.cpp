#include "Engine.h"

namespace Shipping {
  TruckSegment::TruckSegment(Fwk::String _name) :
    Segment(_name, Entity::truckSegment())
  {}
}
