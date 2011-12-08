#include "Engine.h"

namespace Shipping {
  Location::Location(Fwk::String _name, Entity::EntityType _type) :
    Entity(_name, _type),
    segment_()
  {}
  
  Segment::PtrConst Location::segment(unsigned _index) const {
    unsigned i = 1;
    for( SegmentList::IteratorConst iter = segment_.iterator(); iter.ptr() && i <= _index; ++iter, i++) {
      if(i == _index)
        return iter.ptr();
    }
    return NULL;
  }

  Segment::Ptr Location::segment(unsigned _index) {
    unsigned i = 1;
    for( SegmentList::Iterator iter = segment_.iterator(); iter.ptr() && i <= _index; ++iter, i++) {
      if(i == _index)
        return iter.ptr();
    }
    return NULL;
  }

  void Location::shipmentIs(Shipment::Ptr _ptr) {
	Fwk::String name = _ptr->name();
	Shipment::Ptr m = shipment_[name];
	if(m) {
	  throw Fwk::NameInUseException(name);
	} else {
	  shipment_.newMember(_ptr);
	}
	if(notifiee_) {
	  try {
		notifiee_->onShipmentNew(_ptr);
	  } catch(...) {}
	}
  }

  Shipment::Ptr Location::shipmentDel(Fwk::String _name){
    Shipment::Ptr m = shipment_.deleteMember(_name);
    if(!m) return NULL;
    return m;
  }

  void Location::routeIs(Fwk::String _name, Segment::Ptr _ptr) {
	routes_[_name] = _ptr;
  }
  Segment::Ptr Location::routeDel(Fwk::String _name) {
	Segment::Ptr m = routes_[_name];
	routes_[_name] = NULL;
	return m;
  }

  void Location::Notifiee::notifierIs(Location::Ptr& _notifier) {
    Location::Ptr notifierSave(notifier_.ptr());
    if(notifier_==_notifier)
      return;
    notifier_ = _notifier;
    if(notifierSave)
      notifierSave->notifiee_ == NULL;
    if(_notifier)
      _notifier->notifiee_ = this;
  }

  Location::Notifiee::~Notifiee() {
    if(notifier_) {
      notifier_->notifiee_ = NULL;
    }
  }
}
