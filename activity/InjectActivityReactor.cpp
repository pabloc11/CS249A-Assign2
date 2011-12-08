#include "Reactors.h"

InjectActivityReactor::InjectActivityReactor(Activity::Ptr _a) :
	Activity::Notifiee(_a.ptr()),
	activity_(_a),
	transferRate_(0),
	shipmentSize_(0),
	destination_(NULL) {}
	
void InjectActivityReactor::transferRateIs(ShipmentsPerDay n) {
	transferRate_ = n;
}

void InjectActivityReactor::shipmentSizeIs(PackagesPerShipment n) {
	shipmentSize_ = n;
}

void InjectActivityReactor::destinationIs(CustomerLocation::Ptr l) {
	destination_ = l;
}

void InjectActivityReactor::onNextTime() {}

void InjectActivityReactor::onStatus() {
	
	switch (activity_->status()) {
		
		case Activity::queued:
			cout << "Queueing " << activity_->name() << endl;
			break;
	
	  case Activity::executing:
			cout << "Executing " << activity_->name() << endl;
			break;	
			
	  case Activity::done:
			cout << "Done executing " << activity_->name() << endl;
			break;
			
	  default:
			break;
	}
	
}