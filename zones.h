//
// By João Cardoso
//

#ifndef zones_h
#define zones_h
#include "common.h"

class Zone {
  static const int NUM_ZONES = 1; // Zones are being created manually, and will rarely change
  static Zone* Zones[NUM_ZONES];
  static Zone* current;
  static int numZones;
  
  virtual void OnEvent(const SEvent&) = 0;
  virtual void OnUpdate() = 0;
  virtual void OnEnter() = 0;
  virtual void OnLeave() = 0;
  aabbox3df area;
  
  public:
    Zone(aabbox3df&);
    static void Update(VEC&);
    static void Event(const SEvent&);
};

#endif
