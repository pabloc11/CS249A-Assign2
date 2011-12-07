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
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}