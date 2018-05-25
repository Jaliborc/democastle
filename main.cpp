//
// By João Cardoso
//

#include "common.h"

ITriangleSelector* MapSelector;
ICameraSceneNode* Camera;
IrrlichtDevice* Device;
ISceneManager* Scene;
IVideoDriver* Driver;
IFileSystem* Files;

class EventHandler: public IEventReceiver {
  public:
    virtual bool OnEvent(const SEvent& event) {
      Zone::Event(event);
      return false;
    };
};

float Elapsed;
float Time = 0;
float rand(float min, float max) {
  return std::rand() * (max - min) / RAND_MAX - min;
}

int main() {
  // Startup
  EventHandler Handler;
  Device = createDevice(EDT_OPENGL, dimension2d<u32>(1000, 800), 16, false, true, false, &Handler);
  if (!Device)
		return 1;
  
  Scene = Device->getSceneManager();
  Driver = Device->getVideoDriver();
  Files = Device->getFileSystem();
  srand(time(NULL));
  
  // Camera
  Camera = Scene->addCameraSceneNodeFPS(0, 100., .2);
  Camera->setRotation(VEC(0,180,0));
  Scene->setAmbientLight(SColorf(.2, .2, .2));
  
  // Map, Collision, Gravity
  Files->addZipFileArchive("map-20kdm2.pk3");
  mesh map = Scene->addOctreeSceneNode(Scene->getMesh("20kdm2.bsp")->getMesh(0), 0, -1, 1024);
  MapSelector = Scene->createOctreeTriangleSelector(map->getMesh(), map, 128);
  
  map->setPosition(VEC(-1400,-280,-1800));
  map->setTriangleSelector(MapSelector);
  
  collider collision = Scene->createCollisionResponseAnimator(
       MapSelector,
       Camera,
       VEC(30,50,30),
       VEC(0,0,0),
       VEC(0,30,0)
  );

  Camera->addAnimator(collision);
  collision->drop();
  
  // Rooms
  planetRoom();
  tennisRoom();
  
  // Run
  while(Device->run()) {
    if (Device->isWindowActive()) {
      float time = (float)clock() / CLOCKS_PER_SEC;
      Elapsed = time - Time;
      Time = time;
      
      Driver->beginScene(true, true, SColor(255,50,40,40));
      VEC pos = Camera->getPosition();
      Zone::Update(pos);
      
      stringw text = "X: ";
      text += pos.X;
      text += " Y: ";
      text += pos.Y;
      text += " Z: ";
      text += pos.Z;

			Scene->drawAll();
			Driver->endScene();
      Device->setWindowCaption(text.c_str());
    } else {
      Device->yield();
    }
  }
  
  // End
  Device->drop();
	return 0;
}