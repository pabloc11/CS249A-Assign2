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
		fleetRepReactor_->truckSpeed_ = fleet_->truckSpeed();
	}
	else if (name == "Boat, speed") {
		fleet_->boatSpeedIs(Fleet::Speed(atof(v.c_str())));
		fleetRepReactor_->boatSpeed_ = fleet_->boatSpeed();
	}
	else if (name == "Plane, speed") {
		fleet_->planeSpeedIs(Fleet::Speed(atof(v.c_str())));
		fleetRepReactor_->planeSpeed_ = fleet_->planeSpeed();
	}
	else if (name == "Truck, cost") {
		fleet_->truckCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->truckCost_ = fleet_->truckCost();
	}
	else if (name == "Boat, cost") {
		fleet_->boatCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->boatCost_ = fleet_->boatCost();
	}
	else if (name == "Plane, cost") {
		fleet_->planeCostIs(Fleet::Cost(atof(v.c_str())));
		fleetRepReactor_->planeCost_ = fleet_->planeCost();
	}
	else if (name == "Truck, capacity") {
		fleet_->truckCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->truckCapacity_ = fleet_->truckCapacity();
	}
	else if (name == "Boat, capacity") {
		fleet_->boatCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->boatCapacity_ = fleet_->boatCapacity();
	}
	else if (name == "Plane, capacity") {
		fleet_->planeCapacityIs(Fleet::Capacity(atoi(v.c_str())));
		fleetRepReactor_->planeCapacity_ = fleet_->planeCapacity();
	}
	else if (name == "Truck, speed, special") {
		fleetRepReactor_->specialTruckSpeed_ = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->tSpeed = true;
	}
	else if (name == "Boat, speed, special") {
		fleetRepReactor_->specialBoatSpeed_ = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->bSpeed = true;
	}
	else if (name == "Plane, speed, special") {
		fleetRepReactor_->specialPlaneSpeed_ = Fleet::Speed(atof(v.c_str()));
		fleetRepReactor_->pSpeed = true;
	}
	else if (name == "Truck, cost, special") {
		fleetRepReactor_->specialTruckCost_ = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->tCost = true;
	}
	else if (name == "Boat, cost, special") {
		fleetRepReactor_->specialBoatCost_ = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->bCost = true;
	}
	else if (name == "Plane, cost, special") {
		fleetRepReactor_->specialPlaneCost_ = Fleet::Cost(atof(v.c_str()));
		fleetRepReactor_->pCost = true;
	}
	else if (name == "Truck, capacity, special") {
		fleetRepReactor_->specialTruckCapacity_ = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->tCapacity = true;
	}
	else if (name == "Boat, capacity, special") {
		fleetRepReactor_->specialBoatCapacity_ = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->bCapacity = true;
	}
	else if (name == "Plane, capacity, special") {
		fleetRepReactor_->specialPlaneCapacity_ = Fleet::Capacity(atoi(v.c_str()));
		fleetRepReactor_->pCapacity = true;
	}
	else if (name == "Start time, special") {
		fleetRepReactor_->startTime = Time(atof(v.c_str()));
		fleetRepReactor_->start = true;
		if (fleetRepReactor_->end = true) fleetRepReactor_->onAttributeIs();
	}
	else if (name == "End time, special") {
		fleetRepReactor_->endTime = Time(atof(v.c_str()));
		fleetRepReactor_->end = true;
		if (fleetRepReactor_->start = true) fleetRepReactor_->onAttributeIs();
	}	
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}
