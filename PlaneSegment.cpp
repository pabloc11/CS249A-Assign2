#include "Engine.h"

namespace Shipping {
  PlaneSegment::PlaneSegment(Fwk::String _name) :
    Segment(_name, Entity::planeSegment())
  {}
}
