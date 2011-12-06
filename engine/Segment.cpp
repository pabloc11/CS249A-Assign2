#include "Engine.h"
#include "fwk/Exception.h"

namespace Shipping {
  Segment::Segment(Fwk::String _name, Entity::EntityType _type) : 
    Entity(_name, _type),
    source_(NULL),
    returnSegment_(NULL),
	length_(0.0f),
    difficulty_(1.0f),
    expeditedState_(Segment::notExpedited()),
    shipmentsReceived_(0),
    shipmentsRefused_(0),
    capacity_(10)
  {}
    
  void Segment::sourceIs(Fwk::Ptr<Location> _source) {
    if(source_ == _source)
      return;
    
    if(_source && _source->entityType() == Entity::truckTerminal() && this->entityType() != Entity::truckSegment())
        return;

    if(_source && _source->entityType() == Entity::planeTerminal() && this->entityType() != Entity::planeSegment())
        return;

    if(_source && _source->entityType() == Entity::boatTerminal() && this->entityType() != Entity::boatSegment())
        return;

    if(source_) {
      //remove from Location array
      source_->segment_.deleteMember(this);
    }
     
    if(_source) {
      // add to end of Location array
      _source->segment_.newMember(this);
    }
    
    source_ = _source;
  }
  
  void Segment::returnSegmentIs(Segment::Ptr _returnSegment) {
	
		if (_returnSegment == NULL) {
			returnSegment_->returnSegment_ = NULL;
			returnSegment_ = NULL;
			return;
		}
	
    if(_returnSegment->entityType() != this->entityType()) {
			cerr << "A segment and its return segment must have the same entity type." << endl;
      throw Fwk::InternalException("A segment and its return segment must have the same entity type");
		}
    
    if(returnSegment_ == _returnSegment)
      return;
      
    if(_returnSegment) {
      _returnSegment->returnSegment_ = this;
    }
    returnSegment_ = _returnSegment;
     
  }

	void Segment::lengthIs(Length _length) {
		length_ = _length;
	}
  
  void Segment::difficultyIs(Difficulty _difficulty) {
    difficulty_ = _difficulty;
  }
  
  void Segment::capacityIs(NumShipments _capacity) {
    capacity_ = _capacity;
  }

  void Segment::expeditedIs(Expedited _expedited) {
    if(expeditedState_ == _expedited)
      return;
    expeditedState_ = _expedited;
    if(notifiee_) {
      try {
        notifiee_->onExpedited(_expedited);
      } catch(...) {}
    }
  }
  
  Shipment::Ptr Segment::queuedShipment(Fwk::String _name) {
    for(ShipmentQueueIterator i = shipmentQueue_.begin(); i != shipmentQueue_.end(); ++i) {
    	if ((*i)->name() == _name) return *i;
    }
    return NULL;
  }

  void Segment::queuedShipmentIs(Shipment::Ptr _ptr) {
	if(activeShipments() < (U32)capacity_.value()) {
	  activeShipmentIs(_ptr);
	}
	else if (this->queuedShipment(_ptr->name()) == NULL) {
	  shipmentQueue_.push_back(_ptr);
	  ++shipmentsRefused_;
	}
  }

  Shipment::Ptr Segment::queuedShipmentDel(Fwk::String _name) {
	for(ShipmentQueueIterator i = shipmentQueue_.begin(); i != shipmentQueue_.end(); ++i) {
		if ((*i)->name() == _name) {
			shipmentQueue_.erase(i);
			return *i;
		}
	}
	return NULL;
  }

  void Segment::activeShipmentIs(Shipment::Ptr _ptr) {
	++shipmentsReceived_;
	Fwk::String name = _ptr->name();
	Shipment::Ptr m = activeShipment_[name];
	if(m) {
	  throw Fwk::NameInUseException(name);
	} else {
	  m = _ptr;
	  activeShipment_.newMember(m);
	}
	if(notifiee_) {
	  try {
		notifiee_->onActiveShipmentNew(_ptr);
	  } catch(...) {}
	}
  }

  Fwk::Ptr<Shipment> Segment::activeShipmentDel(Fwk::String _name) {
	Shipment::Ptr m = activeShipment_.deleteMember(_name);
	if(!m) {
	  return NULL;
	}
	if(!shipmentQueue_.empty()) {
		Shipment::Ptr ptr = shipmentQueue_.back();
		shipmentQueue_.pop_back();
		activeShipmentIs(ptr);
	}
	return m;
  }

  void Segment::Notifiee::notifierIs(Segment::Ptr& _notifier) {
    Segment::Ptr notifierSave(notifier_.ptr());
    if(notifier_==_notifier)
      return;
    notifier_ = _notifier;
    if(notifierSave)
      notifierSave->notifiee_ == NULL;
    if(_notifier)
      _notifier->notifiee_ = this;
  }
  
  Segment::Notifiee::~Notifiee() {
    if(notifier_) {
      notifier_->notifiee_ = NULL;
    }
  }
}
