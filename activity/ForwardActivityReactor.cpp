#include "Activity.h"
#include "Reactors.h"

ForwardActivityReactor::ForwardActivityReactor(Activity* _a) : Activity::Notifiee(_a) {}

void ForwardActivityReactor::onNextTime() {}

void ForwardActivityReactor::onStatus() {
	//TODO: update the shipments timeTaken_ and costTaken_ when done forwarding them
}
