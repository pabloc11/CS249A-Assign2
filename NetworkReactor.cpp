#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
    void NetworkReactor::onEntityNew(Entity::Ptr _ptr) {
    	if(_ptr->entityType() == Entity::truckSegment()) {
          stats_->truckSegmentCountInc();
          if(((TruckSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
        }

        else if(_ptr->entityType() == Entity::boatSegment()) {
          stats_->boatSegmentCountInc();
          if(((BoatSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
        }

        else if(_ptr->entityType() == Entity::planeSegment()) {
          stats_->planeSegmentCountInc();
          if(((PlaneSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
        }

        else if(_ptr->entityType() == Entity::customerLocation()) {
          stats_->customerLocationCountInc();
        }

        else if(_ptr->entityType() == Entity::portLocation()) {
          stats_->portLocationCountInc();
        }

        else if(_ptr->entityType() == Entity::truckTerminal()) {
          stats_->truckTerminalCountInc();
        }

        else if(_ptr->entityType() == Entity::boatTerminal()) {
          stats_->boatTerminalCountInc();
        }

        else if(_ptr->entityType() == Entity::planeTerminal()) {
          stats_->planeTerminalCountInc();
        }
      }
      void NetworkReactor::onEntityDel(Entity::Ptr _ptr) {
        if(_ptr->entityType() == Entity::truckSegment()) {
          stats_->truckSegmentCountDec();
          if(((TruckSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
          TruckSegment* s = ((TruckSegment*)_ptr.ptr());
          s->sourceIs(NULL);
          s->returnSegmentIs(NULL);
        }

        else if(_ptr->entityType() == Entity::boatSegment()) {
          stats_->boatSegmentCountDec();
          if(((BoatSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
          BoatSegment* s = ((BoatSegment*)_ptr.ptr());
          s->sourceIs(NULL);
          s->returnSegmentIs(NULL);
        }

        else if(_ptr->entityType() == Entity::planeSegment()) {
          stats_->planeSegmentCountDec();
          if(((PlaneSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
          PlaneSegment* s = ((PlaneSegment*)_ptr.ptr());
          s->sourceIs(NULL);
          s->returnSegmentIs(NULL);
        }

        else if(_ptr->entityType() == Entity::customerLocation()) {
          stats_->customerLocationCountDec();
          CustomerLocation * l = (CustomerLocation *)_ptr.ptr();
          for(Segment::Ptr p = l->segment(1); p.ptr(); p = l->segment(1)) {
            p->sourceIs(NULL);
          }
        }

        else if(_ptr->entityType() == Entity::portLocation()) {
          stats_->portLocationCountDec();
          PortLocation * l = (PortLocation *)_ptr.ptr();
          for(Segment::Ptr p = l->segment(1); p.ptr(); p = l->segment(1)) {
            p->sourceIs(NULL);
          }
        }

        else if(_ptr->entityType() == Entity::truckTerminal()) {
          stats_->truckTerminalCountDec();
          TruckTerminal * l = (TruckTerminal *)_ptr.ptr();
          for(Segment::Ptr p = l->segment(1); p.ptr(); p = l->segment(1)) {
            p->sourceIs(NULL);
          }
        }

        else if(_ptr->entityType() == Entity::boatTerminal()) {
          stats_->boatTerminalCountDec();
          BoatTerminal * l = (BoatTerminal *)_ptr.ptr();
          for(Segment::Ptr p = l->segment(1); p.ptr(); p = l->segment(1)) {
            p->sourceIs(NULL);
          }
        }

        else if(_ptr->entityType() == Entity::planeTerminal()) {
          stats_->planeTerminalCountDec();
          PlaneTerminal * l = (PlaneTerminal *)_ptr.ptr();
          for(Segment::Ptr p = l->segment(1); p.ptr(); p = l->segment(1)) {
            p->sourceIs(NULL);
          }
        }
      }
      NetworkReactor::NetworkReactor(Network::Ptr _n, Stats::Ptr _stats) : Network::Notifiee(), stats_(_stats) {
        notifierIs(_n);
      }
}
