#include "Engine.h"

namespace Shipping {
  TruckTerminal::TruckTerminal(Fwk::String _name) :
    TerminalLocation(_name, Entity::truckTerminal())
  {}
}
