#include "InstanceImpl.h"

namespace Shipping {

string FleetRep::attribute(const string& name) {
	
	if (name == "Truck, speed")
		return Util::FloatToString(fleet_->truckSpeed().value());
	else if (name == "Boat, speed")
		return Util::FloatToString(fleet_->boatSpeed().value());
	else if (name == "Plane, speed")
		return Util::FloatToString(fleet_->planeSpeed().value());
	else if (name == "Truck, cost")
		return Util::FloatToString(fleet_->truckCost().value());
	else if (name == "Boat, cost")
		return Util::FloatToString(fleet_->boatCost().value());
	else if (name == "Plane, cost")
		return Util::FloatToString(fleet_->planeCost().value());
	else if (name == "Truck, capacity")
		return Util::IntToString(fleet_->truckCapacity().value());
	else if (name == "Boat, capacity")
		return Util::IntToString(fleet_->boatCapacity().value());
	else if (name == "Plane, capacity")
		return Util::IntToString(fleet_->planeCapacity().value());	
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	return "";
}

void FleetRep::attributeIs(const string& name, const string& v) {

	if (name == "Truck, speed") {
		fleet_->truckSpeedIs(Fleet::Speed(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.truckSpeed = Fleet::Speed(atof(v.c_str()));
	}
	else if (name == "Boat, speed") {
		fleet_->boatSpeedIs(Fleet::Speed(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.boatSpeed = Fleet::Speed(atof(v.c_str()));
	}
	else if (name == "Plane, speed") {
		fleet_->planeSpeedIs(Fleet::Speed(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.planeSpeed = Fleet::Speed(atof(v.c_str()));
	}
	else if (name == "Truck, cost") {
		fleet_->truckCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.truckCost = Fleet::Cost(atof(v.c_str()));
	}
	else if (name == "Boat, cost") {
		fleet_->boatCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.boatCost = Fleet::Cost(atof(v.c_str()));
	}
	else if (name == "Plane, cost") {
		fleet_->planeCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.planeCost = Fleet::Cost(atof(v.c_str()));
	}
	else if (name == "Truck, capacity") {
		fleet_->truckCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.truckCapacity = Fleet::Capacity(atoi(v.c_str()));
	}
	else if (name == "Boat, capacity") {
		fleet_->boatCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.boatCapacity = Fleet::Capacity(atoi(v.c_str()));
	}
	else if (name == "Plane, capacity") {
		fleet_->planeCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->scheduledAttrs_.planeCapacity = Fleet::Capacity(atoi(v.c_str()));
	}
	else if (name == "Truck, speed, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sTruckSpeed = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.tSpeed = true;
	}
	else if (name == "Boat, speed, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sBoatSpeed = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.bSpeed = true;
	}
	else if (name == "Plane, speed, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sPlaneSpeed = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.pSpeed = true;
	}
	else if (name == "Truck, cost, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sTruckCost = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.tCost = true;
	}
	else if (name == "Boat, cost, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sBoatCost = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.bCost = true;
	}
	else if (name == "Plane, cost, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sPlaneCost = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.pCost = true;
	}
	else if (name == "Truck, capacity, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sTruckCapacity = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.tCapacity = true;
	}
	else if (name == "Boat, capacity, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sBoatCapacity = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.bCapacity = true;
	}
	else if (name == "Plane, capacity, scheduled") {
		fleetRepReactor_->scheduledAttrs_.sPlaneCapacity = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.pCapacity = true;
	}
	else if (name == "Start time, scheduled") {
		fleetRepReactor_->scheduledAttrs_.startTime = Time(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.start = true;
		if (fleetRepReactor_->scheduledAttrs_.end) fleetRepReactor_->onAttributeIs();
	}
	else if (name == "End time, scheduled") {
		fleetRepReactor_->scheduledAttrs_.endTime = Time(atof(v.c_str()));
		fleetRepReactor_->scheduledAttrs_.end = true;
		if (fleetRepReactor_->scheduledAttrs_.start) fleetRepReactor_->onAttributeIs();
	}	
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}
