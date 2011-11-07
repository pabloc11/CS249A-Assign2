#include "Engine.h"

namespace Shipping {
  Location::Location(Fwk::String _name, Entity::EntityType _type) :
    Entity(_name, Entity::customerLocation()),
    segment_()
  {}
}
