#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include "Activity.h"
#include "Instance.h"
#include "Engine.h"
#include "Reactors.h"

using namespace std;

Fwk::Ptr<Instance::Manager> shippingInstanceManager();

namespace Shipping {

class Util
{
public:
	static string IntToString(int v) {
		std::stringstream out;
		out << v;
		return out.str();	
	}

	static string FloatToString(float v) {
	 	char buffer [100];
		int n = sprintf(buffer, "%.2f", v);
		if (n >= 100) {
			cerr << "Floating point number too large to display." << endl;
			throw Fwk::RangeException("Floating point number too large to display");
		}
		return buffer;
	}
};

class StatsRep;
class FleetRep;
class ConnRep;
class ClockRep;
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

	Activity::Manager::Ptr activityManager();
	Network::Ptr network() const { return network_; }
	Ptr<FleetRep> fleetRep() const { return fleetRep_; }
	Ptr<StatsRep> statsRep() const { return statsRep_; }
	Ptr<ConnRep> connRep() const { return connRep_; }

private:
	friend class ClockRep;
	Activity::Manager::Ptr activityManager_;
    map<string,Ptr<Instance> > instance_;
	Network::Ptr network_;
	NetworkReactor::Ptr networkReactor_;
	Ptr<StatsRep> statsRep_;
	Ptr<FleetRep> fleetRep_;
	Ptr<ConnRep> connRep_;
	Ptr<ClockRep> clockRep_;
};

/************************** REPRESENTATION CLASSES **************************/

class StatsRep : public Instance {
public:	
	StatsRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {
		stats_ = Stats::StatsNew(manager->network());
	}
	Stats::Ptr stats() const { return stats_; }
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
	Fleet::Ptr fleet() const { return fleet_; }
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;
	Fleet::Ptr fleet_;
};

class ClockRep : public Instance {
public:
	ClockRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {
		activityManager_ = activityManagerInstance();
	}
	Activity::Manager::Ptr activityManager() const { return activityManager_; }
	string attribute(const string& name);
	void attributeIs(const string& name, const string& v);

private:
  Ptr<ManagerImpl> manager_;
  Activity::Manager::Ptr activityManager_;
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
	Connectivity::Ptr connectivity() const { return connectivity_; }
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
	Connectivity::Ptr connectivity_;
};

class LocationRep : public Instance {
public:
    LocationRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {}
		Location::Ptr location() const { return location_; }
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
		Segment::Ptr segment() const { return segment_; }
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
		CustomerLocation::Ptr ptr = CustomerLocation::CustomerLocationNew(name);
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
		PortLocation::Ptr ptr = PortLocation::PortLocationNew(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class TruckTerminalRep : public LocationRep {
public:
    TruckTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		TruckTerminal::Ptr ptr = TruckTerminal::TruckTerminalNew(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class BoatTerminalRep : public LocationRep {
public:
    BoatTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		BoatTerminal::Ptr ptr = BoatTerminal::BoatTerminalNew(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class PlaneTerminalRep : public LocationRep {
public:
    PlaneTerminalRep(const string& name, ManagerImpl *manager) : LocationRep(name, manager) {
		PlaneTerminal::Ptr ptr = PlaneTerminal::PlaneTerminalNew(name);
		manager->network()->entityIs(ptr);
		location_ = ptr;
		locationReactor_ = new LocationReactor(location_);
	}
};

class TruckSegmentRep : public SegmentRep {
public:
    TruckSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
 		TruckSegment::Ptr ptr = TruckSegment::TruckSegmentNew(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

class BoatSegmentRep : public SegmentRep {
public:
    BoatSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		BoatSegment::Ptr ptr = BoatSegment::BoatSegmentNew(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

class PlaneSegmentRep : public SegmentRep {
public:
    PlaneSegmentRep(const string& name, ManagerImpl *manager) : SegmentRep(name, manager) {
		PlaneSegment::Ptr ptr = PlaneSegment::PlaneSegmentNew(name);
		manager->network()->entityIs(ptr);
		segment_ = ptr;
		segmentReactor_ = new SegmentReactor(segment_, manager->statsRep()->stats());
	}
};

}
