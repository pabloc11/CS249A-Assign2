#include "Reactors.h"
#include "Engine.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l) :
	CustomerLocation::Notifiee(),
	transferRateInit_(false),
	shipmentSizeInit_(false),
	destinationInit_(false)
	{
		notifierIs(_l);
	}

    void CustomerReactor::onTransferRate(ShipmentsPerDay transferRate_) {
    	transferRateInit_ = true;
    	createOrUpdateActivity();
    }
    void CustomerReactor::onShipmentSize(PackagesPerShipment shipmentSize_) {
    	shipmentSizeInit_ = true;
    	createOrUpdateActivity();
    }
    void CustomerReactor::onDestination(CustomerLocation::Ptr destination_) {
    	destinationInit_ = true;
    	createOrUpdateActivity();
    }

    void CustomerReactor::createOrUpdateActivity(){
    	if(!transferRateInit_ || !shipmentSizeInit_ ||  !destinationInit_)
    		return;
    	/*if(activity pointer == null)
    	{
    		create new injecting activity
    		return;
    	}
    	update the properties of the existing injecting activity
    	*/
    }
}