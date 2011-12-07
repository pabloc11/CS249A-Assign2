#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	LocationReactor::LocationReactor(Location::Ptr _l) : Location::Notifiee() {
		notifierIs(_l);
	}

	void LocationReactor::onShipmentNew(Shipment::Ptr _ptr) {
		if(_ptr->destination()->name() == notifier_->name()) {
			// it has arrived at its destination, TODO: update the stats
			return;
		}

		// do forwarding
		notifier_->shipmentDel(_ptr->name());
		Segment::Ptr nextSegment = notifier_->route(_ptr->destination()->name());
		nextSegment->queuedShipmentIs(_ptr);
	}
}
