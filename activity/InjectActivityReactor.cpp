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

void InjectActivityReactor::onStatus() {

	if (activity_->status() == Activity::queued) {
		//cout << "  Queueing " << activity_->name() << endl;
		activityManagerInstance()->lastActivityIs(activity_);
	}

  else if (activity_->status() == Activity::executing) {
	
		//cout << "  Executing inject activity: " << activity_->name() << endl;
		
		Shipment::Ptr shipment = Shipment::ShipmentNew(Shipment::NumPackages(shipmentSize_.value()), source_, destination_, activity_->nextTime());
		source_->shipmentIs(shipment);
		activity_->nextTimeIs(Time(activityManagerInstance()->now().value() + 24.0/transferRate_.value()));
		activity_->statusIs(Activity::queued);
	}	

}
