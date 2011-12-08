#ifndef REACTORS_H
#define REACTORS_H

#include "Engine.h"
#include "Activity.h"

using namespace Shipping;

namespace Shipping {
  class SegmentReactor : public Segment::Notifiee {
  public:
	typedef Fwk::Ptr<SegmentReactor> Ptr;
    void onExpedited(Segment::Expedited _expedited);
    void onShipmentNew(Shipment::Ptr _ptr);
    SegmentReactor(Segment::Ptr _s, Stats::Ptr _stats);
  private:
    Fwk::Ptr<Stats> stats_;
  };

  class NetworkReactor : public Network::Notifiee {
  public:
	 	typedef Fwk::Ptr<NetworkReactor> Ptr;
    void onEntityNew(Entity::Ptr _ptr);
    void onEntityDel(Entity::Ptr _ptr);
    NetworkReactor(Network::Ptr _n, Stats::Ptr _stats);
  private:
    Fwk::Ptr<Stats> stats_;
  };

  class LocationReactor : public Location::Notifiee {
  public:
    typedef Fwk::Ptr<LocationReactor> Ptr;
    void onShipmentNew(Shipment::Ptr _ptr);
    LocationReactor(Location::Ptr _l);
  private:
  };

  class CustomerReactor : public CustomerLocation::Notifiee {
  public:
    typedef Fwk::Ptr<CustomerReactor> Ptr;
    void onTransferRate(ShipmentsPerDay _transferRate);
    void onShipmentSize(PackagesPerShipment _shipmentSize);
    void onDestination(CustomerLocation::Ptr _destination);
    CustomerReactor(CustomerLocation::Ptr _l, Activity::Manager::Ptr _a);
  private:
    void createOrUpdateActivity();
		Activity::Manager::Ptr activityManager_;
		Activity::Ptr injectActivity_;
    bool transferRateInit_;
    bool shipmentSizeInit_;
    bool destinationInit_;
		ShipmentsPerDay transferRate_;
		PackagesPerShipment shipmentSize_;
		CustomerLocation::Ptr destination_;
  };
}

class InjectActivityReactor : public Activity::Notifiee {
public:
	typedef Fwk::Ptr<InjectActivityReactor> Ptr;
	InjectActivityReactor(Activity::Ptr _a);
	void transferRateIs(ShipmentsPerDay n);
	void shipmentSizeIs(PackagesPerShipment n);
	void destinationIs(CustomerLocation::Ptr l);
	void onNextTime();
	void onStatus();	
private:
	Activity::Ptr activity_;
	ShipmentsPerDay transferRate_;
	PackagesPerShipment shipmentSize_;
	CustomerLocation::Ptr destination_;
};

class ForwardActivityReactor : public Activity::Notifiee {
public:
	typedef Fwk::Ptr<ForwardActivityReactor> Ptr;
	ForwardActivityReactor(Activity* _a);
	void onNextTime();
	void onStatus();	
};

#endif
