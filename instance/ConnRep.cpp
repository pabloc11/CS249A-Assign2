#include "InstanceImpl.h"

namespace Shipping {

ConnRep::ConnRep(const string& name, ManagerImpl* manager) :
  Instance(name),
  manager_(manager),
  connectivity_(Connectivity::ConnectivityNew(manager_->fleetRep()->fleet()))
{}

string ConnRep::tokenizeExplore(vector<Connectivity::Connection> & results) {
	string s = "";
	for(vector<Connectivity::Connection>::iterator i = results.begin(); i != results.end(); ++i) {
		for(vector<Segment::Ptr>::iterator j = i->segments_.begin(); j != i->segments_.end(); ++j) {
			if(j == i->segments_.begin())
				s += (*j)->source()->name();
			s += "(" + (*j)->name() + ":" + Util::FloatToString((*j)->length().value()) + ":" +
					(*j)->returnSegment()->name() + ") " + (*j)->returnSegment()->source()->name();
		}
		s += "\n";
	}
	return s;
}

string ConnRep::tokenizeConnect(vector<Connectivity::Connection> & results) {
	string s = "";
	for(vector<Connectivity::Connection>::iterator i = results.begin(); i != results.end(); ++i) {
		s += Util::FloatToString(i->cost_.value()) + " " + Util::FloatToString(i->time_.value()) + " " +
				(i->expedited_ == Segment::expedited() ? "yes" : "no") + "; ";
		for(vector<Segment::Ptr>::iterator j = i->segments_.begin(); j != i->segments_.end(); ++j) {
			if(j == i->segments_.begin())
				s += (*j)->source()->name();
			s += "(" + (*j)->name() + ":" + Util::FloatToString((*j)->length().value()) +
					":" + (*j)->returnSegment()->name() + ") " + (*j)->returnSegment()->source()->name();
		}
		s += "\n";
	}
	return s;
}

string ConnRep::attribute(const string& name) {
	
	// Tokenize string
	istringstream iss(name);
	vector<string> tokens;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
	vector<string>::iterator token;
	token = tokens.begin();
	if (token == tokens.end()) return "";
	
	if (*token == "explore") {
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc) return "";
		advance(token, 1);
		if (token != tokens.end() && *token == ":")
			advance(token, 1);

		SearchParams params;
		params.cost = 0;
		params.dist = 0;
		params.time = 0;
		params.expedited = false;
		while (token != tokens.end()) {
			string attr = *token;
			advance(token, 1);
			if (attr == "expedited") {
				params.expedited = true;
				continue;
			}
			if (token == tokens.end()) return "";
			string val = *token;
			advance(token, 1);
			if (attr == "distance") params.dist = atof(val.c_str());
			else if (attr == "cost") params.cost = atof(val.c_str());
			else if (attr == "time") params.time = atof(val.c_str());
		}
		vector<Connectivity::Connection> results = connectivity_->exploreAll(loc->location(),
				Segment::Length(params.dist), Fleet::Cost(params.cost),
				Time(params.time), params.expedited ? Segment::expedited() : Segment::notExpedited());
		return tokenizeExplore(results);
	}
	else if (*token == "connect") {
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc0 = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc0) return "";
		advance(token, 1);
		if (token == tokens.end() || *token != ":") return "";
		advance(token, 1);
		if (token == tokens.end()) return "";
		Ptr<LocationRep> loc1 = dynamic_cast<LocationRep *>(manager_->instance(*token).ptr());
		if (!loc1) return "";
		vector<Connectivity::Connection> results = connectivity_->connectAll(loc0->location(), loc1->location());
		return tokenizeConnect(results);
	}
	else if (*token == "routing algorithm")
	{
		Connectivity::Algorithm alg = connectivity_->algorithm();
		if(alg == Connectivity::dfs())
			return "dfs";
		else if(alg == Connectivity::ucs())
			return "ucs";
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	
	return "";
}

void ConnRep::attributeIs(const string& name, const string& v)
{
	if (name == "routing algorithm")
	{
		if(v == "dfs")
		{
			connectivity_->algorithmIs(Connectivity::dfs());
			return;
		}
		else if(v == "ucs")
		{
			connectivity_->algorithmIs(Connectivity::ucs());
			return;
		}
		else
		{
			cerr << "Unsupported routing algorithm: " << v << endl;
			throw Fwk::UnknownArgException("Unsupported routing algorithm");
		}
	}
	else
	{
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
}

}
