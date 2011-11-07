#include "Engine.h"

namespace Shipping {
  CustomerLocation::CustomerLocation(Fwk::String _name) :
    Location(_name, Entity::customerLocation())
  {}
}
