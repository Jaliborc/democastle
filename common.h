#ifndef common_h
#define common_h

#include <irrlicht.h>
#include <iostream>
#include <time.h>
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;

typedef ISceneNodeAnimatorCollisionResponse* collider;
typedef IParticleSystemSceneNode* particles;
typedef IAnimatedMeshSceneNode* animated;
typedef ISceneNodeAnimator* animator;
typedef IParticleEmitter* emitter;
typedef IMeshSceneNode* mesh;
typedef ISceneNode* node;

typedef const char* _string;
typedef IGUIStaticText* staticText;
typedef SColorf COLOR;
typedef vector3df VEC;

extern float Time;
extern float Elapsed;
extern float rand(float, float);
extern void planetRoom();
extern void tennisRoom();

extern IrrlichtDevice* Device;
extern ISceneManager* Scene;
extern IVideoDriver* Driver;
extern IFileSystem * Files;
extern ICameraSceneNode* Camera;
extern ITriangleSelector* MapSelector;

#include "zones.h"
#endif