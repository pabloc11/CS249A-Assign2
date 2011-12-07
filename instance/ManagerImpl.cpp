#include "InstanceImpl.h"

namespace Shipping {

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
		throw Fwk::IllegalNameException("Invalid instance name.");
		return NULL;
	}
	
	// Instance name already exists
	if (instance(name)) {
		throw Fwk::NameInUseException("Instance name already exists.");
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

}