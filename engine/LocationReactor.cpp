#include "Reactors.h"
#include "Engine.h"
#include "Activity.h"

namespace Shipping {
	LocationReactor::LocationReactor(Location::Ptr _l) : Location::Notifiee() {
		notifierIs(_l);
	}

	void LocationReactor::onShipmentNew(Shipment::Ptr _ptr) {
		if(_ptr->destination()->name() == notifier_->name()) {
			_ptr->destination()->shipmentsReceived_ = NumShipments(_ptr->destination()->shipmentsReceived_.value() + 1);
			Time elapsed(activityManagerInstance()->now().value() - _ptr->startTime_.value());
			_ptr->destination()->totalLatency_ = Time(_ptr->destination()->totalLatency_.value() + elapsed.value());
			_ptr->destination()->totalCost_ = Fleet::Cost(_ptr->destination()->totalCost_.value() + _ptr->costTaken_.value());
			return;
		}

		// do forwarding
		notifier_->shipmentDel(_ptr->name());
		Segment::Ptr nextSegment = notifier_->route(_ptr->destination()->name());
		nextSegment->queuedShipmentIs(_ptr);
	}
}
