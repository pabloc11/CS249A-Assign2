#include "InstanceImpl.h"

/*
 * This is the entry point for your library.
 * The client program will call this function to get a handle
 * on the Instance::Manager object, and from there will use
 * that object to interact with the middle layer (which will
 * in turn interact with the engine layer).
 */
Fwk::Ptr<Instance::Manager> shippingInstanceManager() {
    return new Shipping::ManagerImpl();
}