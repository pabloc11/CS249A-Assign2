#include "Engine.h"
#include "Reactors.h"

namespace Shipping {

	void SegmentReactor::onExpedited(Segment::Expedited _expedited) {
	  if(_expedited == Segment::expedited())
		stats_->expeditedSegmentCountInc();
	  else if(_expedited == Segment::notExpedited())
		stats_->expeditedSegmentCountDec();
	}

    SegmentReactor::SegmentReactor(Segment::Ptr _s, Stats::Ptr _stats) : Segment::Notifiee(), stats_(_stats) {
      notifierIs(_s);
    }
}