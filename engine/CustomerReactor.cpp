#include "Reactors.h"

namespace Shipping {
	CustomerReactor::CustomerReactor(CustomerLocation::Ptr _l, Activity::Manager::Ptr _a) :
	CustomerLocation::Notifiee(),
	activityManager_(_a),
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
				injectActivity_->lastNotifieeIs(new InjectActivityReactor(injectActivity_));
				injectActivity_->statusIs(Activity::queued);
			}	
			InjectActivityReactor* reactor = dynamic_cast<InjectActivityReactor* >(injectActivity_->notifiee().ptr());
			reactor->transferRateIs(transferRate_);
			reactor->shipmentSizeIs(shipmentSize_);
			reactor->destinationIs(destination_);
    }
}
