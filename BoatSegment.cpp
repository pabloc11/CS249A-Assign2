#include "Engine.h"

namespace Shipping {
  BoatSegment::BoatSegment(Fwk::String _name) :
    Segment(_name, Entity::boatSegment())
  {}
}
