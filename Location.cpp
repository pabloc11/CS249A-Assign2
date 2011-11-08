#include "Engine.h"

namespace Shipping {
  Location::Location(Fwk::String _name, Entity::EntityType _type) :
    Entity(_name, Entity::customerLocation()),
    segment_()
  {}
  
  Segment::PtrConst Location::segment(unsigned _index) const {
    unsigned i = 0;
    for( SegmentList::IteratorConst iter = segment_.iterator(); iter.ptr() && i <= _index; ++iter, i++) {
      if(i == _index)
        return iter.ptr();
    }
    return NULL;
  }
  Segment::Ptr Location::segment(unsigned _index) {
    unsigned i = 0;
    for( SegmentList::Iterator iter = segment_.iterator(); iter.ptr() && i <= _index; ++iter, i++) {
      if(i == _index)
        return iter.ptr();
    }
    return NULL;
  }
}
