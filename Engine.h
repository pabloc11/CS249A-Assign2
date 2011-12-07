#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Nominal.h"
#include "fwk/NamedInterface.h"
#include "fwk/HashMap.h"
#include "fwk/ListRaw.h"
#include <deque>
#include <map>
#include <vector>
#include <set>
#include "Activity.h"

namespace Shipping {

/************************** ENTITY **************************/

  class Entity : public Fwk::NamedInterface
  {
  public:
    typedef Fwk::Ptr<Entity const> PtrConst;
    typedef Fwk::Ptr<Entity> Ptr;
    
    enum EntityType {
       truckSegment_ = 0,
       boatSegment_ = 1,
       planeSegment_ = 2,
       customerLocation_ = 3,
       portLocation_ = 4,
       truckTerminal_ = 5,
       boatTerminal_ = 6,
       planeTerminal_ = 7
    };

    EntityType entityType() const { return entityType_; }
  
    static inline EntityType truckSegment() { return truckSegment_; }
    static inline EntityType boatSegment() { return boatSegment_; }
    static inline EntityType planeSegment() { return planeSegment_; }
    static inline EntityType customerLocation() { return customerLocation_; }
    static inline EntityType portLocation() { return portLocation_; }
    static inline EntityType truckTerminal() { return truckTerminal_; }
    static inline EntityType boatTerminal() { return boatTerminal_; }
    static inline EntityType planeTerminal() { return planeTerminal_; }
    
    Entity const * fwkHmNext() const { return fwkHmNext_.ptr(); }
    Entity * fwkHmNext() { return fwkHmNext_.ptr(); }
    void fwkHmNextIs(Entity * _fwkHmNext) const {
      fwkHmNext_ = _fwkHmNext;
    }
    Fwk::String fwkKey() const { return name(); }

  protected:
    EntityType entityType_;
    mutable Entity::Ptr fwkHmNext_;

    Entity(Fwk::String _name, EntityType _entityType);
    Entity(const Entity&);
  };

/************************** NETWORK **************************/

  class Stats;
  class Fleet;

  class Network : public Fwk::NamedInterface
  {
  public:
    typedef Fwk::Ptr<Network const> PtrConst;
    typedef Fwk::Ptr<Network> Ptr;
    typedef Fwk::HashMap< Entity, Fwk::String, Entity, Entity::PtrConst, Entity::Ptr> EntityMap; 
  
    // Attribute Accessors
    inline Entity::PtrConst entity(Fwk::String _name) const { return entity_[_name]; }
    inline U32 entities() const { return entity_.members(); }
  
    // Attribute Mutators
    Entity::Ptr entityDel(Fwk::String _name);
    Entity::Ptr entityIs(Entity::Ptr _ptr);
  
    static Network::Ptr NetworkNew(Fwk::String _name) {
      Ptr m = new Network(_name);
      return m;   
    }

    class Notifiee : public virtual Fwk::NamedInterface::Notifiee
    {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      
      Network::Ptr notifier() const { return notifier_; }
      virtual void notifierIs(Network::Ptr& _notifier);
      
      virtual void onEntityNew(Entity::Ptr _ptr) {};
      virtual void onEntityDel(Entity::Ptr _ptr) {};
      
      ~Notifiee();
    protected:
      Notifiee() : Fwk::NamedInterface::Notifiee() {}
      Network::Ptr notifier_;
    };

  protected:  
    EntityMap entity_;
    Network(Fwk::String _name);
    Network( const Network& );
    
    Network::Notifiee::Ptr notifiee_;
  };

/************************** STATS **************************/
  class NetworkReactor;
  class SegmentReactor;

  class Stats : public Fwk::PtrInterface<Stats>
  {
  public:
    typedef Fwk::Ptr<Stats const> PtrConst;
    typedef Fwk::Ptr<Stats> Ptr;

