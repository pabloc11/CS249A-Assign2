#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l) : CustomerLocation::Notifiee() {
		notifierIs(_l);
	}

    void CustomerReactor::onTransferRate(ShipmentsPerDay transferRate_) {
    	// TODO
    }
    void CustomerReactor::onShipmentSize(PackagesPerShipment shipmentSize_) {
    	// TODO
    }
    void CustomerReactor::onDestination(Location::Ptr destination_) {
    	// TODO
    }
}
