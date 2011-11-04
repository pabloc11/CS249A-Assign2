#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "Ptr.h"
#include "PtrInterface.h"
#include "Instance.h"
#include "Nominal.h"
#include "fwk/NamedInterface.h"
#include "fwk/HashMap.h"

namespace Shipping {

/************************** ENTITY **************************/

  class Entity : Fwk::NamedInterface
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
    
    static Entity::Ptr EntityNew(Fwk::String _name, EntityType _type) {
      Ptr m = new Entity(_name, _type);
      return m;   
    }
    
    class NotifieeConst : public virtual Fwk::NamedInterface::NotifieeConst
    {
    public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      
      Entity::PtrConst notifier() const { return notifier_; }
      virtual void notifierIs(const Entity::PtrConst& _notifier);
      
      ~NotifieeConst();
    protected:
      NotifieeConst() : Fwk::NamedInterface::NotifieeConst() {}
      Entity::PtrConst notifier_; 
    };
    Entity::NotifieeConst::PtrConst notifiee() const { return notifiee_; }
  protected:
    EntityType entityType_;
    Entity(Fwk::String, EntityType);
  
    void notifieeIs(Entity::NotifieeConst::PtrConst n) const 
    {
      Entity* me = const_cast<Entity*>(this);
      me->notifiee_ = n;
    }
    Entity::NotifieeConst::PtrConst notifiee_;
  };

/************************** STATS **************************/

  class Stats : Fwk::NamedInterface
  {
  public:
    typedef Fwk::Ptr<Stats const> PtrConst;
    typedef Fwk::Ptr<Stats> Ptr;
  
    U32 truckSegmentCount() { return truckSegmentCount_; }
    U32 boatSegmentCount() { return boatSegmentCount_; }
    U32 planeSegmentCount() { return planeSegmentCount_; }
    float expeditePercentage() { return expeditePercentage_; }
  
  private:
    U32 truckSegmentCount_;
    U32 boatSegmentCount_;
    U32 planeSegmentCount_;
    float expeditePercentage_;
  };

/************************** FLEET **************************/

  class Fleet : Fwk::NamedInterface
  {
  public:
    typedef Fwk::Ptr<Fleet const> PtrConst;
    typedef Fwk::Ptr<Fleet> Ptr;
    
    class Speed : public Ordinal<Speed, float> {
      Speed(float num) : Ordinal<Speed, float>(num) {
        value_ = num;
      }    
    };
    class Cost : public Ordinal<Cost, float> {
      Cost(float num) : Ordinal<Cost, float>(num) {
        value_ = num;
      }    
    };
    class Capacity : public Ordinal<Capacity, int> {
      Capacity(int num) : Ordinal<Capacity, int>(num) {
        value_ = num;
      }    
    };

    Fleet::Speed truckSpeed() { return truckSpeed_; }
    Fleet::Speed boatSpeed() { return boatSpeed_; }
    Fleet::Speed planeSpeed() { return planeSpeed_; }
    Fleet::Cost truckCost() { return truckCost_; }
    Fleet::Cost boatCost() { return boatCost_; }
    Fleet::Cost planeCost() { return planeCost_; }
    Fleet::Capacity truckCapacity() { return truckCapacity_; }
    Fleet::Capacity boatCapacity() { return boatCapacity_; }
    Fleet::Capacity planeCapacity() { return planeCapacity_; }
  
  private:  
    Speed truckSpeed_, boatSpeed_, planeSpeed_;
    Cost truckCost_, boatCost_, planeCost_;
    Capacity truckCapacity_, boatCapacity_, planeCapacity_;
  };

/************************** NETWORK **************************/

  class Network : Fwk::NamedInterface
	{
  public:
    typedef Fwk::Ptr<Network const> PtrConst;
    typedef Fwk::Ptr<Network> Ptr;
    typedef Fwk::HashMap< Entity, Fwk::String, Entity, Entity::PtrConst, Entity::PtrConst > EntityMap; 
  
    // Attribute Accessors
    Entity::PtrConst entity(Fwk::String _name) const { return entity_[_name]; }
    Entity::Ptr entity(Fwk::String _name) { return entity_[_name]; }
    U32 entities() { return entity_.members(); }
		Stats::Ptr stats() { return stats_; };
		Fleet::Ptr fleet() { return fleet_; };
  
    // Attribute Mutators
    Entity::Ptr entityDel(Fwk::String _name);
    Entity::Ptr entityIs(Fwk::String _name);
  
    static Network::Ptr NetworkNew() {
      Ptr m = new Network();
      return m;   
  	}

  protected:  
    EntityMap entity_;
    Stats::Ptr stats_;
    Fleet::Ptr fleet_;
    Network();
    Network( const Network& );
  };

  class Location;

