#ifndef REACTORS_H
#define REACTORS_H

#include "Engine.h"
#include "Activity.h"

using namespace Shipping;

class InjectActivityReactor : public Activity::Notifiee {
public:
	typedef Fwk::Ptr<InjectActivityReactor> Ptr;
	InjectActivityReactor(Activity::Ptr _a, CustomerLocation::Ptr _l);
	void transferRateIs(ShipmentsPerDay n);
	void shipmentSizeIs(PackagesPerShipment n);
	void destinationIs(CustomerLocation::Ptr l);
	void onNextTime();
	void onStatus();	
private:
	Activity::Ptr activity_;
	ShipmentsPerDay transferRate_;
	PackagesPerShipment shipmentSize_;
	CustomerLocation::Ptr source_;
	CustomerLocation::Ptr destination_;
};

class ForwardActivityReactor : public Activity::Notifiee {
public:
	typedef Fwk::Ptr<ForwardActivityReactor> Ptr;
	ForwardActivityReactor(Activity* _a, Fleet::Ptr _fleet, Stats::Ptr _stats, Segment::Ptr _seg, Shipment::Ptr _ship);
	void onNextTime();
	void onStatus();	
private:
	Activity::Ptr activity_;
	Fleet::Ptr fleet_;
	Stats::Ptr stats_;
	Segment::Ptr segment_;
	Shipment::Ptr shipment_;
};

namespace Shipping {
  class SegmentReactor : public Segment::Notifiee {
  public:
	typedef Fwk::Ptr<SegmentReactor> Ptr;
    void onExpedited(Segment::Expedited _expedited);
    void onActiveShipmentNew(Shipment::Ptr _ptr);
    SegmentReactor(Segment::Ptr _s, Fleet::Ptr _fleet, Stats::Ptr _stats);
  private:
		Segment::Ptr segment_;
		Fleet::Ptr fleet_;
    Fwk::Ptr<Stats> stats_;
		Activity::Ptr forwardActivity_;
		ForwardActivityReactor::Ptr reactor_;
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
    CustomerReactor(CustomerLocation::Ptr _l);
  private:
    void createOrUpdateActivity();
		Activity::Ptr injectActivity_;
		InjectActivityReactor::Ptr reactor_;
    bool transferRateInit_;
    bool shipmentSizeInit_;
    bool destinationInit_;
		ShipmentsPerDay transferRate_;
		PackagesPerShipment shipmentSize_;
		CustomerLocation::Ptr source_;
		CustomerLocation::Ptr destination_;
  };

  class FleetRepReactor : public Fwk::PtrInterface<FleetRepReactor>{
  public:
		FleetRepReactor();
		typedef Fwk::Ptr<FleetRepReactor> Ptr;
		void onAttributeIs();
	private:
		friend class FleetRep;
		Fleet::Speed truckSpeed_, boatSpeed_, planeSpeed_, specialTruckSpeed_, specialBoatSpeed_, specialPlaneSpeed_;
		Fleet::Cost truckCost_, boatCost_, planeCost_, specialTruckCost_, specialBoatCost_, specialPlaneCost_;
		Fleet::Capacity truckCapacity_, boatCapacity_, planeCapacity_, specialTruckCapacity_, specialBoatCapacity_, specialPlaneCapacity_;
		bool tSpeed, bSpeed, pSpeed, tCost, bCost, pCost, tCapacity, bCapacity, pCapacity, start, end;
		Time startTime, endTime;
  };
}

#endif
