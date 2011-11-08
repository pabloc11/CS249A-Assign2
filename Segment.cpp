#include "Engine.h"
#include "fwk/Exception.h"

namespace Shipping {
  Segment::Segment(Fwk::String _name, Entity::EntityType _type) : 
    Entity(_name, _type),
    source_(NULL),
    returnSegment_(NULL),
		length_(0.0f),
    difficulty_(1.0f),
    expeditedState_(Segment::notExpedited())
  {}
    
  void Segment::sourceIs(Fwk::Ptr<Location> _source) {
    if(source_ == _source)
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
    if(_returnSegment->entityType() != this->entityType())
      throw Fwk::InternalException("A segment and its return segment must have the same entity type");
    
    if(returnSegment_ == _returnSegment)
      return;
      
    if(_returnSegment) {
      _returnSegment->returnSegment_ = this;
    }
    else {
      _returnSegment->returnSegment_ = NULL;
    }
    returnSegment_ = _returnSegment;
     
  }

	void Segment::lengthIs(Length _length) {
		length_ = _length;
	}
  
  void Segment::difficultyIs(Difficulty _difficulty) {
    difficulty_ = _difficulty;
  }
  
  void Segment::expeditedIs(Expedited _expedited) {
    if(expeditedState_ == _expedited)
      return;
    expeditedState_ = _expedited;
    if(notifiee_) {
      try {
        const_cast<Segment::NotifieeConst *>(notifiee_.ptr())->onExpedited(_expedited);
      } catch(...) {}
    }
  }
  
  void Segment::NotifieeConst::notifierIs(const Segment::PtrConst& _notifier) {
    Segment::Ptr notifierSave(const_cast<Segment *>(notifier_.ptr()));
    if(notifier_==_notifier)
      return;
    notifier_ = _notifier;
    if(notifierSave)
      notifierSave->notifiee_ == NULL;
    if(_notifier)
      (const_cast<Segment*>(_notifier.ptr()))->notifiee_ = this;
  }
  
  Segment::NotifieeConst::~NotifieeConst() {
    if(notifier_) {
      (const_cast<Segment*>(notifier_.ptr()))->notifiee_ = NULL;
    }
  }
}
