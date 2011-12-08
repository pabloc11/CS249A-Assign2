#include "Engine.h"
#include "Reactors.h"

namespace Shipping {

	void SegmentReactor::onExpedited(Segment::Expedited _expedited) {
	  if(_expedited == Segment::expedited())
		stats_->expeditedSegmentCountInc();
	  else if(_expedited == Segment::notExpedited())
		stats_->expeditedSegmentCountDec();
	}

    void SegmentReactor::onActiveShipmentNew(Shipment::Ptr _ptr) {
    	//TODO: create the forwarding activity and kick it off
    }

    SegmentReactor::SegmentReactor(Segment::Ptr _s, Stats::Ptr _stats, Fleet::Ptr _fleet) :
      Segment::Notifiee(),
      stats_(_stats),
      fleet_(_fleet)
    {
      notifierIs(_s);
    }
}
