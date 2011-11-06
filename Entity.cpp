#include "Engine.h"

namespace Shipping {
  Entity::Entity(Fwk::String _name, EntityType _entityType) :
    Fwk::NamedInterface(_name),
    entityType_(_entityType) 
  {}
}
