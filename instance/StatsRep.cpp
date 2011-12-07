#include "InstanceImpl.h"

namespace Shipping {

string StatsRep::attribute(const string& name) {
	
	if (name == "Customer")
		return Util::IntToString(stats_->customerLocationCount());
	else if (name == "Port")
		return Util::IntToString(stats_->portLocationCount());
	else if (name == "Truck terminal")
		return Util::IntToString(stats_->truckTerminalCount());
	else if (name == "Boat terminal")
		return Util::IntToString(stats_->boatTerminalCount());
	else if (name == "Plane terminal")
		return Util::IntToString(stats_->planeTerminalCount());
	else if (name == "Truck segment")
		return Util::IntToString(stats_->truckSegmentCount());
	else if (name == "Boat segment")
		return Util::IntToString(stats_->boatSegmentCount());
	else if (name == "Plane segment")
		return Util::IntToString(stats_->planeSegmentCount());
	else if (name == "expedite percentage")
		return Util::FloatToString(stats_->expeditePercentage().value()*100);
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
		
	return "";
}

void StatsRep::attributeIs(const string& name, const string& v) {
	// nothing to do
}

}