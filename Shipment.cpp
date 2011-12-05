#include "Engine.h"

namespace Shipping {
  Shipment::Shipment(NumPackages _numPackages, Location::Ptr _source, Location::Ptr _destination) :
  numPackages_(_numPackages),
  source_(_source),
  destination_(_destination)
  {}
}
