#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include "Instance.h"
#include "Engine.h"
#include "Reactors.h"

namespace Shipping {

using namespace std;

string IntToString(int v) {
	std::stringstream out;
	out << v;
	return out.str();	
}

string FloatToString(float v) {
 	char buffer [100];
	int n = sprintf(buffer, "%.2f", v);
	if (n >= 100)
		throw Fwk::RangeException("Floating point number too large to display\n");
	return buffer;
}

//
// Rep layer classes
//

class StatsRep;
class FleetRep;
class ConnRep;
class LocationRep;

class ManagerImpl : public Instance::Manager {
public:
	ManagerImpl();

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

	Network::Ptr network() { return network_; }
	Ptr<FleetRep> fleetRep() { return fleetRep_; }
	Ptr<StatsRep> statsRep() { return statsRep_; }

private:
    map<string,Ptr<Instance> > instance_;
	Network::Ptr network_;
	NetworkReactor::Ptr networkReactor_;
	Ptr<StatsRep> statsRep_;
	Ptr<FleetRep> fleetRep_;
	Ptr<ConnRep> connRep_;
};

/************************** REPRESENTATION CLASSES **************************/

class StatsRep : public Instance {
public:	
	StatsRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {
		stats_ = Stats::StatsNew(manager->network());
	}
	Stats::Ptr stats() { return stats_; }
	string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

private:
    Ptr<ManagerImpl> manager_;
	Stats::Ptr stats_;
};

class FleetRep : public Instance {
public:	
	FleetRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {
		fleet_ = Fleet::FleetNew();
	}
	Fleet::Ptr fleet() { return fleet_; }
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);

private:
    Ptr<ManagerImpl> manager_;
	Fleet::Ptr fleet_;
};

class ConnRep : public Instance {
public:	
	struct SearchParams {
		// Explore params
		float dist;
		float cost;
		float time;
		bool expedited;
		// Connect params
		string destination;
	};
	struct SearchProgress {
		float dist;
		float cost;
		float time;
	};
	
	ConnRep(const string& name, ManagerImpl* manager);
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);

private:
	bool paramsExceeded(SearchProgress& progress);
	void appendToPath(Segment::PtrConst seg, Location::PtrConst loc, string& path);
	SearchProgress updateProgress(Segment::PtrConst seg, SearchProgress& progress, bool expedited);
	void exploreDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
			 		SearchProgress progress, string& output, string path);
	void connectDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
					SearchProgress progress, bool expedited, string& output, string path);
 	string explore(Ptr<LocationRep> loc);
	string connect(Ptr<LocationRep> loc0, Ptr<LocationRep> loc1);
	
    Ptr<ManagerImpl> manager_;
	SearchParams params_;	
};

class LocationRep : public Instance {
public:
    LocationRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {}
	Location::Ptr location() { return location_; }
    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

protected:
    Ptr<ManagerImpl> manager_;
	Location::Ptr location_;
	LocationReactor::Ptr locationReactor_;
    int segmentNumber(const string& name);
};

class SegmentRep : public Instance {
public:
    SegmentRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {}
	Segment::Ptr segment() { return segment_; }
    string attribute(const string& name);
    void attributeIs(const string& name, const string& v);

protected:
    Ptr<ManagerImpl> manager_;
	Segment::Ptr segment_;
	SegmentReactor::Ptr segmentReactor_;
};
                                                                                                  
class CustomerLocationRep : public LocationRep {
public:
    CustomerLocationRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		CustomerLocation::Ptr ptr = CustomerLocation::CustomerLocationIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
		customerReactor_ = new CustomerReactor(ptr);
	}
protected:
    CustomerReactor::Ptr customerReactor_;
};

