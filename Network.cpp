#include "Engine.h"
#include "fwk/Exception.h"

namespace Shipping {
  Network::Network(Fwk::String _name) : 
    Fwk::NamedInterface(_name),
    stats_(new Stats(this)),
    fleet_(new Fleet()),
    notifiee_(NULL) 
    {}
  
  Entity::Ptr Network::entityDel(Fwk::String _name){
    Entity::Ptr m = entity_.deleteMember(_name);
    if(!m) return NULL;
    if(notifiee_) { 
      try {
        notifiee_->onEntityDel(m);
      } catch(...) {}
    }
    return m; 
  }
  
  Entity::Ptr Network::entityIs(Entity::Ptr _ptr){
    Fwk::String name = _ptr->name();
    Entity::Ptr m = entity_[name];
    if(m) {
      throw Fwk::NameInUseException(name);
    } else {
      m = _ptr;
      entity_.newMember(m);
    }
    if(notifiee_) { 
      try {
        notifiee_->onEntityNew(m);
      } catch(...) {}
    }
    return _ptr;
  }
  
  void Network::NotifieeConst::notifierIs(const Network::PtrConst& _notifier) {
    Network::Ptr notifierSave(const_cast<Network *>(notifier_.ptr()));
    if(_notifier==notifier_)
      return;
    notifier_ = _notifier;
    notifierSave->notifiee_ = this;
  }
  
  Network::NotifieeConst::~NotifieeConst() {
    if(notifier_) {
      Network::Ptr notifierSave(const_cast<Network *>(notifier_.ptr()));
      notifierSave->notifiee_ = NULL;
    }
  }
}

 Fwk::String valueToStrep(Fwk::String s) { return s; }
