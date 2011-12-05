#ifndef REACTORS_H
#define REACTORS_H

#include "Engine.h"

namespace Shipping {
  class SegmentReactor : public Segment::Notifiee {
  public:
	typedef Fwk::Ptr<SegmentReactor> Ptr;
    void onExpedited(Segment::Expedited _expedited);
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

    LocationReactor(Location::Ptr _l);
  private:
  };

  class CustomerReactor : public CustomerLocation::Notifiee {
  public:
    typedef Fwk::Ptr<CustomerReactor> Ptr;

    CustomerReactor(CustomerLocation::Ptr _l);
  private:
  };
}

#endif
