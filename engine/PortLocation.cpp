#include "Engine.h"

namespace Shipping {
  PortLocation::PortLocation(Fwk::String _name) :
    Location(_name, Entity::portLocation())
  {}
}
