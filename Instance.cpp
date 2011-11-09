#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include "Instance.h"
#include "Engine.h"

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

private:
    map<string,Ptr<Instance> > instance_;
	Network::Ptr network_;
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
		float dist;
		float cost;
		float time;
		bool expedited;
	};
	struct SearchProgress {
		float dist;
		float cost;
		float time;
	};
	
	ConnRep(const string& name, ManagerImpl* manager);
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);
	
 	string explore(Ptr<LocationRep> loc);
	string connect(Ptr<LocationRep> loc0, Ptr<LocationRep> loc1);

private:
	bool paramsExceeded(SearchProgress& progress);
	void DFS(Location::PtrConst loc, set<string>& visited, SearchProgress& progress, string& output);
	
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
};
                                                                                                  
class CustomerLocationRep : public LocationRep {
public:
    CustomerLocationRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		CustomerLocation::Ptr ptr = CustomerLocation::CustomerLocationIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
	}
};

class PortLocationRep : public LocationRep {
public:
    PortLocationRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		PortLocation::Ptr ptr = PortLocation::PortLocationIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
	}
};

class TruckTerminalRep : public LocationRep {
public:
    TruckTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		TruckTerminal::Ptr ptr = TruckTerminal::TruckTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
	}
};

class BoatTerminalRep : public LocationRep {
public:
    BoatTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		BoatTerminal::Ptr ptr = BoatTerminal::BoatTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
	}
};

class PlaneTerminalRep : public LocationRep {
public:
    PlaneTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		PlaneTerminal::Ptr ptr = PlaneTerminal::PlaneTerminalIs(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
	}
};

class TruckSegmentRep : public SegmentRep {
public:
    TruckSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
 		TruckSegment::Ptr ptr = TruckSegment::TruckSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
	}
};

class BoatSegmentRep : public SegmentRep {
public:
    BoatSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		BoatSegment::Ptr ptr = BoatSegment::BoatSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
	}
};

class PlaneSegmentRep : public SegmentRep {
public:
    PlaneSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		PlaneSegment::Ptr ptr = PlaneSegment::PlaneSegmentIs(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
	}
};

/************************ INSTANCE MANAGER IMPLEMENTATION ************************/

ManagerImpl::ManagerImpl() {
	network_ = NULL;
	statsRep_ = NULL;
	fleetRep_ = NULL;
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {
	
	// First call to instanceNew
	if (!network_) {
		network_ = Network::NetworkNew("network");
		statsRep_ = new StatsRep("statsRep", this);
		fleetRep_ = new FleetRep("fleetRep", this);
		connRep_ = new ConnRep("connRep", this);
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
	else 
		cerr << "Invalid type: " << type << endl;

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
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
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
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;

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
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
}

ConnRep::ConnRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {
	params_.dist = 0;
	params_.cost = 0;
	params_.time = 0;
	params_.expedited = false;
}

bool ConnRep::paramsExceeded(SearchProgress& progress) {
	
	if ((params_.dist != 0 && progress.dist > params_.dist) || (params_.cost != 0 && progress.cost > params_.cost) ||
	  	(params_.time != 0 && progress.time > params_.time))
		return true;
	
	return false;
}

void ConnRep::DFS(Location::PtrConst loc, set<string>& visited, SearchProgress& progress, string& output) {

	if (visited.count(loc->name()) || !paramsExceeded(progress))
		return;
	cout << "Visiting: " << loc->name() << endl;
	visited.insert(loc->name());
	
	for (Location::SegmentListIteratorConst iter = loc->segmentIterConst(); iter.ptr(); ++iter) {
		Segment::PtrConst segment = iter.ptr();
		if (segment->returnSegment()) {
			
			float dist = segment->length().value();
			
			float cost, speed;
			if (segment->entityType() == Entity::truckSegment()) {
				cost = manager_->fleetRep()->fleet()->truckCost().value();
				speed = manager_->fleetRep()->fleet()->truckSpeed().value();
			}
			else if (segment->entityType() == Entity::boatSegment()) {
				cost = manager_->fleetRep()->fleet()->boatCost().value();
				speed = manager_->fleetRep()->fleet()->boatSpeed().value();
			}
			else if (segment->entityType() == Entity::planeSegment()) {
				cost = manager_->fleetRep()->fleet()->planeCost().value();
				speed = manager_->fleetRep()->fleet()->planeSpeed().value();
			}
			
			float time = dist / speed;
			
			progress.dist += dist;
			progress.cost += cost;
			progress.time += time;
			DFS(segment->returnSegment()->source(), visited, progress, output);
		}
    }	
	
}

string ConnRep::explore(Ptr<LocationRep> loc) {
	
	SearchProgress progress = {0, 0, 0};
	set<string> visited;
	string output = "";

	DFS(loc->location(), visited, progress, output);
	
	return output;
}

string ConnRep::connect(Ptr<LocationRep> loc0, Ptr<LocationRep> loc1) {
	return "";
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
		if (token == tokens.end() || *token != ":") return "";
		advance(token, 1);

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
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
	
	return "";
}

void ConnRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

string LocationRep::attribute(const string& name) {
	
    int i = segmentNumber(name);
    if (i != 0) {
		if (location_->segment(i))
			return location_->segment(i)->name();
		else
			cerr << "Segment not found with index: " << i << endl;
    }
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
    return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

string SegmentRep::attribute(const string& name) {
	
    if (name == "source")
		return segment_->source()->name();
	else if (name == "length")
		return FloatToString(segment_->length().value());
	else if (name == "return segment")
		return segment_->returnSegment()->name();
	else if (name == "difficulty")
		return FloatToString(segment_->difficulty().value());
	else if (name == "expedite support") {
		if (segment_->expeditedState() == Segment::expedited())
			return "yes";
		else if (segment_->expeditedState() == Segment::notExpedited())
			return "no";
	}
	else
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		
	return "";
}

void SegmentRep::attributeIs(const string& name, const string& v) {
	
    if (name == "source") {
		Ptr<LocationRep> ptr = dynamic_cast<LocationRep *>(manager_->instance(v).ptr());
		if (ptr)
			segment_->sourceIs(ptr->location());
		else
			cerr << "Instance given to 'source' is not a location: " << v << endl;
    }
	else if (name == "length") {
		try	{ segment_->lengthIs(Segment::Length(atof(v.c_str()))); }
		catch (Fwk::Exception e) { cerr << "Invalid length: " << v << endl; }			
	}
	else if (name == "return segment") {
		if (v == "") segment_->returnSegmentIs(NULL);
		Ptr<SegmentRep> ptr = dynamic_cast<SegmentRep *>(manager_->instance(v).ptr());
		if (ptr) {
			try { segment_->returnSegmentIs(ptr->segment()); }
			catch (Fwk::Exception e) { cerr << "Return segment given to " << this->name() << " must have the same entity type."; }
		}
		else
			cerr << "Instance given to 'return segment' is not a segment: " << v << endl;
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
		else
			cerr << "Invalid expedite support: " << v << endl;
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
	}
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
