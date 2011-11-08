#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
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
	sprintf (buffer, "%.2f", v);
	return buffer;
}

//
// Rep layer classes
//

class StatsRep;
class FleetRep;

class ManagerImpl : public Instance::Manager {
public:
	ManagerImpl();

    // Manager method
    Ptr<Instance> instanceNew(const string& name, const string& type);

    // Manager method
    Ptr<Instance> instance(const string& name);

    // Manager method
    void instanceDel(const string& name);

private:
    map<string,Ptr<Instance> > instance_;
	Ptr<StatsRep> statsRep_;
	Ptr<FleetRep> fleetRep_;
};

/*********************** REPRESENTATION CLASSES ***********************/

class StatsRep : public Instance {
public:	
	StatsRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {}
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
		fleet_ = new Fleet();
	}
	Fleet::Ptr fleet() { return fleet_; }
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);

private:
    Ptr<ManagerImpl> manager_;
	Fleet::Ptr fleet_;
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
		location_ = CustomerLocation::CustomerLocationIs(name);
	}
};

class PortLocationRep : public LocationRep {
public:
    PortLocationRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		location_ = PortLocation::PortLocationIs(name);
	}
};

class TruckTerminalRep : public LocationRep {
public:
    TruckTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		location_ = TruckTerminal::TruckTerminalIs(name);
	}
};

class BoatTerminalRep : public LocationRep {
public:
    BoatTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		location_ = BoatTerminal::BoatTerminalIs(name);
	}
};

class PlaneTerminalRep : public LocationRep {
public:
    PlaneTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		location_ = PlaneTerminal::PlaneTerminalIs(name);
	}
};

class TruckSegmentRep : public SegmentRep {
public:
    TruckSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
 		segment_ = TruckSegment::TruckSegmentIs(name);
	}
};

class BoatSegmentRep : public SegmentRep {
public:
    BoatSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		segment_ = BoatSegment::BoatSegmentIs(name);
	}
};

class PlaneSegmentRep : public SegmentRep {
public:
    PlaneSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		segment_ = PlaneSegment::PlaneSegmentIs(name);
	}
};

/************************** IMPLEMENTATIONS **************************/

ManagerImpl::ManagerImpl() {
	statsRep_ = NULL;
	fleetRep_ = NULL;
}

Ptr<Instance> ManagerImpl::instanceNew(const string& name, const string& type) {

	if (type == "Stats" ) {
		if (!statsRep_) {
       	 	Ptr<StatsRep> t = new StatsRep(name, this);
	        instance_[name] = t;
			statsRep_ = t;
	        return t;
		}
		else {
			return statsRep_;
		}
	}
	else if (type == "Fleet" ) {
		if (!fleetRep_) {
       	 	Ptr<FleetRep> t = new FleetRep(name, this);
	        instance_[name] = t;
			fleetRep_ = t;
	        return t;
		}
		else {
			return fleetRep_;
		}
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

    return NULL;
}

Ptr<Instance> ManagerImpl::instance(const string& name) {
    map<string,Ptr<Instance> >::const_iterator t = instance_.find(name);

    return t == instance_.end() ? NULL : (*t).second;
}

void ManagerImpl::instanceDel(const string& name) {
}

/***************** REPRESENTATION CLASS IMPLEMENTATIONS *****************/

string StatsRep::attribute(const string& name) {
	return "";
}

void StatsRep::attributeIs(const string& name, const string& v) {
	
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

	return "";
}

void FleetRep::attributeIs(const string& name, const string& v) {

	if (name == "Truck, speed")
		fleet_->truckSpeedIs(Fleet::Speed(atof(v.c_str())));
	else if (name == "Boat, speed")
		fleet_->boatSpeedIs(Fleet::Speed(atof(v.c_str())));
	else if (name == "Plane, speed")
		fleet_->planeSpeedIs(Fleet::Speed(atof(v.c_str())));
	else if (name == "Truck, cost")
		fleet_->truckCostIs(Fleet::Cost(atof(v.c_str())));
	else if (name == "Boat, cost")
		fleet_->boatCostIs(Fleet::Cost(atof(v.c_str())));
	else if (name == "Plane, cost")
		fleet_->planeCostIs(Fleet::Cost(atof(v.c_str())));
	else if (name == "Truck, capacity")
		fleet_->truckCapacityIs(Fleet::Capacity(atoi(v.c_str())));
	else if (name == "Boat, capacity")
		fleet_->boatCapacityIs(Fleet::Capacity(atoi(v.c_str())));
	else if (name == "Plane, capacity")
		fleet_->planeCapacityIs(Fleet::Capacity(atoi(v.c_str())));
}

string LocationRep::attribute(const string& name) {
    int i = segmentNumber(name);
    if (i != 0) {
		//return location_->segment(i)->name();
    }
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
		
	return "";
}

void SegmentRep::attributeIs(const string& name, const string& v) {
    if (name == "source") {
		Ptr<LocationRep> ptr = dynamic_cast<LocationRep *>(manager_->instance(v).ptr());
		if (ptr) {
			segment_->sourceIs(ptr->location());
		}
    }
	if (name == "length") {
		segment_->lengthIs(Segment::Length(atof(v.c_str())));
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
