#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
	
FleetRepReactor::FleetRepReactor() :
	truckSpeed_(1),
	boatSpeed_(1),
	planeSpeed_(1),
	specialTruckSpeed_(1),
	specialBoatSpeed_(1),
	specialPlaneSpeed_(1),
	truckCost_(1),
	boatCost_(1),
	planeCost_(1),
	specialTruckCost_(1),
	specialBoatCost_(1),
	specialPlaneCost_(1),
	truckCapacity_(1),
	boatCapacity_(1),
	planeCapacity_(1),
	specialTruckCapacity_(1),
	specialBoatCapacity_(1),
	specialPlaneCapacity_(1),
	tSpeed(false),
	bSpeed(false),
	pSpeed(false),
	tCost(false),
	bCost(false),
	pCost(false),
	tCapacity(false),
	bCapacity(false),
	pCapacity(false),
	start(false),
	end(false),
	startTime(0),
	endTime(0)
{}

void FleetRepReactor::onAttributeIs() {}

}
