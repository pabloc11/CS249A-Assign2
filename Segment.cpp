#include "Engine.h"
#include "fwk/Exception.h"

namespace Shipping {
  Segment::Segment(Fwk::String _name, Entity::EntityType _type) : 
    Entity(_name, _type),
    source_(NULL),
    returnSegment_(NULL),
    difficulty_(1.0f),
    expeditedState_(Segment::notExpedited())
  {}
    
  void Segment::sourceIs(Fwk::Ptr<Location const> _source) {
    if(source_ == _source)
      return;
    source_ = _source;
    //TODO: add to Location array
  }
  
  void Segment::returnSegmentIs(Segment::Ptr _returnSegment) {
    returnSegment_ = _returnSegment;
    _returnSegment->returnSegment_ = this;
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
        notifiee_->onExpedited(_expedited);
      } catch(...) {}
    }
  }
  
  void Segment::NotifieeConst::notifierIs(const Segment::PtrConst& _notifier) {
    Segment::Ptr notifierSave(const_cast<Segment *>(notifier_.ptr()));
    if(_notifier==notifier_)
      return;
    notifier_ = _notifier;
    notifierSave->notifiee_ = this;
  }
  
  Segment::NotifieeConst::~NotifieeConst() {
    if(notifier_) {
      Segment::Ptr notifierSave(const_cast<Segment *>(notifier_.ptr()));
      notifierSave->notifiee_ = NULL;
    }
  }
}
