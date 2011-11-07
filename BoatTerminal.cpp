#include "Engine.h"

namespace Shipping {
  BoatTerminal::BoatTerminal(Fwk::String _name) :
    TerminalLocation(_name, Entity::boatTerminal())
  {}
}
