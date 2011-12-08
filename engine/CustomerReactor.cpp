#include "Reactors.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l) :
	CustomerLocation::Notifiee(),
	reactor_(NULL),
	transferRateInit_(false),
	shipmentSizeInit_(false),
	destinationInit_(false),
	transferRate_(0),
	shipmentSize_(0),
	source_(_l),
	destination_(NULL)
	{
		notifierIs(_l);
	}

    void CustomerReactor::onTransferRate(ShipmentsPerDay _transferRate) {
    	transferRateInit_ = true;
			transferRate_ = _transferRate;
    	createOrUpdateActivity();
    }
    void CustomerReactor::onShipmentSize(PackagesPerShipment _shipmentSize) {
    	shipmentSizeInit_ = true;
			shipmentSize_ = _shipmentSize;
    	createOrUpdateActivity();
    }
    void CustomerReactor::onDestination(CustomerLocation::Ptr _destination) {
    	destinationInit_ = true;
			destination_ = _destination;
    	createOrUpdateActivity();
    }

    void CustomerReactor::createOrUpdateActivity(){
    	if(!transferRateInit_ || !shipmentSizeInit_ ||  !destinationInit_)
    		return;
    	/*
			if (injectActivity_ == NULL)
   		{
				injectActivity_ = activityManagerInstance()->activityNew(notifier()->name());
				reactor_ = new InjectActivityReactor(injectActivity_, source_);
				injectActivity_->lastNotifieeIs(reactor_.ptr());
				injectActivity_->statusIs(Activity::queued);
			}	
			reactor_->transferRateIs(transferRate_);
			reactor_->shipmentSizeIs(shipmentSize_);
			reactor_->destinationIs(destination_);*/
    }
}
