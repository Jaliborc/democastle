//
// By João Cardoso
//

#include "zones.h"

int Zone::numZones = 0;
Zone* Zone::current;
Zone* Zone::Zones[NUM_ZONES];
Zone::Zone(aabbox3df& new_area) {
  Zones[numZones] = this;
  area = new_area;
  numZones++;
};
    
void Zone::Update(VEC &point) {
  if (current != NULL) {
    if (current->area.isPointInside(point)) {
      current->OnUpdate();
      return;
    } else {
      current->OnLeave();
    };
  };
  
  for (int i = 0; i < NUM_ZONES; i++) {
    Zone* target = Zones[i];
  
    if (target->area.isPointInside(point)) {
      current = target;
      target->OnEnter();
      return;
    };
  };
      
  current = NULL;
};
  
void Zone::Event(const SEvent &event) {
  if (current != NULL) {
    current->OnEvent(event);
  };
}