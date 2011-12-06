#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l) : CustomerLocation::Notifiee() {
		notifierIs(_l);
	}
}
