#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
	
	SegmentReactor::SegmentReactor(Segment::Ptr _s, Fleet::Ptr _fleet, Stats::Ptr _stats) :
		Segment::Notifiee(),
		segment_(_s),
		fleet_(_fleet),
		stats_(_stats),
		forwardActivity_(NULL),
		reactor_(NULL)		
 	{
    notifierIs(_s);
  }

	void SegmentReactor::onExpedited(Segment::Expedited _expedited) {
	  if(_expedited == Segment::expedited())
		stats_->expeditedSegmentCountInc();
	  else if(_expedited == Segment::notExpedited())
		stats_->expeditedSegmentCountDec();
	}

  void SegmentReactor::onActiveShipmentNew(Shipment::Ptr _ptr) {
  	forwardActivity_ = activityManagerInstance()->activityNew(_ptr->name());
		reactor_ = new ForwardActivityReactor(forwardActivity_.ptr(), fleet_, stats_, notifier(), _ptr);
		
		float speed;
		float length = segment_->length().value();
		
		// Truck segment
		TruckSegment* truckPtr = dynamic_cast<TruckSegment *>(segment_.ptr());
		if (truckPtr) speed = fleet_->truckSpeed().value();
		
		// Boat segment
		BoatSegment* boatPtr = dynamic_cast<BoatSegment *>(segment_.ptr());
		if (boatPtr) speed = fleet_->boatSpeed().value();
		
		// Plane segment
		PlaneSegment* planePtr = dynamic_cast<PlaneSegment *>(segment_.ptr());
		if (planePtr) speed = fleet_->planeSpeed().value();
		
		forwardActivity_->nextTimeIs(activityManagerInstance()->now().value() + length/speed);
		forwardActivity_->lastNotifieeIs(reactor_.ptr());
		forwardActivity_->statusIs(Activity::queued);
  }
}
