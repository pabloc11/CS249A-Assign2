#include <math.h>
#include "Activity.h"
#include "Reactors.h"

ForwardActivityReactor::ForwardActivityReactor(Activity* _a, Fleet::Ptr _fleet, Stats::Ptr _stats, Segment::Ptr _seg, Shipment::Ptr _ship) : 
	Activity::Notifiee(_a),
	fleet_(_fleet),
	stats_(_stats),
	segment_(_seg),
	shipment_(_ship)
	{}

void ForwardActivityReactor::onNextTime() {}

void ForwardActivityReactor::onStatus() {
		
	if (activity_->status() == Activity::queued) {
		cout << "  Queueing " << activity_->name() << endl;
		activityManagerInstance()->lastActivityIs(activity_);
	}

  else if (activity_->status() == Activity::executing) {
		activity_->statusIs(Activity::done);
	}
		
  else if (activity_->status() == Activity::done) {
	
		float cost;
		
		// Truck segment
		TruckSegment* truckPtr = dynamic_cast<TruckSegment *>(segment_.ptr());
		if (truckPtr) cost = fleet_->truckCost().value();
		
		// Boat segment
		BoatSegment* boatPtr = dynamic_cast<BoatSegment *>(segment_.ptr());
		if (boatPtr) cost = fleet_->boatCost().value();
		
		// Plane segment
		PlaneSegment* planePtr = dynamic_cast<PlaneSegment *>(segment_.ptr());
		if (planePtr) cost = fleet_->planeCost().value();
	
		int numPackages = shipment_->numPackages().value();
		int capacity = segment_->capacity().value();
		float difficulty = segment_->difficulty().value();
		float length = segment_->length().value();
	
		int numTrips = ceil((float)numPackages/(float)capacity);
		shipment_->costTakenIs(Fleet::Cost(shipment_->costTaken().value() + numTrips*cost*difficulty*length));
		
		Shipment::Ptr shipment = segment_->activeShipmentDel(shipment_->name());
		shipment->destination()->shipmentIs(shipment);
		
		cout << "  Done executing " << activity_->name() << endl;
		activityManagerInstance()->activityDel(activity_->name());
	}
}

