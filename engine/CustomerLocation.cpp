#include "Engine.h"

namespace Shipping {
  CustomerLocation::CustomerLocation(Fwk::String _name) :
    Location(_name, Entity::customerLocation()),
    transferRate_(0),
    shipmentSize_(0),
    destination_(NULL),
    shipmentsReceived_(0),
    averageLatency_(0),
    totalCost_(0)
  {}

  void CustomerLocation::transferRateIs(ShipmentsPerDay _transferRate) {
	transferRate_ = _transferRate;
	if(notifiee_) {
	  try {
		notifiee_->onTransferRate(transferRate_);
	  } catch(...) {}
	}
  }

  void CustomerLocation::shipmentSizeIs(PackagesPerShipment _shipmentSize) {
    shipmentSize_ = _shipmentSize;
	if(notifiee_) {
	  try {
		notifiee_->onShipmentSize(shipmentSize_);
	  } catch(...) {}
	}
  }

  void CustomerLocation::destinationIs(CustomerLocation::Ptr _destination) {
    destination_ = _destination;
	if(notifiee_) {
	  try {
		notifiee_->onDestination(destination_);
	  } catch(...) {}
	}
  }

  void CustomerLocation::Notifiee::notifierIs(CustomerLocation::Ptr& _notifier) {
    CustomerLocation::Ptr notifierSave(notifier_.ptr());
    if(notifier_==_notifier)
      return;
    notifier_ = _notifier;
    if(notifierSave)
      notifierSave->notifiee_ == NULL;
    if(_notifier)
      _notifier->notifiee_ = this;
  }

  CustomerLocation::Notifiee::~Notifiee() {
    if(notifier_) {
      notifier_->notifiee_ = NULL;
    }
  }
}
