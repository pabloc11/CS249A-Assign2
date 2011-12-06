#ifndef REACTORS_H
#define REACTORS_H

#include "Engine.h"

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
    void onTransferRate(ShipmentsPerDay transferRate_);
    void onShipmentSize(PackagesPerShipment shipmentSize_);
    void onDestination(Location::Ptr destination_);
    CustomerReactor(CustomerLocation::Ptr _l);
  private:
  };
}

#endif
