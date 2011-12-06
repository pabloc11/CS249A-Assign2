#include "Engine.h"

namespace Shipping {
  PlaneTerminal::PlaneTerminal(Fwk::String _name) :
    TerminalLocation(_name, Entity::planeTerminal())
  {}
}