		class Percent : public Ordinal<Percent, float>
		{
    public:
	  	Percent(float num) : Ordinal<Percent, float>(num) {
				if (num < 0.0f || num > 1.0f) {
					cerr << "Invalid value passed to Percent constructor: " << num << endl;
          throw Fwk::RangeException("Invalid value passed to Percent constructor.");
				}
				value_ = num;
		  }
		};

		inline U32 truckSegmentCount() const { return truckSegmentCount_; }
		inline U32 boatSegmentCount() const { return boatSegmentCount_; }
		inline U32 planeSegmentCount() const { return planeSegmentCount_; }
		inline U32 customerLocationCount() const { return customerLocationCount_; }
		inline U32 portLocationCount() const { return portLocationCount_; }
		inline U32 truckTerminalCount() const { return truckTerminalCount_; }
		inline U32 boatTerminalCount() const { return boatTerminalCount_; }
		inline U32 planeTerminalCount() const { return planeTerminalCount_; }
		inline Percent expeditePercentage() const {
			int total = truckSegmentCount_ + boatSegmentCount_ + planeSegmentCount_;
			if (total > 0)
				return Percent(expeditedSegmentCount_/(float)(total));
			else
				return Percent(0.0f);
		}

    static Stats::Ptr StatsNew(Network::Ptr _n) {
      Ptr m = new Stats(_n);
      return m;
    }

  protected:
    friend class NetworkReactor;
    friend class SegmentReactor;
    Stats(const Stats&);
    Stats(Network::Ptr);

    void truckSegmentCountInc();
    void boatSegmentCountInc();
    void planeSegmentCountInc();
    void customerLocationCountInc();
    void portLocationCountInc();
    void truckTerminalCountInc();
    void boatTerminalCountInc();
    void planeTerminalCountInc();
    void expeditedSegmentCountInc();

    void truckSegmentCountDec();
    void boatSegmentCountDec();
    void planeSegmentCountDec();
    void customerLocationCountDec();
    void portLocationCountDec();
    void truckTerminalCountDec();
    void boatTerminalCountDec();
    void planeTerminalCountDec();
    void expeditedSegmentCountDec();


	U32 expeditedSegmentCount_;

	U32 truckSegmentCount_;
	U32 boatSegmentCount_;
	U32 planeSegmentCount_;
	U32 customerLocationCount_;
	U32 portLocationCount_;
	U32 truckTerminalCount_;
	U32 boatTerminalCount_;
	U32 planeTerminalCount_;
  };

/************************** FLEET **************************/

  class Fleet : public Fwk::PtrInterface<Fleet>
  {
  public:
    typedef Fwk::Ptr<Fleet const> PtrConst;
    typedef Fwk::Ptr<Fleet> Ptr;
    
    class Speed : public Ordinal<Speed, float> {
    public:
      Speed(float num) : Ordinal<Speed, float>(num) {
				if(num < 0.0f) {
					cerr << "Invalid value passed to Speed constructor: " << num << endl;
					throw Fwk::RangeException("Invalid value passed to Speed constructor.");
				}
        value_ = num;
      }    
    };
    class Cost : public Ordinal<Cost, float> {
    public:
      Cost(float num) : Ordinal<Cost, float>(num) {
				if(num < 0.0f) {
					cerr << "Invalid value passed to Cost constructor: " << num << endl;
					throw Fwk::RangeException("Invalid value passed to Cost constructor.");
				}
        value_ = num;
      }    
    };
    class Capacity : public Ordinal<Capacity, int> {
    public:
      Capacity(int num) : Ordinal<Capacity, int>(num) {
				if(num < 0) {
					cerr << "Invalid value passed to Capacity constructor: " << num << endl;
					throw Fwk::RangeException("Invalid value passed to Capacity constructor.");
				}
        value_ = num;
      }    
    };

    inline Fleet::Speed truckSpeed() const { return truckSpeed_; }
    inline Fleet::Speed boatSpeed() const { return boatSpeed_; }
    inline Fleet::Speed planeSpeed() const { return planeSpeed_; }
    inline Fleet::Cost truckCost() const { return truckCost_; }
    inline Fleet::Cost boatCost() const { return boatCost_; }
    inline Fleet::Cost planeCost() const { return planeCost_; }
    inline Fleet::Capacity truckCapacity() const { return truckCapacity_; }
    inline Fleet::Capacity boatCapacity() const { return boatCapacity_; }
    inline Fleet::Capacity planeCapacity() const { return planeCapacity_; }

