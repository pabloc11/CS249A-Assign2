#include "InstanceImpl.h"

namespace Shipping {

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

string LocationRep::attribute(const string& name) {
	
	Ptr<CustomerLocation> customerLocation = dynamic_cast<CustomerLocation *>(this->location().ptr());
	
  int i = segmentNumber(name);
  if (i != 0) {
		if (location_->segment(i)) return location_->segment(i)->name();
		else {
			cerr << "Segment not found with index: " << i << endl;
			throw Fwk::EntityNotFoundException("Segment not found with given index.");
		}
  }

	// Instance is a customer location
	else if (customerLocation && name == "transfer rate") {
		return Util::IntToString(customerLocation->transferRate().value());
	}
	else if (customerLocation && name == "shipment size") {
		return Util::IntToString(customerLocation->shipmentSize().value());
	}
	else if (customerLocation && name == "destination") {
		return customerLocation->destination()->name();
	}
	else if (customerLocation && name == "shipments received") {
		return Util::IntToString(customerLocation->shipmentsReceived().value());
	}
	else if (customerLocation && name == "average latency") {
		return Util::FloatToString(customerLocation->averageLatency().value());
	}
	else if (customerLocation && name == "total cost") {
		return Util::FloatToString(customerLocation->totalCost().value());
	}	
	
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
		
  return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
	
	CustomerLocation* customerLocation = dynamic_cast<CustomerLocation *>(location_.ptr());
	
	// Instance is a customer location
	if (customerLocation && name == "transfer rate") {
		customerLocation->transferRateIs(ShipmentsPerDay(atoi(v.c_str())));
	}
	else if (customerLocation && name == "shipment size") {
		customerLocation->shipmentSizeIs(PackagesPerShipment(atoi(v.c_str())));
	}
	else if (customerLocation && name == "destination") {
		Ptr<CustomerLocationRep> ptr = dynamic_cast<CustomerLocationRep *>(manager_->instance(v).ptr());
		Ptr<CustomerLocation> destination = dynamic_cast<CustomerLocation *>(ptr->location().ptr());
		if (destination)
			customerLocation->destinationIs(destination);
		else {
			cerr << "Instance given to 'destination' is not a customer location: " << v << endl;
			throw Fwk::EntityNotFoundException("Instance given to 'destination' is not a customer location.");
		}
	}
	
  else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t);
    }
    return 0;
}

}