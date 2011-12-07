#include "Engine.h"

namespace Shipping {
  Connectivity::Connectivity(Fleet::Ptr _fleet) :
    fleet_(_fleet)
  {
	// does nothing
  }
  Connectivity::Connection::Connection() :
	segments_(),
	cost_(0),
	time_(0),
	distance_(0),
	expedited_(Segment::notExpedited())
  {}

  bool Connectivity::simpleDFS(Location::Ptr currentLocation, Location::Ptr goal, set<Fwk::String> & visited, Connectivity::Connection &path) {
	if (visited.count(currentLocation->name()))
	  return false;

	if (currentLocation->name() == goal->name())
	  return true;

	for (Location::SegmentListIterator iter = currentLocation->segmentIter(); iter.ptr(); ++iter) {
	  Segment::Ptr segment = iter.ptr();
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

  void Connectivity::DFSall(Segment::Ptr prevSegment, Location::Ptr currentLocation,
		  Location::Ptr goal, set<Fwk::String> visited,
		  Connectivity::Connection path,
		  vector<Connectivity::Connection> & results,
		  Segment::Expedited expedited)
  {
	if(visited.count(currentLocation->name()))
	  return;
	if(prevSegment) {
	  path.segments_.push_back(prevSegment);
	  path.expedited_ = expedited;
	  float segmentCost = 0;
	  float segmentSpeed = 0;
	  if(prevSegment->entityType() == Entity::truckSegment()) {
		segmentCost = fleet_->truckCost().value();
	    segmentSpeed = fleet_->truckSpeed().value();
	  }
	  else if(prevSegment->entityType() == Entity::planeSegment()) {
		segmentCost = fleet_->planeCost().value();
	    segmentSpeed = fleet_->planeSpeed().value();
	  }
	  else if(prevSegment->entityType() == Entity::boatSegment()) {
		segmentCost = fleet_->boatCost().value();
	  	segmentSpeed = fleet_->boatSpeed().value();
	  }
	  float additionalCost = prevSegment->difficulty().value() * prevSegment->length().value() * segmentCost;
	  if(expedited == Segment::expedited()) {
		additionalCost *= additionalCost;
		segmentSpeed *= segmentSpeed;
	  }
	  path.cost_ = Fleet::Cost(path.cost_.value() + additionalCost);
	  path.time_ = Time(path.time_.value() + (prevSegment->length().value() / segmentSpeed));
	}
	if(currentLocation->name() == goal->name())
	  results.push_back(path);

	visited.insert(currentLocation->name());

	for (Location::SegmentListIterator iter = currentLocation->segmentIter(); iter.ptr(); ++iter) {
	  Segment::Ptr segment = iter.ptr();
	  if (segment->returnSegment() && (expedited == Segment::notExpedited() || segment->expedited() == Segment::expedited()))
		DFSall(segment, segment->returnSegment()->source(), goal, visited, path, results, expedited);
    }
  }

  vector<Connectivity::Connection> Connectivity::connectAll(Location::Ptr start_, Location::Ptr end_)
  {
	vector<Connectivity::Connection> paths;
	set<Fwk::String> visited;
	Connectivity::Connection path;
	DFSall(NULL, start_, end_, visited, path, paths, Segment::notExpedited());
	DFSall(NULL, start_, end_, visited, path, paths, Segment::expedited());
    return paths;
  }

  void Connectivity::DFSallWithLimit(Segment::Ptr prevSegment, Location::Ptr currentLocation,
		  set<Fwk::String> visited,
		  Connectivity::Connection path,
		  vector<Connectivity::Connection> & results,
		  Segment::Expedited expedited,
		  Fleet::Cost maxCost, Time maxTime, Segment::Length maxDist)
  {
	if(visited.count(currentLocation->name()))
	  return;

	if(prevSegment) {
	  path.segments_.push_back(prevSegment);
	  path.expedited_ = expedited;
	  float segmentCost = 0;
	  float segmentSpeed = 0;
	  if(prevSegment->entityType() == Entity::truckSegment()) {
		segmentCost = fleet_->truckCost().value();
	    segmentSpeed = fleet_->truckSpeed().value();
	  }
	  else if(prevSegment->entityType() == Entity::planeSegment()) {
		segmentCost = fleet_->planeCost().value();
	    segmentSpeed = fleet_->planeSpeed().value();
	  }
	  else if(prevSegment->entityType() == Entity::boatSegment()) {
		segmentCost = fleet_->boatCost().value();
	  	segmentSpeed = fleet_->boatSpeed().value();
	  }
	  float additionalCost = prevSegment->difficulty().value() * prevSegment->length().value() * segmentCost;
	  if(expedited == Segment::expedited()) {
		additionalCost *= additionalCost;
		segmentSpeed *= segmentSpeed;
	  }
	  path.cost_ = Fleet::Cost(path.cost_.value() + additionalCost);
	  path.time_ = Time(path.time_.value() + (prevSegment->length().value() / segmentSpeed));
	  path.distance_ = Segment::Length(path.distance_.value() + prevSegment->length().value());
	}

	if((maxCost.value() > 0 && path.cost_ > maxCost) || (maxTime.value() > 0 && path.time_ > maxTime) || (maxDist.value() > 0 && path.distance_ > maxDist))
	  return;

	results.push_back(path);

	visited.insert(currentLocation->name());

	for (Location::SegmentListIterator iter = currentLocation->segmentIter(); iter.ptr(); ++iter) {
	  Segment::Ptr segment = iter.ptr();
	  if (segment->returnSegment() && (expedited == Segment::notExpedited() || segment->expedited() == Segment::expedited()))
		DFSallWithLimit(segment, segment->returnSegment()->source(), visited, path, results, expedited, maxCost, maxTime, maxDist);
    }
  }

  vector<Connectivity::Connection> Connectivity::exploreAll(Location::Ptr start_, Segment::Length distance_, Fleet::Cost cost_, Time time_, Segment::Expedited expedited_) {
	vector<Connectivity::Connection> paths;
	set<Fwk::String> visited;
	Connectivity::Connection path;
	DFSallWithLimit(NULL, start_, visited, path, paths, expedited_, cost_, time_, distance_);
	return paths;
  }
}