    void truckSpeedIs(Fleet::Speed _truckSpeed);
    void boatSpeedIs(Fleet::Speed _boatSpeed);
    void planeSpeedIs(Fleet::Speed _planeSpeed);
    void truckCostIs(Fleet::Cost _truckCost);
    void boatCostIs(Fleet::Cost _boatCost);
    void planeCostIs(Fleet::Cost _planeCost);
    void truckCapacityIs(Fleet::Capacity _truckCapacity);
    void boatCapacityIs(Fleet::Capacity _boatCapacity);
    void planeCapacityIs(Fleet::Capacity _planeCapacity);
  
    static Fleet::Ptr FleetNew() {
      Ptr m = new Fleet();
      return m;   
    }
  
  protected:
    Fleet();
    Fleet(const Fleet&);
    Speed truckSpeed_, boatSpeed_, planeSpeed_;
    Cost truckCost_, boatCost_, planeCost_;
    Capacity truckCapacity_, boatCapacity_, planeCapacity_;
  };

  class Location;

/************************** SEGMENT **************************/
  class Shipment;

  class NumShipments : public Ordinal<NumShipments, int> {
	public:
  	NumShipments(int num) : Ordinal<NumShipments, int>(num) {
		if(num < 0) {
			cerr << "Invalid value passed to NumShipments constructor: " << num << endl;
		  throw Fwk::RangeException("Invalid value passed to NumShipments constructor.");
		}
		value_ = num;
	  }
	};

  class Segment : public Entity 
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;
    typedef deque<Fwk::Ptr<Shipment> > ShipmentQueue;
    typedef deque<Fwk::Ptr<Shipment> >::iterator ShipmentQueueIterator;
    typedef Fwk::HashMap< Shipment, Fwk::String, Shipment, Fwk::Ptr<Shipment const>, Fwk::Ptr<Shipment> > ShipmentMap;

	class Length : public Ordinal<Length, float>
	{
	public:
		Length(float num) : Ordinal<Length, float>(num)
		{
			if(num < 0.0f) {
				cerr << "Invalid value passed to Length constructor: " << num << endl;
				throw Fwk::RangeException("Invalid value passed to Length constructor.");
			}
			value_ = num;
		}
	};

    class Difficulty : public Ordinal<Difficulty, float> 
    {
    public:
      Difficulty(float num) : Ordinal<Difficulty, float>(num)
      {
        if(num < 1.0f || num > 5.0f) {
					cerr << "Invalid value passed to Difficulty constructor: " << num << endl;
          throw Fwk::RangeException("Invalid value passed to Difficulty constructor.");
				}
        value_ = num;
      }    
    };
   
    enum Expedited
    {
      expedited_ = 1,
      notExpedited_ = 0
    };
    static inline Expedited expedited() { return expedited_; }
    static inline Expedited notExpedited() { return notExpedited_; }

    // Attribute Accessors
    Fwk::Ptr<Location> source() const { return source_; }
    Segment::Ptr returnSegment() const { return returnSegment_; }
	Length length() const { return length_; }
    Difficulty difficulty() const { return difficulty_; }
    Expedited expeditedState() const { return expeditedState_; }
    NumShipments shipmentsReceived() const { return shipmentsReceived_; }
    NumShipments shipmentsRefused() const { return shipmentsRefused_; }
    NumShipments capacity() const { return capacity_; }
    

    // Attribute Mutators
    void sourceIs(Fwk::Ptr<Location> _source);
    void returnSegmentIs(Segment::Ptr _returnSegment);
	void lengthIs(Length _length);
    void difficultyIs(Difficulty _difficulty);
    void expeditedIs(Expedited _expedited);
    void capacityIs(NumShipments _capacity);

