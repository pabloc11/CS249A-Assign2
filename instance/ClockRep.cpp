#include "InstanceImpl.h"

namespace Shipping {

string ClockRep::attribute(const string& name) {

	if (name == "type"){
		//return activityManager_->type()
	}
	else if (name == "now") {
		return Util::FloatToString(activityManager_->now().value());
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	return "";
}

void ClockRep::attributeIs(const string& name, const string& v) {

	if (name == "type"){
		//activityManager_->typeIs(...)
	}
	else if (name == "now") {
		//fleet_->planeCapacityIs(Fleet::Capacity(atoi(v.c_str())));
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}
