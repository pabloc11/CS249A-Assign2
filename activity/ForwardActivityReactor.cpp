#include "Activity.h"
#include "Reactors.h"

ForwardActivityReactor::ForwardActivityReactor(Activity* _a, Fleet::Ptr _fleet, Stats::Ptr _stats, Segment::Ptr _seg, Shipment::Ptr _ship) : 
	Activity::Notifiee(_a),
	fleet_(_fleet),
	stats_(_stats),
	segment_(_seg),
	capacity_(_seg->capacity()),
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
	
			cout << "  Done executing " << activity_->name() << endl;
			activity_->statusIs(Activity::done);
		
			float length = segment_->length().value();
			float speed, cost;
				
			// Truck segment
			TruckSegment* truckPtr = dynamic_cast<TruckSegment *>(segment_.ptr());
			if (truckPtr) {
				speed = fleet_->truckSpeed().value();
				cost = fleet_->truckCost().value();
			}
			
			// Boat segment
			BoatSegment* boatPtr = dynamic_cast<BoatSegment *>(segment_.ptr());
			if (boatPtr) {
				speed = fleet_->boatSpeed().value();
				cost = fleet_->boatCost().value();
			}
			
			// Plane segment
			PlaneSegment* planePtr = dynamic_cast<PlaneSegment *>(segment_.ptr());
			if (planePtr) {
				speed = fleet_->planeSpeed().value();
				cost = fleet_->planeCost().value();
			}
			
			activityManagerInstance()->activityDel(activity_->name());
		}
}