    Fwk::Ptr<Shipment> queuedShipment(Fwk::String _name);
    inline U32 queuedShipments() const { return shipmentQueue_.size(); }
    void queuedShipmentIs(Fwk::Ptr<Shipment> _ptr);
    Fwk::Ptr<Shipment> queuedShipmentDel(Fwk::String _name);

    inline Fwk::Ptr<Shipment> activeShipment(Fwk::String _name) { return activeShipment_[_name]; }
    inline U32 activeShipments() const { return activeShipment_.members(); }
    Fwk::Ptr<Shipment> activeShipmentDel(Fwk::String _name);
    
    // Notifier Class
    class Notifiee : public virtual Fwk::NamedInterface::Notifiee
    {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;
      
      Segment::Ptr notifier() const { return notifier_; }
      virtual void notifierIs(Segment::Ptr& _notifier);
      
      virtual void onActiveShipmentNew(Fwk::Ptr<Shipment> _ptr) {};

      virtual void onExpedited(Segment::Expedited _expedited) {}
      
      ~Notifiee();
    protected:
      Notifiee() : Fwk::NamedInterface::Notifiee() {}
      Segment::Ptr notifier_;
	  };
 
    Segment const * fwkHmNext() const { return dynamic_cast<Segment const *>(fwkHmNext_.ptr()); }
    Segment * fwkHmNext() { return dynamic_cast<Segment *>(fwkHmNext_.ptr()); }
    void fwkHmNextIs(Segment * _fwkHmNext) const {
      fwkHmNext_ = _fwkHmNext;
    }
 
    Segment * lrNext() const { return lrNext_; }
    void lrNextIs(Segment * _lrNext) {
         lrNext_ = _lrNext;
    }
  
  protected:
    // Constructors
    Segment( const Segment& );
    Segment(Fwk::String _name, Entity::EntityType _type);

    void activeShipmentIs(Fwk::Ptr<Shipment> _ptr);

    // Attributes
    Fwk::Ptr<Location> source_;
    Segment::Ptr returnSegment_;
	Length length_;
    Difficulty difficulty_;
    Expedited expeditedState_;
    NumShipments shipmentsReceived_;
    NumShipments shipmentsRefused_;
    NumShipments capacity_;
    
    ShipmentMap activeShipment_;
    ShipmentQueue shipmentQueue_;

    Segment::Notifiee::Ptr notifiee_;
    Segment *lrNext_;
  };

/************************** TRUCK SEGMENT **************************/

  class TruckSegment : public Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static TruckSegment::Ptr TruckSegmentNew(Fwk::String _name) {
      TruckSegment::Ptr m = new TruckSegment(_name);
      return m;
    }
 
  protected:
    // Constructors
    TruckSegment( const TruckSegment& );
    TruckSegment(Fwk::String _name);
  };

/************************** BOAT SEGMENT **************************/

  class BoatSegment : public Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static BoatSegment::Ptr BoatSegmentNew(Fwk::String _name) {
      BoatSegment::Ptr m = new BoatSegment(_name);
      return m;
    }

  protected:
    // Constructors
    BoatSegment( const BoatSegment& );
    BoatSegment(Fwk::String _name);
  };

