#include "Activity.h"
#include "Reactors.h"

ScheduleChangesReactor::ScheduleChangesReactor(Activity* _a, Fleet::Ptr _f, ScheduledFleetAttrs& _s) : 
	Activity::Notifiee(_a),
	activity_(_a),
	fleet_(_f),
	scheduledAttrs_(_s) {}

void ScheduleChangesReactor::onStatus() {

	if (activity_->status() == Activity::queued) {
		//cout << "  Queueing " << activity_->name() << endl;
		activityManagerInstance()->lastActivityIs(activity_);
	}
		
  else if (activity_->status() == Activity::executing) {
	
		//cout << "  Executing scheduling activity" << endl;
	
		if (scheduledAttrs_.tSpeed) {
			fleet_->truckSpeedIs(scheduledAttrs_.sTruckSpeed);
		}
		if (scheduledAttrs_.bSpeed) {
			fleet_->boatSpeedIs(scheduledAttrs_.sBoatSpeed);
		}
		if (scheduledAttrs_.pSpeed) {
			fleet_->planeSpeedIs(scheduledAttrs_.sPlaneSpeed);
		}
		if (scheduledAttrs_.tCost) {
			fleet_->truckCostIs(scheduledAttrs_.sTruckCost);
		}
		if (scheduledAttrs_.bCost) {
			fleet_->boatCostIs(scheduledAttrs_.sBoatCost);
		}
		if (scheduledAttrs_.pCost) {
			fleet_->planeCostIs(scheduledAttrs_.sPlaneCost);
		}
		if (scheduledAttrs_.tCapacity) {
			fleet_->truckCapacityIs(scheduledAttrs_.sTruckCapacity);
		}
		if (scheduledAttrs_.bCapacity) {
			fleet_->boatCapacityIs(scheduledAttrs_.sBoatCapacity);
		}
		if (scheduledAttrs_.pCapacity) {
			fleet_->planeCapacityIs(scheduledAttrs_.sPlaneCapacity);
		}
	
		//activity_->nextTimeIs(activity_->nextTime().value() + 24.0);
		activity_->nextTimeIs(scheduledAttrs_.startTime.value() + FleetRepReactor::currentTimeRoundedDownTo24() + 24.0);
		activity_->statusIs(Activity::queued);		
	}
}

