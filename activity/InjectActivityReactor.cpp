#include "Reactors.h"

using namespace Shipping;

InjectActivityReactor::InjectActivityReactor(Activity::Ptr _a, CustomerLocation::Ptr _l) :
	Activity::Notifiee(_a.ptr()),
	activity_(_a),
	transferRate_(0),
	shipmentSize_(0),
	source_(_l),
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

void InjectActivityReactor::onNextTime() {
	activity_->statusIs(Activity::queued);
}

void InjectActivityReactor::onStatus() {
	
	switch (activity_->status()) {
		
		case Activity::queued:
			cout << "  Queueing " << activity_->name() << endl;
			activityManagerInstance()->lastActivityIs(activity_);
			break;
	
	  case Activity::executing:
			cout << "  Executing " << activity_->name() << endl;
			Shipment::ShipmentNew(Shipment::NumPackages(shipmentSize_.value()), source_, destination_, activity_->nextTime());
			break;	
			
	  case Activity::done:
			cout << "  Done executing " << activity_->name() << endl;
			activity_->statusIs(Activity::done);
			activity_->nextTimeIs(activity_->nextTime().value() + 24.0/transferRate_.value());
			break;
			
	  default:
			break;
	}
	
}