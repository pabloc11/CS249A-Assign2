#include "InstanceImpl.h"

namespace Shipping {

ConnRep::ConnRep(const string& name, ManagerImpl* manager) : Instance(name), manager_(manager) {	
	params_.dist = 0;
	params_.cost = 0;
	params_.time = 0;
	params_.expedited = false;
	params_.destination = "";
}

bool ConnRep::paramsExceeded(SearchProgress& progress) {
	
	if ((params_.dist != 0 && progress.dist > params_.dist) || (params_.cost != 0 && progress.cost > params_.cost) ||
	  	(params_.time != 0 && progress.time > params_.time))
		return true;
	
	return false;
}

void ConnRep::appendToPath(Segment::PtrConst seg, Location::PtrConst loc, string& path) {
	
	if (path == "") path += seg->source()->name();
	path += "(" + seg->name() + ":" + Util::FloatToString(seg->length().value()) + ":" +
		seg->returnSegment()->name() + ") " + loc->name();
}

ConnRep::SearchProgress ConnRep::updateProgress(Segment::PtrConst seg, SearchProgress& progress, bool expedited) {
	
	float distance = seg->length().value();
	float difficulty = seg->difficulty().value();
	
	float cost, speed;
	if (seg->entityType() == Entity::truckSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->truckCost().value();
		speed = manager_->fleetRep()->fleet()->truckSpeed().value();
	}
	else if (seg->entityType() == Entity::boatSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->boatCost().value();
		speed = manager_->fleetRep()->fleet()->boatSpeed().value();
	}
	else if (seg->entityType() == Entity::planeSegment()) {
		cost = distance * difficulty * manager_->fleetRep()->fleet()->planeCost().value();
		speed = manager_->fleetRep()->fleet()->planeSpeed().value();
	}
	
	// Update for expedited shipping
	if (expedited) {
		cost *= 1.5;
		speed *= 1.3;
	}
	
	float time = distance / speed;
	
	// Update progress
	SearchProgress newProgress = { progress.dist + distance, progress.cost + cost, progress.time + time };
	return newProgress;
}

void ConnRep::exploreDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
				  		 SearchProgress progress, string& output, string path) {

	// Base case - location has already been visited or search parameters are exceeded
	if (visited.count(loc->name()) || paramsExceeded(progress))
		return;
	
	// Add path to output string
	if (seg) {
		appendToPath(seg, loc, path);
		output += path + "\n";
	}
	
	visited.insert(loc->name());
	
	// Search all outbound segments from current location
	for (Location::SegmentListIteratorConst iter = loc->segmentIterConst(); iter.ptr(); ++iter) {
		Segment::PtrConst segment = iter.ptr();
		
		// Skip segment if expedited shipping is specified but segment doesn't offer it
		if (params_.expedited && segment->expeditedState() != Segment::expedited())
			continue;
					
		if (segment->returnSegment()) {			
			SearchProgress newProgress = updateProgress(segment, progress, params_.expedited);
			exploreDFS(segment, segment->returnSegment()->source(), visited, newProgress, output, path);
		}
    }		
}

void ConnRep::connectDFS(Segment::PtrConst seg, Location::PtrConst loc, set<string> visited,
				  		 SearchProgress progress, bool expedited, string& output, string path) {

	// Base case - location has already been visited
	if (visited.count(loc->name()))
		return;
	
	if (seg) appendToPath(seg, loc, path);
	
	// Add path to output string if destination reached
	if (loc->name() == params_.destination) {
		string isExpedited;
		if (expedited) isExpedited = "yes";
		else isExpedited = "no";
		output += Util::FloatToString(progress.cost) + " " + Util::FloatToString(progress.time) + " " + isExpedited + "; " + path + "\n";
		return;
	}
	
	visited.insert(loc->name());
	
	// Search all outbound segments from current location
	for (Location::SegmentListIteratorConst iter = loc->segmentIterConst(); iter.ptr(); ++iter) {
		Segment::PtrConst segment = iter.ptr();	
			
		if (segment->returnSegment()) {			
			SearchProgress newProgress = updateProgress(segment, progress, expedited);
			connectDFS(segment, segment->returnSegment()->source(), visited, newProgress, expedited, output, path);
		}
    }		
}

string ConnRep::explore(Ptr<LocationRep> loc) {
	
	SearchProgress progress = {0, 0, 0};
	set<string> visited;
	string output = "";

	exploreDFS(NULL, loc->location(), visited, progress, output, "");
	
	size_t end = output.find_last_of("\n");
	if (end != string::npos) output = output.substr(0, end);
	return output;
}

string ConnRep::connect(Ptr<LocationRep> loc0, Ptr<LocationRep> loc1) {
	
	params_.destination = loc1->name();
	SearchProgress progress = {0, 0, 0};
	set<string> visited;
	string output = "";	
	connectDFS(NULL, loc0->location(), visited, progress, false, output, "");
	
	SearchProgress expeditedProgress = {0, 0, 0};
	set<string> expeditedVisited;
	connectDFS(NULL, loc0->location(), expeditedVisited, expeditedProgress, true, output, "");
	
	size_t end = output.find_last_of("\n");
	if (end != string::npos) output = output.substr(0, end);
	return output;
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
		if (token != tokens.end() && *token == ":") advance(token, 1);

		while (token != tokens.end()) {
			string attr = *token;
			advance(token, 1);
			if (attr == "expedited") {
				params_.expedited = true;
				continue;
			}
			if (token == tokens.end()) return "";
			string val = *token;
			advance(token, 1);
			if (attr == "distance") params_.dist = atof(val.c_str());
			else if (attr == "cost") params_.cost = atof(val.c_str());
			else if (attr == "time") params_.time = atof(val.c_str());
		}
		return explore(loc);
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
		return connect(loc0, loc1);
	}
	else {
		cerr << "Incompatible type-attribute pair: " << this->name() << ", " << name << endl;
		throw Fwk::UnknownArgException("Incompatible type-attribute pair.");
	}
	
	return "";
}

void ConnRep::attributeIs(const string& name, const string& v)
{
	if (name == "routing")
	{
		if(v == "bfs")
		{
			//TODO:
			return;
		}
		else if(v == "dijkstra")
		{
			//TODO:
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