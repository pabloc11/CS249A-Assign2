#include "InstanceImpl.h"

namespace Shipping {

static const string segmentStr = "segment";
static const int segmentStrlen = segmentStr.length();

string LocationRep::attribute(const string& name) {
	
  int i = segmentNumber(name);
  if (i != 0) {
		if (location_->segment(i)) return location_->segment(i)->name();
		else {
			cerr << "Segment not found with index: " << i << endl;
			throw Fwk::EntityNotFoundException("Segment not found with given index.");
		}
  }
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
		
  return "";
}


void LocationRep::attributeIs(const string& name, const string& v) {
    //nothing to do
}

int LocationRep::segmentNumber(const string& name) {
    if (name.substr(0, segmentStrlen) == segmentStr) {
        const char* t = name.c_str() + segmentStrlen;
        return atoi(t);
    }
    return 0;
}

}