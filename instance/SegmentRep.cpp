#include "InstanceImpl.h"

namespace Shipping {

string SegmentRep::attribute(const string& name) {

  if (name == "source") {
      Location::PtrConst l = segment_->source();
      if(l) return l->name();
      return "";
  }
	else if (name == "length")
		return Util::FloatToString(segment_->length().value());
	else if (name == "return segment") {
    Segment::PtrConst s = segment_->returnSegment();
    if(s) return s->name();
    return "";
  }
	else if (name == "difficulty")
		return Util::FloatToString(segment_->difficulty().value());
	else if (name == "expedite support") {
		if (segment_->expeditedState() == Segment::expedited())
			return "yes";
		else if (segment_->expeditedState() == Segment::notExpedited())
			return "no";
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	return "";
}

void SegmentRep::attributeIs(const string& name, const string& v) {
	
  if (name == "source") {
		Ptr<LocationRep> ptr = dynamic_cast<LocationRep *>(manager_->instance(v).ptr());
		if (ptr) segment_->sourceIs(ptr->location());
		else {
			cerr << "Instance given to 'source' is not a location: " << v << endl;
			throw Fwk::EntityNotFoundException("Instance given to 'source is not a location.");
		}
	}
	else if (name == "length")
		segment_->lengthIs(Segment::Length(atof(v.c_str())));
	else if (name == "return segment") {
		if (v == "") {
			segment_->returnSegmentIs(NULL);
			return;
		}
		Ptr<SegmentRep> ptr = dynamic_cast<SegmentRep *>(manager_->instance(v).ptr());
		if (ptr)
			segment_->returnSegmentIs(ptr->segment());
		else {
			cerr << "Instance given to 'return segment' is not a segment: " << v << endl;
			throw Fwk::EntityNotFoundException("Instance given to 'return segment is not a segment.");
		}
	}
	else if (name == "difficulty")
		segment_->difficultyIs(Segment::Difficulty(atof(v.c_str())));
	else if (name == "expedite support") {
		if (v == "yes")
			segment_->expeditedIs(Segment::expedited());
		else if (v == "no")
			segment_->expeditedIs(Segment::notExpedited());
		else {
			cerr << "Invalid expedite support: " << v << endl;
			throw Fwk::UnknownArgException("Invalid expedite support.");
		}
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}