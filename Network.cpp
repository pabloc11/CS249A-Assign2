#include "Engine.h"
#include "fwk/Exception.h"

namespace Shipping {
  Network::Network(Fwk::String _name) : 
    Fwk::NamedInterface(_name),
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
    if(notifier_==_notifier)
      return;
    notifier_ = _notifier;
    if(notifierSave)
      notifierSave->notifiee_ == NULL;
    if(_notifier)
      (const_cast<Network*>(_notifier.ptr()))->notifiee_ = this;
  }
  
  Network::NotifieeConst::~NotifieeConst() {
    if(notifier_) {
      (const_cast<Network*>(notifier_.ptr()))->notifiee_ = NULL;
    }
  }
}

 Fwk::String valueToStrep(Fwk::String s) { return s; }
