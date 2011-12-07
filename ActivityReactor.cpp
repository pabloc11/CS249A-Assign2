#include <iostream>
#include "ActivityReactor.h"

void ActivityReactor::onStatus() {

  switch (activity_->status()) {
	
	  case Activity::executing:
			cout << "Executing " << activity_->name() << endl;
			activity_->statusIs(Activity::done);
			break;	
			
  	case Activity::queued:
			cout << "Queueing " << activity_->name() << endl;
			activity_->nextTimeIs(Time(activity_->nextTime().value()));
			activity_->statusIs(Activity::nextTimeScheduled);
			break;
			
	  case Activity::nextTimeScheduled:
			cout << "Scheduling " << activity_->name() << endl;
			manager_->lastActivityIs(activity_);
			break;	
			
	  default:
			break;
  }
}
