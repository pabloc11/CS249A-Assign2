user: pabloc11
user: nch25

*************** MIDDLE LAYER *************** 

The middle layer is divided into several classes, all of which inherit from the Instance class:

--StatsRep class - this class represents the stats entity to the client.  It overrides the attribute() and attributeIs() methods of the Instance class and performs error checking relevant to the Stats entity.  The attributeIs() method is empty since writing to the stats entity is not supported.

--FleetRep class - this class represents the fleet entity to the client.  It overrides the attribute() and attributeIs() methods of the Instance class and performs error checking relevant to the Fleet entity.

--ConnRep class - this class represents a connectivity entity which doesn't exist in the engine layer.  It overrides the attribute() and attributeIs() methods of the Instance class and performs error checking relevant to this entity as outlined in the assignment description.  The class also includes several private methods to aid in the graph traversals required by the attribute() method.  The attributeIs() method is empty since writing to this class is not supported.

--LocationRep class - this class represents the location entity to the client.  It overrides the attribute() and attributeIs() methods of the Instance class and performs error checking relevant to the location entity.  The attributeIs() method is empty since writing to the location entity is not supported.

--SegmentRep class - this class represents the segment entity to the client.  It overrides the attribute() and attributeIs() methods of the Instance class and performs error checking relevant to the segment entity.

The following classes inherit from LocationRep: CustomerLocationRep, PortLocationRep, TruckTerminalRep, BoatTerminalRep and PlaneTerminalRep, all of which represent respective entities in the engine layer.  Similarly, the following classes inherit from SegmentRep: TruckSegmentRep, BoatSegmentRep and PlaneSegmentRep.

Finally the ManagerImpl class inherits from Instance::Manager, and provides functionality for creating new instance objects (instanceNew) and deleting instance objects (instanceDel).

*************** ENGINE LAYER *************** 

The engine layer is divided into several classes enumerated below:

--Network class - this class is a collection of shipping entities maintained in a hash map. Entities are added/removed from the Network by calling entityIs()/entityDel(). The Network also provides a notifier interface for a single notifiee, in this case intended for the Stats object to track the number of each kind of entity in the network.

--Stats class - this class keeps a number of counters about information pertaining to the entities in a network. It registers with a Network object to be notified when entities are added/removed from the network. In addition when a Segment is added to the Network the Stats object will also register to be notified of changes to the expedited state of the segment so that it can accurately maintain the percentage of expedited segments in the network.

--Fleet class - this class provides simple accessors/mutators for a number of attributes pertaining to speed/cost/capacity of the different modes of transportation in a network.

--Entity class - this class is the root of the entity hierarchy and defines a single attribute entityType which is an enumeration of all the possible entities. This is easily expandable if more entities are created.

--Segment class - this class has a number of attributes pertaining to segments, source, returnSegment, length, difficulty, and expeditedState. It also provides a notifier interface for a single notifiee, in this case intended to notify the Stats object when the expeditedState changes. 

--Location class - this class maintains a linked list of segments that originate from the Location. A linked list is used so that arbitrary elements can be easily removed. This list can be read by index, but cannot be written directly. Instead the Segment class is made a friend class of the Location class so that setting the source attribute on a Segment can also modify the segment list in a Location object. This ensures that it is impossible to create inconsistency between the two.

--the remaining classes fill out the rest of the entity hierarchy but do not have any relevant attributes.
