#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Nominal.h"
#include "fwk/NamedInterface.h"
#include "fwk/HashMap.h"
#include "fwk/ListRaw.h"

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

    class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst
    {
    public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      
      Network::PtrConst notifier() const { return notifier_; }
      virtual void notifierIs(const Network::PtrConst& _notifier);
      
      virtual void onEntityNew(Entity::Ptr _ptr) {};
      virtual void onEntityDel(Entity::Ptr _ptr) {};
      
      ~NotifieeConst();
    protected:
      NotifieeConst() : Fwk::NamedInterface::NotifieeConst() {}
      Network::PtrConst notifier_; 
    };

  protected:  
    EntityMap entity_;
    Network(Fwk::String _name);
    Network( const Network& );
    
    Network::NotifieeConst::PtrConst notifiee_;
  };

/************************** STATS **************************/

  class Stats : public Fwk::PtrInterface<Stats>
  {
  public:
    typedef Fwk::Ptr<Stats const> PtrConst;
    typedef Fwk::Ptr<Stats> Ptr;

		class Percent : public Ordinal<Percent, float>
		{
    public:
	  	Percent(float num) : Ordinal<Percent, float>(num) {
				if (num < 0.0f || num > 1.0f)
          throw Fwk::RangeException("Invalid range passed to Percent constructor\n");
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
    
    static Stats::Ptr StatsNew(Network::Ptr _n) {
      Ptr m = new Stats(_n);
      return m;   
    }
  
  protected:
    Stats(const Stats&);
    Stats(Network::Ptr);
   
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
				if(num < 0.0f)
					throw Fwk::RangeException("Invalid range passed to Speed constructor\n");
        value_ = num;
      }    
    };
    class Cost : public Ordinal<Cost, float> {
    public:
      Cost(float num) : Ordinal<Cost, float>(num) {
				if(num < 0.0f)
					throw Fwk::RangeException("Invalid range passed to Cost constructor\n");
        value_ = num;
      }    
    };
    class Capacity : public Ordinal<Capacity, int> {
    public:
      Capacity(int num) : Ordinal<Capacity, int>(num) {
				if(num < 0)
					throw Fwk::RangeException("Invalid range passed to Capacity constructor\n");
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

  class Segment : public Entity 
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

		class Length : public Nominal<Length, float>
		{
		public:
			Length(float num) : Nominal<Length, float>(num)
			{
				if(num < 0.0f)
					throw Fwk::RangeException("Invalid range passed to Length constructor\n");
				value_ = num;
			}
		};

    class Difficulty : public Ordinal<Difficulty, float> 
    {
    public:
      Difficulty(float num) : Ordinal<Difficulty, float>(num)
      {
        if(num < 1.0f || num > 5.0f)
          throw Fwk::RangeException("Invalid range passed to Difficulty constructor\n");
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
    Fwk::Ptr<Location const> source() const { return source_; }
    Segment::PtrConst returnSegment() const { return returnSegment_; }
		Length length() const { return length_; }
    Difficulty difficulty() const { return difficulty_; }
    Expedited expeditedState() const { return expeditedState_; }
    
    // Attribute Mutators
    void sourceIs(Fwk::Ptr<Location> _source);
    void returnSegmentIs(Segment::Ptr _returnSegment);
		void lengthIs(Length _length);
    void difficultyIs(Difficulty _difficulty);
    void expeditedIs(Expedited _expedited);
    
    // Notifier Class
    class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst
    {
    public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      typedef Fwk::Ptr<NotifieeConst> Ptr;
      
      Segment::PtrConst notifier() const { return notifier_; }
      virtual void notifierIs(const Segment::PtrConst& _notifier);
      
      virtual void onExpedited(Segment::Expedited _expedited) {}
      
      ~NotifieeConst();
    protected:
      NotifieeConst() : Fwk::NamedInterface::NotifieeConst() {}
      Segment::PtrConst notifier_; 
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

    // Attributes
    Fwk::Ptr<Location> source_;
    Segment::Ptr returnSegment_;
		Length length_;
    Difficulty difficulty_;
    Expedited expeditedState_;
    
    Segment::NotifieeConst::Ptr notifiee_;
    Segment *lrNext_;
  };

/************************** TRUCK SEGMENT **************************/

  class TruckSegment : public Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static TruckSegment::Ptr TruckSegmentIs(Fwk::String _name) {
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
    static BoatSegment::Ptr BoatSegmentIs(Fwk::String _name) {
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
    static PlaneSegment::Ptr PlaneSegmentIs(Fwk::String _name) {
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
    
    // Attribute Accessors
    Segment::PtrConst segment(unsigned _index) const;
    Segment::Ptr segment(unsigned _index);
    inline U32 segments() { return segment_.members(); }
    inline SegmentListIteratorConst segmentIterConst() const { return segment_.iterator(); }
    
  protected:
    friend class Segment;
    SegmentList segment_;
    Location(Fwk::String _name, Entity::EntityType _type);
    Location( const Location& );
  };
  
  /************************** CUSTOMER LOCATION **************************/
  
  class CustomerLocation: public Location {
    
  public:
    typedef Fwk::Ptr<CustomerLocation const> PtrConst;
    typedef Fwk::Ptr<CustomerLocation> Ptr; 

    static CustomerLocation::Ptr CustomerLocationIs(Fwk::String _name) {
       Ptr m = new CustomerLocation(_name);
       return m;
    }
    
  protected:
    CustomerLocation( const CustomerLocation& );
    CustomerLocation(Fwk::String _name);
  };
  
  /************************** PORT LOCATION **************************/
  
  class PortLocation : public Location {
    
  public:
    typedef Fwk::Ptr<PortLocation const> PtrConst;
    typedef Fwk::Ptr<PortLocation> Ptr;

    static PortLocation::Ptr PortLocationIs(Fwk::String _name) {
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

    static TruckTerminal::Ptr TruckTerminalIs(Fwk::String _name) {
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

    static BoatTerminal::Ptr BoatTerminalIs(Fwk::String _name) {
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

    static PlaneTerminal::Ptr PlaneTerminalIs(Fwk::String _name) {
       Ptr m = new PlaneTerminal(_name);
       return m;
    }
  
  protected:
    PlaneTerminal( const PlaneTerminal& );
    PlaneTerminal(Fwk::String _name);
  };

} /* end namespace */

#endif