/************************** SEGMENT **************************/

  class Segment : Entity 
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    class Difficulty : public Ordinal<Difficulty, float> 
    {
    public:
      Difficulty(float num) : Ordinal<Difficulty, float>(num)
      {
        if(num < 1.0f || num > 5.0f)
        {
          //TODO: throw an exception
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
    Fwk::Ptr<Location const> source() const { return source_; }
    Segment::PtrConst returnSegment() const { return returnSegment_; }
    Difficulty difficulty() const { return difficulty_; }
    Expedited expeditedState() const { return expeditedState_; }
    
    // Attribute Mutators
    void sourceIs(Fwk::Ptr<Location const> _source);
    void returnSegmentIs(Segment::PtrConst _returnSegment);
    void difficultyIs(Difficulty _difficulty);
    void expeditedIs(Expedited _expedited);
 
    // public constructor
    static Segment::Ptr SegmentIs(Fwk::String _name) {
      Segment::Ptr m = new Segment(_name);
      return m;
    }
    
    // Notifier Class
    class NotifieeConst : public virtual Entity::NotifieeConst
    {
    public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      
      Segment::PtrConst notifier() const { return dynamic_cast<Segment const *>(Entity::NotifieeConst::notifier().ptr()); }
      
      virtual void onSource() {}
      virtual void onReturnSegment() {}
      virtual void onDifficulty() {}
      virtual void onExpedited() {}
    protected:
      NotifieeConst() : Entity::NotifieeConst() {}
	  };
 
  protected:
    // Constructors
    Segment( const Segment& );
    Segment(Fwk::String _name);

    // Attributes
    Fwk::Ptr<Location const> source_;
    Segment::PtrConst returnSegment_;
    Difficulty difficulty_;
    Expedited expeditedState_;
  };

/************************** TRUCK SEGMENT **************************/

  class TruckSegment : Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static TruckSegment::Ptr TruckSegmentIs(Fwk::String _name) {
      TruckSegment::Ptr m = new TruckSegment(_name);
      return m;
    }
    
    // Notifier Interfaces
    class NotifieeConst : public virtual Segment::NotifieeConst
    {
    public:
      typedef Fwk::Ptr<NotifieeConst const> PtrConst;
      
      Segment::PtrConst notifier() const { return dynamic_cast<TruckSegment const *>(Segment::NotifieeConst::notifier().ptr()); }
      
    protected:
      NotifieeConst() : Segment::NotifieeConst() {}
		};
 
  protected:
    // Constructors
    TruckSegment( const TruckSegment& );
    TruckSegment(Fwk::String _name);
  };

/************************** BOAT SEGMENT **************************/

  class BoatSegment : Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static BoatSegment::Ptr BoatSegmentIs(Fwk::String _name) {
      BoatSegment::Ptr m = new BoatSegment(_name);
      return m;
    }
    
    // Notifier Interfaces
    class NotifieeConst : public virtual Segment::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      Segment::PtrConst notifier() const { return dynamic_cast<BoatSegment const *>(Segment::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : Segment::NotifieeConst() {}
		};
 
  protected:
    // Constructors
    BoatSegment( const BoatSegment& );
    BoatSegment(Fwk::String _name);
  };

/************************** PLANE SEGMENT **************************/

  class PlaneSegment : Segment
  {
  public:
    typedef Fwk::Ptr<Segment const> PtrConst;
    typedef Fwk::Ptr<Segment> Ptr;

    // public constructor
    static PlaneSegment::Ptr PlaneSegmentIs(Fwk::String _name) {
      PlaneSegment::Ptr m = new PlaneSegment(_name);
      return m;
    }
    
    // Notifier Interfaces
    class NotifieeConst : public virtual Segment::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      Segment::PtrConst notifier() const { return dynamic_cast<PlaneSegment const *>(Segment::NotifieeConst::notifier().ptr()); }
      
    protected:
      NotifieeConst() : Segment::NotifieeConst() {}
		};
 
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
    typedef Fwk::HashMap< Segment, Fwk::String, Segment, Segment::PtrConst, Segment::Ptr > SegmentMap;
    
    // Attribute Accessors
    Segment::PtrConst segment(Fwk::String _str) const { return segment_[_str]; }
    Segment::Ptr segment(Fwk::String _str) { return segment_[_str]; }
    U32 segments() { return segment_.members(); }
    
    // Attribute Mutators
    Segment::Ptr segmentDel(Fwk::String _str);
    Segment::Ptr segmentIs(Fwk::String _str);

    // Notifier Class
    class NotifieeConst : public virtual Entity::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      Location::PtrConst notifier() const { return dynamic_cast<Location const *>(Entity::NotifieeConst::notifier().ptr()); }
      
      virtual void onSegmentNew() {}
      virtual void onSegmentDel() {}
    protected:
      NotifieeConst() : Entity::NotifieeConst() {}
		};
    
  protected:  
    SegmentMap segment_;
    Location( const Location& );
  };
  
  /************************** CUSTOMER LOCATION **************************/
  
  class CustomerLocation: public Location {
    
  public:
    typedef Fwk::Ptr<CustomerLocation const> PtrConst;
    typedef Fwk::Ptr<CustomerLocation> Ptr; 

    // Notifier Class
    class NotifieeConst : public virtual Location::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      CustomerLocation::PtrConst notifier() const { return dynamic_cast<CustomerLocation const *>(Location::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : Location::NotifieeConst() {}
		};
    
    static CustomerLocation::Ptr CustomerLocationIs(Fwk::String _name, Entity::EntityType _type) {
       Ptr m = new CustomerLocation(_name, _type);
       return m;
    }
    
  protected:
    CustomerLocation( const CustomerLocation& );
    CustomerLocation(Fwk::String _name, Entity::EntityType _type);
  };
  
  /************************** PORT LOCATION **************************/
  
  class PortLocation : public Location {
    
  public:
    typedef Fwk::Ptr<PortLocation const> PtrConst;
    typedef Fwk::Ptr<PortLocation> Ptr;

    // Notifier Class
    class NotifieeConst : public virtual Location::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      PortLocation::PtrConst notifier() const { return dynamic_cast<PortLocation const *>(Location::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : Location::NotifieeConst() {}
		};
    
    static PortLocation::Ptr PortLocationIs(Fwk::String _name, Entity::EntityType _type) {
       Ptr m = new PortLocation(_name, _type);
       return m;
    }
  
  protected:
    PortLocation( const PortLocation& );
    PortLocation(Fwk::String _name, Entity::EntityType _type);
  };
  
  /************************** TERMINAL LOCATION **************************/
  
  class TerminalLocation : public Location {
    
  public:
    typedef Fwk::Ptr<TerminalLocation const> PtrConst;
    typedef Fwk::Ptr<TerminalLocation> Ptr;

    // Notifier Class
    class NotifieeConst : public virtual Location::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      TerminalLocation::PtrConst notifier() const { return dynamic_cast<TerminalLocation const *>(Location::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : Location::NotifieeConst() {}
		};
  
  protected:
    TerminalLocation( const TerminalLocation& );
  };
  
  /************************** TRUCK TERMINAL **************************/
  
  class TruckTerminal : TerminalLocation {
    
  public:
    typedef Fwk::Ptr<TruckTerminal const> PtrConst;
    typedef Fwk::Ptr<TruckTerminal> Ptr;

    // Notifier Class
    class NotifieeConst : public virtual TerminalLocation::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      TruckTerminal::PtrConst notifier() const { return dynamic_cast<TruckTerminal const *>(TerminalLocation::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : TerminalLocation::NotifieeConst() {}
		};
    
    static TruckTerminal::Ptr TruckTerminalIs(Fwk::String _name, Entity::EntityType _type) {
       Ptr m = new TruckTerminal(_name, _type);
       return m;
    }
  
  protected:
    TruckTerminal( const TruckTerminal& );
    TruckTerminal(Fwk::String _name, Entity::EntityType _type);
  };
  
  /************************** BOAT TERMINAL **************************/
  
  class BoatTerminal : TerminalLocation {
    
  public:
    typedef Fwk::Ptr<BoatTerminal const> PtrConst;
    typedef Fwk::Ptr<BoatTerminal> Ptr; 

    // Notifier Class
    class NotifieeConst : public virtual TerminalLocation::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      BoatTerminal::PtrConst notifier() const { return dynamic_cast<BoatTerminal const *>(TerminalLocation::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : TerminalLocation::NotifieeConst() {}
		};
    
    static BoatTerminal::Ptr BoatTerminalIs(Fwk::String _name, Entity::EntityType _type) {
       Ptr m = new BoatTerminal(_name, _type);
       return m;
    }
  
  protected:
    BoatTerminal( const BoatTerminal& );
    BoatTerminal(Fwk::String _name, Entity::EntityType _type);  
  };
  
  /************************** PLANE TERMINAL **************************/
  
  class PlaneTerminal : TerminalLocation {
    
  public:
    typedef Fwk::Ptr<PlaneTerminal const> PtrConst;
    typedef Fwk::Ptr<PlaneTerminal> Ptr;

    // Notifier Class
    class NotifieeConst : public virtual TerminalLocation::NotifieeConst
    {
    public:
      typedef Fwk::PtrInterface<NotifieeConst const> PtrConst;
      
      PlaneTerminal::PtrConst notifier() const { return dynamic_cast<PlaneTerminal const *>(TerminalLocation::NotifieeConst::notifier().ptr()); }

    protected:
      NotifieeConst() : TerminalLocation::NotifieeConst() {}
		};
    
    static PlaneTerminal::Ptr PlaneTerminalIs(Fwk::String _name, Entity::EntityType _type) {
       Ptr m = new PlaneTerminal(_name, _type);
       return m;
    }
  
  protected:
    PlaneTerminal( const PlaneTerminal& );
    PlaneTerminal(Fwk::String _name, Entity::EntityType _type);
  };

} /* end namespace */

#endif
