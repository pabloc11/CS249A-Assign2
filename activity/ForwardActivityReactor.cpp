#include "Activity.h"
#include "Reactors.h"

ForwardActivityReactor::ForwardActivityReactor(Activity* _a) : Activity::Notifiee(_a) {}

void ForwardActivityReactor::onNextTime() {}

void ForwardActivityReactor::onStatus() {}