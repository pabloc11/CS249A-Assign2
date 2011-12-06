#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	LocationReactor::LocationReactor(Location::Ptr _l) : Location::Notifiee() {
		notifierIs(_l);
	}

	void LocationReactor::onShipmentNew(Shipment::Ptr _ptr) {
		//TODO: do forwarding (or not if not needed)
	}
}
