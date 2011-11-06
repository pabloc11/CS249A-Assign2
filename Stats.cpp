#include "Engine.h"

namespace Shipping {
  
  class NetworkReactor : public Network::NotifieeConst {
  public:
      void onEntityNew(Entity::Ptr _ptr) const {
        //TODO: fill these in to update the states
      }
      void onEntityDel(Entity::Ptr _ptr) const {
        //TODO: fill these in to update the stats
      }
      NetworkReactor(Network * _n, Stats* _stats) : Network::NotifieeConst(), stats_(_stats) {
        notifierIs(_n);
      }
  private:
    Fwk::Ptr<Stats> stats_;
  };
  
  Stats::Stats(Network* _n) :
    expeditedCount_(0),
    totalCount_(0),
    truckSegmentCount_(0),
    boatSegmentCount_(0),
    planeSegmentCount_(0),
    customerLocationCount_(0),
    portLocationCount_(0),
    truckTerminalCount_(0),
    boatTerminalCount_(0),
    planeTerminalCount_(0),
    reactor_(new NetworkReactor(_n, this))
  {}
}