/************************** PLANE SEGMENT **************************/

  class PlaneSegment : public Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static PlaneSegment::Ptr PlaneSegmentNew(Fwk::String _name) {
      PlaneSegment::Ptr m = new PlaneSegment(_name);
      return m;
    }

  protected:
    // Constructors
    PlaneSegment( const PlaneSegment& );
    PlaneSegment(Fwk::String _name);
  };

  /************************** LOCATION **************************/
  
  class Location : public Entity {
    
  public:
    typedef Fwk::Ptr<Location const> PtrConst;
    typedef Fwk::Ptr<Location> Ptr;
    typedef Fwk::ListRaw<Segment> SegmentList;
    typedef SegmentList::IteratorConst SegmentListIteratorConst;
    typedef SegmentList::Iterator SegmentListIterator;
    typedef Fwk::HashMap< Shipment, Fwk::String, Shipment, Fwk::Ptr<Shipment const>, Fwk::Ptr<Shipment> > ShipmentMap;
    typedef map<Fwk::String, Segment::Ptr> RouteMap;
    
    // Attribute Accessors
    Segment::PtrConst segment(unsigned _index) const;
    Segment::Ptr segment(unsigned _index);
    inline U32 segments() { return segment_.members(); }
    inline SegmentListIteratorConst segmentIterConst() const { return segment_.iterator(); }
    inline SegmentListIterator segmentIter() { return segment_.iterator(); }

    inline Fwk::Ptr<Shipment> shipment(Fwk::String _name) { return shipment_[_name]; }
    inline U32 shipments() const { return shipment_.members(); }
    void shipmentIs(Fwk::Ptr<Shipment> _ptr);
    Fwk::Ptr<Shipment> shipmentDel(Fwk::String _name);

    inline Segment::Ptr route(Fwk::String _name) { return routes_[_name]; }
    inline U32 routes() const { return routes_.size(); }
    void routeIs(Fwk::String _name, Segment::Ptr _ptr);
    Segment::Ptr routeDel(Fwk::String _name);

    class Notifiee : public virtual Fwk::NamedInterface::Notifiee
    {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;

      Location::Ptr notifier() const { return notifier_; }
      virtual void notifierIs(Location::Ptr& _notifier);

      virtual void onShipmentNew(Fwk::Ptr<Shipment> _ptr) {};

      ~Notifiee();
    protected:
      Notifiee() : Fwk::NamedInterface::Notifiee() {}
      Location::Ptr notifier_;
    };

  protected:
    friend class Segment;
    SegmentList segment_;
    ShipmentMap shipment_;
    RouteMap routes_;
    Location(Fwk::String _name, Entity::EntityType _type);
    Location( const Location& );
    Location::Notifiee::Ptr notifiee_;
  };
  
  /************************** CUSTOMER LOCATION **************************/
  class ShipmentsPerDay : public Ordinal<ShipmentsPerDay, int> {
  public:
  	ShipmentsPerDay(int num) : Ordinal<ShipmentsPerDay, int>(num) {
      if(num < 0) {
				cerr << "Invalid value passed to ShipmentsPerDay constructor: " << num << endl;
        throw Fwk::RangeException("Invalid value passed to ShipmentsPerDay constructor.");
			}
      value_ = num;
    }
  };

  class PackagesPerShipment : public Ordinal<PackagesPerShipment, int> {
	public:
  	PackagesPerShipment(int num) : Ordinal<PackagesPerShipment, int>(num) {
		if(num < 0) {
			cerr << "Invalid value passed to PackagesPerShipment constructor: " << num << endl;
		  throw Fwk::RangeException("Invalid value passed to PackagesPerShipment constructor.");
		}
		value_ = num;
	  }
	};

  class CustomerLocation: public Location {

  public:
    typedef Fwk::Ptr<CustomerLocation const> PtrConst;
    typedef Fwk::Ptr<CustomerLocation> Ptr;

    // Attribute Accessors
    ShipmentsPerDay transferRate() const { return transferRate_; }
    PackagesPerShipment shipmentSize() const { return shipmentSize_; }
	CustomerLocation::Ptr destination() const { return destination_; }
	NumShipments shipmentsReceived() const { return shipmentsReceived_; }
    Time averageLatency() const { return averageLatency_; }
    Fleet::Cost totalCost() const { return totalCost_; }

    // Attribute Mutators
    void transferRateIs(ShipmentsPerDay _transferRate);
    void shipmentSizeIs(PackagesPerShipment _ShipmentSize);
	void destinationIs(CustomerLocation::Ptr _destination);

    class Notifiee : public virtual Fwk::NamedInterface::Notifiee
    {
    public:
      typedef Fwk::Ptr<Notifiee> Ptr;

      CustomerLocation::Ptr notifier() const { return notifier_; }
      virtual void notifierIs(CustomerLocation::Ptr& _notifier);

      virtual void onTransferRate(ShipmentsPerDay transferRate_) {};
      virtual void onShipmentSize(PackagesPerShipment shipmentSize_) {};
      virtual void onDestination(CustomerLocation::Ptr destination_) {}

      ~Notifiee();
    protected:
      Notifiee() : Fwk::NamedInterface::Notifiee() {}
      CustomerLocation::Ptr notifier_;
    };

    static CustomerLocation::Ptr CustomerLocationNew(Fwk::String _name) {
       Ptr m = new CustomerLocation(_name);
       return m;
    }
    
  protected:
    CustomerLocation( const CustomerLocation& );
    CustomerLocation(Fwk::String _name);
    ShipmentsPerDay transferRate_;
    PackagesPerShipment shipmentSize_;
    CustomerLocation::Ptr destination_;
    NumShipments shipmentsReceived_;
    Time averageLatency_;
    Fleet::Cost totalCost_;

    CustomerLocation::Notifiee::Ptr notifiee_;
  };
  
  /************************** PORT LOCATION **************************/
  
  class PortLocation : public Location {
    
  public:
    typedef Fwk::Ptr<PortLocation const> PtrConst;
    typedef Fwk::Ptr<PortLocation> Ptr;

    static PortLocation::Ptr PortLocationNew(Fwk::String _name) {
       Ptr m = new PortLocation(_name);
       return m;
    }
  
  protected:
    PortLocation( const PortLocation& );
    PortLocation(Fwk::String _name);
  };
  
  /************************** TERMINAL LOCATION **************************/
  
  class TerminalLocation : public Location {
    
  public:
    typedef Fwk::Ptr<TerminalLocation const> PtrConst;
    typedef Fwk::Ptr<TerminalLocation> Ptr;

  protected:
    TerminalLocation(Fwk::String _name, Entity::EntityType _entity);
    TerminalLocation( const TerminalLocation& );
  };
  
  /************************** TRUCK TERMINAL **************************/
  
  class TruckTerminal : public TerminalLocation {
    
  public:
    typedef Fwk::Ptr<TruckTerminal const> PtrConst;
    typedef Fwk::Ptr<TruckTerminal> Ptr;

    static TruckTerminal::Ptr TruckTerminalNew(Fwk::String _name) {
       Ptr m = new TruckTerminal(_name);
       return m;
    }
  
  protected:
    TruckTerminal( const TruckTerminal& );
    TruckTerminal(Fwk::String _name);
  };
  
  /************************** BOAT TERMINAL **************************/
  
  class BoatTerminal : public TerminalLocation {
    
  public:
    typedef Fwk::Ptr<BoatTerminal const> PtrConst;
    typedef Fwk::Ptr<BoatTerminal> Ptr; 

    static BoatTerminal::Ptr BoatTerminalNew(Fwk::String _name) {
       Ptr m = new BoatTerminal(_name);
       return m;
    }
  
  protected:
    BoatTerminal( const BoatTerminal& );
    BoatTerminal(Fwk::String _name);  
  };
  
  /************************** PLANE TERMINAL **************************/
  
  class PlaneTerminal : public TerminalLocation {
    
  public:
    typedef Fwk::Ptr<PlaneTerminal const> PtrConst;
    typedef Fwk::Ptr<PlaneTerminal> Ptr;

    static PlaneTerminal::Ptr PlaneTerminalNew(Fwk::String _name) {
       Ptr m = new PlaneTerminal(_name);
       return m;
    }
  
  protected:
    PlaneTerminal( const PlaneTerminal& );
    PlaneTerminal(Fwk::String _name);
  };

  /************************** SHIPMENT **************************/

  class Shipment : public Fwk::NamedInterface
  {
  public:
    typedef Fwk::Ptr<Shipment const> PtrConst;
    typedef Fwk::Ptr<Shipment> Ptr;

    class NumPackages : public Ordinal<NumPackages, int> {
  	public:
    	NumPackages(int num) : Ordinal<NumPackages, int>(num) {
  		if(num < 0) {
				cerr << "Invalid value passed to NumPackages constructor: " << num << endl;
  		  throw Fwk::RangeException("Invalid value passed to NumPackages constructor.");
			}
  		value_ = num;
  	  }
  	};

    // Attribute Accessors
    NumPackages numPackages() const { return numPackages_; }
    Location::Ptr source() const { return source_; }
    Location::Ptr destination() const { return destination_; }
    Time startTime() const { return startTime_; }

    static Shipment::Ptr ShipmentNew(NumPackages _numPackages, Location::Ptr _source, Location::Ptr _destination, Time _startTime) {
      std::stringstream s;
      s << _numPackages.value() << _source->name() << _destination->name() << _startTime.value();
      Ptr m = new Shipment(_numPackages, _source, _destination, _startTime, s.str());
      return m;
    }

    Shipment const * fwkHmNext() const { return fwkHmNext_.ptr(); }
    Shipment * fwkHmNext() { return fwkHmNext_.ptr(); }
    void fwkHmNextIs(Shipment * _fwkHmNext) const {
      fwkHmNext_ = _fwkHmNext;
    }
    Fwk::String fwkKey() const { return name(); }

  protected:
    Shipment( const Shipment& );
    Shipment(NumPackages _numPackages, Location::Ptr _source, Location::Ptr _destination, Time _startTime, Fwk::String _name);

    mutable Shipment::Ptr fwkHmNext_;

    NumPackages numPackages_;
    Location::Ptr source_;
    Location::Ptr destination_;
    Time startTime_;
  };

  /************************** CONNECTIVITY **************************/

  class Connectivity : public Fwk::PtrInterface<Fleet>
  {
  public:
	typedef Fwk::Ptr<Connectivity const> PtrConst;
	typedef Fwk::Ptr<Connectivity> Ptr;

	struct Connection {
	  Connection();
	  vector<Segment::Ptr> segments_;
	  Fleet::Cost cost_;
	  Time time_;
	  Segment::Length distance_;
	  Segment::Expedited expedited_;
	};

    enum Algorithm {
       dfs_ = 0,
       ucs_ = 1
    };


    Algorithm algorithm() const { return algorithm_; }
    void algorithmIs(Algorithm _alg);

    static inline Algorithm dfs() { return dfs_; }
    static inline Algorithm ucs() { return ucs_; }

	Connectivity::Connection connect(Location::Ptr start_, Location::Ptr end_);
	vector<Connectivity::Connection> connectAll(Location::Ptr start_, Location::Ptr end_);
	vector<Connectivity::Connection> exploreAll(Location::Ptr start_, Segment::Length distance_, Fleet::Cost cost_, Time time_, Segment::Expedited expedited_);

    static Connectivity::Ptr ConnectivityNew(Fleet::Ptr _fleet) {
      Ptr m = new Connectivity(_fleet);
      return m;
    }

  protected:
    Fleet::Ptr fleet_;
    Algorithm algorithm_;
    bool simpleDFS(Location::Ptr currentLocation, Location::Ptr goal, set<Fwk::String> & visited, Connectivity::Connection &path);
    bool simpleUCS(Location::Ptr start, Location::Ptr goal, Connectivity::Connection &path);
    void DFSall(Segment::Ptr prevSegment, Location::Ptr currentLocation,
  		  Location::Ptr goal, set<Fwk::String> visited,
  		  Connectivity::Connection path,
  		  vector<Connectivity::Connection> & results,
  		  Segment::Expedited expedited);
    void DFSallWithLimit(Segment::Ptr prevSegment, Location::Ptr currentLocation,
  		  set<Fwk::String> visited,
  		  Connectivity::Connection path,
  		  vector<Connectivity::Connection> & results,
  		  Segment::Expedited expedited,
  		  Fleet::Cost maxCost, Time maxTime, Segment::Length maxDist);
    Connectivity( const Connectivity& );
    Connectivity(Fleet::Ptr _fleet);
  };
} /* end namespace */

#endif
