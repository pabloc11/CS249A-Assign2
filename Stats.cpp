#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
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
	  //nothing to do here
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
