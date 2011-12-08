#include "InstanceImpl.h"

namespace Shipping {

string ClockRep::attribute(const string& name) {

	if (name == "type"){
		Activity::Manager::Type type = activityManager_->type();
		if(type == Activity::Manager::real())
			return "real";
		else if(type == Activity::Manager::virt())
			return "virtual";
	}
	else if (name == "now") {
		return Util::FloatToString(activityManager_->now().value());
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	return "";
}

void ClockRep::attributeIs(const string& name, const string& v) {

	if (name == "type"){
		if(v == "real")
			activityManager_->typeIs(Activity::Manager::real());
		else if (v == "virtual")
			activityManager_->typeIs(Activity::Manager::virt());
		else {
			cerr << "Unsupported clock type argument: " << v << endl;
			throw Fwk::UnknownArgException("Unsupported clock type argument");
		}
	}
	else if (name == "now") {
		vector<Location::Ptr> allLocations;
		map<string, Ptr<Instance> > entities = manager_->instance_;

		for(map<string,Ptr<Instance> >::iterator i = entities.begin(); i != entities.end(); ++i){
			Ptr<Instance> instance = i->second;
			Ptr<LocationRep> ptr = dynamic_cast<LocationRep *>(instance.ptr());
			if(ptr)
				allLocations.push_back(ptr->location());
		}

		Connectivity::Ptr con = manager_->connRep()->connectivity();
		for(vector<Location::Ptr>::iterator i = allLocations.begin(); i != allLocations.end(); ++i) {
			for(vector<Location::Ptr>::iterator j = allLocations.begin(); j != allLocations.end(); ++j) {
				CustomerLocation * iC = dynamic_cast<CustomerLocation *>(i->ptr());
				CustomerLocation * jC = dynamic_cast<CustomerLocation *>(j->ptr());
				if(i != j && (iC || jC)) {
					(*i)->routeIs((*j)->name(), con->connect(*i, *j).segments_[0]);
				}
			}
		}

		activityManager_->nowIs(Time(atof(v.c_str())));
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}
