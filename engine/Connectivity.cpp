#include "Engine.h"

namespace Shipping {
  Connectivity::Connectivity()
  {
	// does nothing
  }
  Connectivity::Connection::Connection() :
	segments_(),
	cost_(0),
	time_(0),
	expedited_(Segment::notExpedited())
  {}

  bool Connectivity::simpleDFS(Location::Ptr currentLocation, Location::Ptr goal, set<Fwk::String> & visited, Connectivity::Connection &path) {
	if (visited.count(currentLocation->name()))
	  return false;

	if (currentLocation->name() == goal->name())
	  return true;

	for (Location::SegmentListIteratorConst iter = currentLocation->segmentIterConst(); iter.ptr(); ++iter) {
	  Segment::PtrConst segment = iter.ptr();
	  if (segment->returnSegment()) {
		path.segments_.push_back(segment);
		if(simpleDFS(segment->returnSegment()->source(), goal, visited, path))
			return true;
		path.segments_.pop_back();
	  }
	}
	return false;
  }

  Connectivity::Connection Connectivity::connect(Location::Ptr start_, Location::Ptr end_) {
	set<Fwk::String> visited;
	Connectivity::Connection path;
	if(!simpleDFS(start_, end_, visited, path)) {
		//TODO: throw an error if route isn't found
		cerr << "No route found!" << endl;
	}
	return path;
  }

  vector<Connectivity::Connection> Connectivity::connectAll(Location::Ptr start_, Location::Ptr end_) {
    return vector<Connectivity::Connection>();
  }
  vector<Connectivity::Connection> Connectivity::exploreAll(Location::Ptr start_, Segment::Length distance_, Fleet::Cost cost_, Time time_, Segment::Expedited expedited_) {
	return vector<Connectivity::Connection>();
  }
}
