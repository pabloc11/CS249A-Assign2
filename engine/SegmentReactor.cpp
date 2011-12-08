#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
	
	SegmentReactor::SegmentReactor(Segment::Ptr _s, Fleet::Ptr _fleet, Stats::Ptr _stats) :
		Segment::Notifiee(),
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
		forwardActivity_->lastNotifieeIs(reactor_.ptr());
		forwardActivity_->statusIs(Activity::queued);
  }
}
