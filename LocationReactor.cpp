#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	LocationReactor::LocationReactor(Location::Ptr _l) : Location::Notifiee() {
		notifierIs(_l);
	}
}
