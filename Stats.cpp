#include "Engine.h"

namespace Shipping {
  
  class SegmentReactor : public Segment::NotifieeConst {
  public:
    void onExpedited(Segment::Expedited _expedited) {
      if(_expedited == Segment::expedited())
        stats_->expeditedSegmentCountInc();
      else if(_expedited == Segment::notExpedited())
        stats_->expeditedSegmentCountDec();
    }
    
    SegmentReactor(Segment * _s, Stats* _stats) : Segment::NotifieeConst(), stats_(_stats) {
      notifierIs(_s);
    }
  private:
    Fwk::Ptr<Stats> stats_;
  };
  
  class NetworkReactor : public Network::NotifieeConst {
  public:
      void onEntityNew(Entity::Ptr _ptr) {        
        if(_ptr->entityType() == Entity::truckSegment()) {
          stats_->truckSegmentCountInc();
          if(((TruckSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
          new SegmentReactor((Segment*)_ptr.ptr(), stats_.ptr());
        }
       
        else if(_ptr->entityType() == Entity::boatSegment()) {
          stats_->boatSegmentCountInc();
          if(((BoatSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
          new SegmentReactor((Segment*)_ptr.ptr(), stats_.ptr());
        }
        
        else if(_ptr->entityType() == Entity::planeSegment()) {
          stats_->planeSegmentCountInc();
          if(((PlaneSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountInc();
          }
          new SegmentReactor((Segment*)_ptr.ptr(), stats_.ptr());
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
      void onEntityDel(Entity::Ptr _ptr) {
        if(_ptr->entityType() == Entity::truckSegment()) {
          stats_->truckSegmentCountDec();
          if(((TruckSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
        }
       
        if(_ptr->entityType() == Entity::boatSegment()) {
          stats_->boatSegmentCountDec();
          if(((BoatSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
        }
        
        if(_ptr->entityType() == Entity::planeSegment()) {
          stats_->planeSegmentCountDec();
          if(((PlaneSegment*)_ptr.ptr())->expeditedState() == Segment::expedited()) {
            stats_->expeditedSegmentCountDec();
          }
        }
        
        if(_ptr->entityType() == Entity::customerLocation()) {
          stats_->customerLocationCountDec();
        }
        
        if(_ptr->entityType() == Entity::portLocation()) {
          stats_->portLocationCountDec();
        }
        
        if(_ptr->entityType() == Entity::truckTerminal()) {
          stats_->truckTerminalCountDec();
        }
        
        if(_ptr->entityType() == Entity::boatTerminal()) {
          stats_->boatTerminalCountDec();
        }
        
        if(_ptr->entityType() == Entity::planeTerminal()) {
          stats_->planeTerminalCountDec();
        }
      }
      NetworkReactor(Network::Ptr _n, Stats* _stats) : Network::NotifieeConst(), stats_(_stats) {
        notifierIs(_n);
      }
  private:
    Fwk::Ptr<Stats> stats_;
  };
  
  Stats::Stats(Network::Ptr _n) :
    expeditedSegmentCount_(0),
    truckSegmentCount_(0),
    boatSegmentCount_(0),
    planeSegmentCount_(0),
    customerLocationCount_(0),
    portLocationCount_(0),
    truckTerminalCount_(0),
    boatTerminalCount_(0),
    planeTerminalCount_(0)
  {
    new NetworkReactor(_n, this);
  }
  
  void Stats::truckSegmentCountInc() {
    truckSegmentCount_++;
  }
  
  void Stats::boatSegmentCountInc() {
    boatSegmentCount_++;
  }
  
  void Stats::planeSegmentCountInc() {
    planeSegmentCount_++;
  }
  
  void Stats::customerLocationCountInc() {
    customerLocationCount_++;
  }
  
  void Stats::portLocationCountInc() {
    portLocationCount_++;
  }
  
  void Stats::truckTerminalCountInc() {
    truckTerminalCount_++;
  }
  
  void Stats::boatTerminalCountInc() {
    boatTerminalCount_++;
  }
  
  void Stats::planeTerminalCountInc() {
    planeTerminalCount_++;
  }
  
  void Stats::expeditedSegmentCountInc() {
    expeditedSegmentCount_++;
  }

  void Stats::truckSegmentCountDec() {
    truckSegmentCount_--;
  }
  
  void Stats::boatSegmentCountDec() {
    boatSegmentCount_--;
  }
    
  void Stats::planeSegmentCountDec() {
    planeSegmentCount_--;
  }
  
  void Stats::customerLocationCountDec() {
    customerLocationCount_--;
  }
  
  void Stats::portLocationCountDec() {
    portLocationCount_--;
  }

  void Stats::truckTerminalCountDec() {
    truckTerminalCount_--;
  }
  
  void Stats::boatTerminalCountDec() {
    boatTerminalCount_--;
  }
  
  void Stats::planeTerminalCountDec() {
    planeTerminalCount_--;
  }
  
  void Stats::expeditedSegmentCountDec() {
    expeditedSegmentCount_--;
  }
}
