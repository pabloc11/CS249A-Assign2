#include "Reactors.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l, Activity::Manager::Ptr _a) :
	CustomerLocation::Notifiee(),
	activityManager_(_a),
	reactor_(NULL),
	transferRateInit_(false),
	shipmentSizeInit_(false),
	destinationInit_(false),
	transferRate_(0),
	shipmentSize_(0),
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
			if (injectActivity_ == NULL)
   		{
				injectActivity_ = activityManager_->activityNew("inject activity");
				reactor_ = new InjectActivityReactor(injectActivity_);
				injectActivity_->lastNotifieeIs(reactor_.ptr());
				injectActivity_->statusIs(Activity::queued);
			}	
			reactor_->transferRateIs(transferRate_);
			reactor_->shipmentSizeIs(shipmentSize_);
			reactor_->destinationIs(destination_);
    }
}
