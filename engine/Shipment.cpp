#include "Engine.h"

namespace Shipping {
  Shipment::Shipment(NumPackages _numPackages, CustomerLocation::Ptr _source, CustomerLocation::Ptr _destination, Time _startTime, Fwk::String _name) :
  NamedInterface(_name),
  numPackages_(_numPackages),
  source_(_source),
  destination_(_destination),
  startTime_(_startTime),
  costTaken_(0)
  {}
}