class PortLocationRep : public LocationRep {
public:
    PortLocationRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		PortLocation::Ptr ptr = PortLocation::PortLocationIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class TruckTerminalRep : public LocationRep {
public:
    TruckTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		TruckTerminal::Ptr ptr = TruckTerminal::TruckTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class BoatTerminalRep : public LocationRep {
public:
    BoatTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		BoatTerminal::Ptr ptr = BoatTerminal::BoatTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class PlaneTerminalRep : public LocationRep {
public:
    PlaneTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		PlaneTerminal::Ptr ptr = PlaneTerminal::PlaneTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class TruckSegmentRep : public SegmentRep {
public:
    TruckSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
 		TruckSegment::Ptr ptr = TruckSegment::TruckSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

class BoatSegmentRep : public SegmentRep {
public:
    BoatSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		BoatSegment::Ptr ptr = BoatSegment::BoatSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

class PlaneSegmentRep : public SegmentRep {
public:
    PlaneSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		PlaneSegment::Ptr ptr = PlaneSegment::PlaneSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

/************************ INSTANCE MANAGER IMPLEMENTATION ************************/

ManagerImpl::ManagerImpl() {
	network_ = NULL;
	networkReactor_ = NULL;
	statsRep_ = NULL;
	fleetRep_ = NULL;
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	
	// First call to instanceNew
	if (!network_) {
		network_ = Network::NetworkNew("network");
		statsRep_ = new StatsRep("statsRep", this);
		networkReactor_ = new NetworkReactor(network_, statsRep_->stats());
		fleetRep_ = new FleetRep("fleetRep", this);
		connRep_ = new ConnRep("connRep", this);
	}
	
	// Instance name is invalid
	if (name == "") {
		cerr << "Invalid instance name." << endl;
		return NULL;
	}
	
	// Instance name already exists
	if (instance(name)) {
		cerr << "An instance already exists with the name: " << name << endl;
		return NULL;
	}

	if (type == "Stats" ) {
		return statsRep_;
	}
	else if (type == "Fleet" ) {
		return fleetRep_;
	}
	else if (type == "Conn") {
		return connRep_;
	}
    else if (type == "Customer") {
        Ptr<CustomerLocationRep> t = new CustomerLocationRep(name, this);
        instance_[name] = t;
        return t;
    }
    else if (type == "Port") {
        Ptr<PortLocationRep> t = new PortLocationRep(name, this);
        instance_[name] = t;
        return t;
    }
    else if (type == "Truck terminal") {
        Ptr<TruckTerminalRep> t = new TruckTerminalRep(name, this);
        instance_[name] = t;
        return t;
    }
    else if (type == "Boat terminal") {
        Ptr<BoatTerminalRep> t = new BoatTerminalRep(name, this);
        instance_[name] = t;
        return t;
    }
    else if (type == "Plane terminal") {
        Ptr<PlaneTerminalRep> t = new PlaneTerminalRep(name, this);
        instance_[name] = t;
        return t;
    }
	else if (type == "Truck segment") {
		Ptr<TruckSegmentRep> t = new TruckSegmentRep(name, this);
		instance_[name] = t;
		return t;
	}
	else if (type == "Boat segment") {
		Ptr<BoatSegmentRep> t = new BoatSegmentRep(name, this);
		instance_[name] = t;
		return t;
	}
	else if (type == "Plane segment") {
		Ptr<PlaneSegmentRep> t = new PlaneSegmentRep(name, this);
		instance_[name] = t;
		return t;
	}
	else cerr << "Invalid type: " << type << endl;

    return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);
    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
	network_->entityDel(name);
	instance_.erase(name);
}

/***************** REPRESENTATION CLASS IMPLEMENTATIONS *****************/

string StatsRep::attribute(const string& name) {
	
	if (name == "Customer")
		return IntToString(stats_->customerLocationCount());
	else if (name == "Port")
		return IntToString(stats_->portLocationCount());
	else if (name == "Truck terminal")
		return IntToString(stats_->truckTerminalCount());
	else if (name == "Boat terminal")
		return IntToString(stats_->boatTerminalCount());
	else if (name == "Plane terminal")
		return IntToString(stats_->planeTerminalCount());
	else if (name == "Truck segment")
		return IntToString(stats_->truckSegmentCount());
	else if (name == "Boat segment")
		return IntToString(stats_->boatSegmentCount());
	else if (name == "Plane segment")
		return IntToString(stats_->planeSegmentCount());
	else if (name == "expedite percentage")
		return FloatToString(stats_->expeditePercentage().value()*100);
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
	return "";
}

void StatsRep::attributeIs(const string& name, const string& v) {
	// nothing to do
}

string FleetRep::attribute(const string& name) {
	
	if (name == "Truck, speed")
		return FloatToString(fleet_->truckSpeed().value());
	else if (name == "Boat, speed")
		return FloatToString(fleet_->boatSpeed().value());
	else if (name == "Plane, speed")
		return FloatToString(fleet_->planeSpeed().value());
	else if (name == "Truck, cost")
		return FloatToString(fleet_->truckCost().value());
	else if (name == "Boat, cost")
		return FloatToString(fleet_->boatCost().value());
	else if (name == "Plane, cost")
		return FloatToString(fleet_->planeCost().value());
	else if (name == "Truck, capacity")
		return IntToString(fleet_->truckCapacity().value());
	else if (name == "Boat, capacity")
		return IntToString(fleet_->boatCapacity().value());
	else if (name == "Plane, capacity")
		return IntToString(fleet_->planeCapacity().value());	
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;

	return "";
}

void FleetRep::attributeIs(const string& name, const string& v) {

	if (name == "Truck, speed") {
		try { fleet_->truckSpeedIs(Fleet::Speed(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid speed: " << v << endl; }
	}
	else if (name == "Boat, speed") {
		try { fleet_->boatSpeedIs(Fleet::Speed(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid speed: " << v << endl; }
	}
	else if (name == "Plane, speed") {
		try { fleet_->planeSpeedIs(Fleet::Speed(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid speed: " << v << endl; }
	}
	else if (name == "Truck, cost") {
		try { fleet_->truckCostIs(Fleet::Cost(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid cost: " << v << endl; }
	}
	else if (name == "Boat, cost") {
		try { fleet_->boatCostIs(Fleet::Cost(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid cost: " << v << endl; }
	}
	else if (name == "Plane, cost") {
		try { fleet_->planeCostIs(Fleet::Cost(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid cost: " << v << endl; }
	}
	else if (name == "Truck, capacity") {
		try { fleet_->truckCapacityIs(Fleet::Capacity(atoi(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid capacity: " << v << endl; }
	}
	else if (name == "Boat, capacity") {
		try { fleet_->boatCapacityIs(Fleet::Capacity(atoi(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid capacity: " << v << endl; }
	}
	else if (name == "Plane, capacity") {
		try { fleet_->planeCapacityIs(Fleet::Capacity(atoi(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid capacity: " << v << endl; }
	}
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
}

ConnRep::ConnRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {	
	params_.dist = 0;
	params_.cost = 0;
	params_.time = 0;
	params_.expedited = false;
	params_.destination = "";
}

bool ConnRep::paramsExceeded(SearchProgress& progress) {
	
	if ((params_.dist != 0 && progress.dist > params_.dist) || (params_.cost != 0 && progress.cost > params_.cost) ||
	  	(params_.time != 0 && progress.time > params_.time))
		return true;
	
	return false;
}

void ConnRep::appendToPath(Segment::PtrConst seg, Location::PtrConst loc, string& path) {
	
	if (path == "") path += seg->source()->name();
	path += "(" + seg->name() + ":" + FloatToString(seg->length().value()) + ":" +
		seg->returnSegment()->name() + ") " + loc->name();
}

ConnRep::SearchProgress ConnRep::updateProgress(Segment::PtrConst seg, SearchProgress& progress, bool expedited) {
	
	float distance = seg->length().value();
	float difficulty = seg->difficulty().value();
	
	float cost, speed;
	if (seg->entityType() == Entity::truckSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->truckCost().value();
		speed = manager_->fleetRep()->fleet()->truckSpeed().value();
	}
	else if (seg->entityType() == Entity::boatSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->boatCost().value();
		speed = manager_->fleetRep()->fleet()->boatSpeed().value();
	}
	else if (seg->entityType() == Entity::planeSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->planeCost().value();
		speed = manager_->fleetRep()->fleet()->planeSpeed().value();
	}
	
	// Update for expedited shipping
	if (expedited) {
		cost *= 1.5;
		speed *= 1.3;
	}
	
	float time = distance / speed;
	
	// Update progress
	SearchProgress newProgress = { progress.dist + distance, progress.cost + cost, progress.time + time };
	return newProgress;
}

void ConnRep::exploreDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
				  		 SearchProgress progress, string& output, string path) {

	// Base case - location has already been visited or search parameters are exceeded
	if (visited.count(loc->name()) || paramsExceeded(progress))
		return;
	
	// Add path to output string
	if (seg) {
		appendToPath(seg, loc, path);
		output += path + "\n";
	}
	
	visited.insert(loc->name());
	
	// Search all outbound segments from current location
	for (Location::SegmentListIteratorConst iter = loc->segmentIterConst(); iter.ptr(); ++iter) {
		Segment::PtrConst segment = iter.ptr();
		
		// Skip segment if expedited shipping is specified but segment doesn't offer it
		if (params_.expedited && segment->expeditedState() != Segment::expedited())
			continue;
					
		if (segment->returnSegment()) {			
			SearchProgress newProgress = updateProgress(segment, progress, params_.expedited);
			exploreDFS(segment, segment->returnSegment()->source(), visited, newProgress, output, path);
		}
    }		
}

void ConnRep::connectDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
				  		 SearchProgress progress, bool expedited, string& output, string path) {

	// Base case - location has already been visited
	if (visited.count(loc->name()))
		return;
	
	if (seg) appendToPath(seg, loc, path);
	
	// Add path to output string if destination reached
	if (loc->name() == params_.destination) {
		string isExpedited;
		if (expedited) isExpedited = "yes";
		else isExpedited = "no";
		output += FloatToString(progress.cost) + " " + FloatToString(progress.time) + " " + isExpedited + "; " + path + "\n";
		return;
	}
	
	visited.insert(loc->name());
	
	// Search all outbound segments from current location
	for (Location::SegmentListIteratorConst iter = loc->segmentIterConst(); iter.ptr(); ++iter) {
		Segment::PtrConst segment = iter.ptr();	
			
		if (segment->returnSegment()) {			
			SearchProgress newProgress = updateProgress(segment, progress, expedited);
			connectDFS(segment, segment->returnSegment()->source(), visited, newProgress, expedited, output, path);
		}
    }		
}

string ConnRep::explore(Ptr<LocationRep> loc) {
	
	SearchProgress progress = {0, 0, 0};
	set<string> visited;
	string output = "";

	exploreDFS(NULL, loc->location(), visited, progress, output, "");
	
	size_t end = output.find_last_of("\n");
	if (end != string::npos) output = output.substr(0, end);
	return output;
}

string ConnRep::connect(Ptr<LocationRep> loc0, Ptr<LocationRep> loc1) {
	
	params_.destination = loc1->name();
	SearchProgress progress = {0, 0, 0};
	set<string> visited;
	string output = "";	
	connectDFS(NULL, loc0->location(), visited, progress, false, output, "");
	
	SearchProgress expeditedProgress = {0, 0, 0};
	set<string> expeditedVisited;
	connectDFS(NULL, loc0->location(), expeditedVisited, expeditedProgress, true, output, "");
	
	size_t end = output.find_last_of("\n");
	if (end != string::npos) output = output.substr(0, end);
	return output;
}

string ConnRep::attribute(const string& name) {
	
	// Tokenize string
	istringstream iss(name);
	vector<string> tokens;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
	vector<string>::iterator token;
	token = tokens.begin();
	if (token == tokens.end()) return "";
	
	if (*token == "explore") {
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc) return "";
		advance(token, 1);
		if (token != tokens.end() && *token == ":") advance(token, 1);

		while (token != tokens.end()) {
			string attr = *token;
			advance(token, 1);
			if (attr == "expedited") {
				params_.expedited = true;
				continue;
			}
			if (token == tokens.end()) return "";
			string val = *token;
			advance(token, 1);
			if (attr == "distance") params_.dist = atof(val.c_str());
			else if (attr == "cost") params_.cost = atof(val.c_str());
			else if (attr == "time") params_.time = atof(val.c_str());
		}
		return explore(loc);
	}
	else if (*token == "connect") {
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc0 = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc0) return "";
		advance(token, 1);
		if (token == tokens.end() || *token != ":") return "";
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc1 = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc1) return "";
		return connect(loc0, loc1);
	}
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
	
	return "";
}

void ConnRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

string LocationRep::attribute(const string& name) {
	
    int i = segmentNumber(name);
    if (i != 0) {
		if (location_->segment(i)) return location_->segment(i)->name();
		else cerr << "Segment not found with index: " << i << endl;
    }
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
    return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

string SegmentRep::attribute(const string& name) {
	
    if (name == "source") {
        Location::PtrConst l = segment_->source();
        if(l)
    		return l->name();
        return "";
    }
	else if (name == "length")
		return FloatToString(segment_->length().value());
	else if (name == "return segment") {
        Segment::PtrConst s = segment_->returnSegment();
        if(s)
    		return s->name();
        return "";
    }
	else if (name == "difficulty")
		return FloatToString(segment_->difficulty().value());
	else if (name == "expedite support") {
		if (segment_->expeditedState() == Segment::expedited())
			return "yes";
		else if (segment_->expeditedState() == Segment::notExpedited())
			return "no";
	}
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
	return "";
}

void SegmentRep::attributeIs(const string& name, const string& v) {
	
    if (name == "source") {
		Ptr<LocationRep> ptr = dynamic_cast<LocationRep *>(manager_->instance(v).ptr());
		if (ptr) segment_->sourceIs(ptr->location());
		else cerr << "Instance given to 'source' is not a location: " << v << endl;
    }
	else if (name == "length") {
		try	{ segment_->lengthIs(Segment::Length(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid length: " << v << endl; }			
	}
	else if (name == "return segment") {
		if (v == "") {
			segment_->returnSegmentIs(NULL);
			return;
		}
		Ptr<SegmentRep> ptr = dynamic_cast<SegmentRep *>(manager_->instance(v).ptr());
		if (ptr) {
			try { segment_->returnSegmentIs(ptr->segment()); }
			catch (Fwk::Exception e) { cerr << "Return segment given to " << this->name() << " must have the same entity type."; }
		}
		else cerr << "Instance given to 'return segment' is not a segment: " << v << endl;
	}
	else if (name == "difficulty") {
		try { segment_->difficultyIs(Segment::Difficulty(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid difficulty: " << v << endl; }
	}
	else if (name == "expedite support") {
		if (v == "yes")
			segment_->expeditedIs(Segment::expedited());
		else if (v == "no")
			segment_->expeditedIs(Segment::notExpedited());
		else cerr << "Invalid expedite support: " << v << endl;
	}
	else cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
}

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t);
    }
    return 0;
}


}

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Ptr<Instance::Manager> shippingInstanceManager() {
    return new Shipping::ManagerImpl();
}
