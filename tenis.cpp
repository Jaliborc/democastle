#include "common.h"
#include <list>

// Balls
class Ball {
  class CollisionHandler: public ICollisionCallback {
    public:
      Ball* target;
      virtual bool onCollision(const ISceneNodeAnimatorCollisionResponse &animator) {
        target->OnCollision(animator.getCollisionPoint());
        return false;
      };
  };
  
  protected:
    CollisionHandler handler;
    animated model;
    VEC velocity;
  
  public:
    static std::list<Ball*> MovingBalls;
    
    // Create & Destroy
    Ball() {
      float vAngle = (180 - Camera->getRotation().X) * PI/180;
      float hAngle = Camera->getRotation().Y * PI/180;
      float random = rand(0, 1);
      _string file;
    
      if (random > 0.5) {
        file = "../../Projects/A Maze/pokeball.3ds";
      } else {
        file = "../../Projects/A Maze/yangball.3ds";
      }
      
      velocity = VEC(sin(hAngle), -sin(vAngle) * 1.5, cos(hAngle)) * 350;
      model = Scene->addAnimatedMeshSceneNode(Scene->getMesh(file), 0, -1, Camera->getPosition() + velocity / 15);
      model->setScale(VEC(2, 2, 2));

      collider collision = Scene->createCollisionResponseAnimator(MapSelector, model, VEC(10,10,10), VEC(0,0,0));
      model->addAnimator(collision);
      collision->setCollisionCallback(&handler);
      collision->drop();
      
      MovingBalls.push_front(this);
      handler.target = this;
    };
  
    virtual ~Ball() {
      model->remove();
    }
  
  
    // Simple physycs
    //  Gravity could have been implemented using the engine. But it's useful for a programming demo.
    static inline void OnUpdate() {
      std::list<Ball*>::iterator i;
      
      for (i = MovingBalls.begin(); i != MovingBalls.end(); i++) {
        Ball* ball = *i;
        animated model = ball->model;

        ball->velocity.Y -= 260 * Elapsed;
        model->setPosition(model->getPosition() + ball->velocity * Elapsed);
      }
    };
  
    inline void OnCollision(VEC normal) {
      normal -= model->getPosition();
      normal *= normal;
      
      velocity *= .85;
      velocity -= normal.normalize() * velocity * 2;
      
      if (normal.Y == 1 and !normal.X and !normal.Z and velocity.Y <= 7) {
        MovingBalls.remove(this); // Ignore dead balls
      }
    };
  
    // Peformance improvements
    //    Delete balls that are too far from the camera.
    static inline void OnCleanup() {
      std::list<Ball*>::iterator i = MovingBalls.begin();
      
      while (i != MovingBalls.end()) {
        Ball* ball = *i;
        
        if (ball->model->getPosition().getDistanceFrom(Camera->getPosition()) > 700) {
          i = MovingBalls.erase(i);
          delete ball;
        } else {
          ++i;
        };
      };
    };
};

std::list<Ball*> Ball::MovingBalls;


// Room
inline VEC interpolate(VEC start, VEC end, int speed) {
  return start + (end - start) * Elapsed * speed;
}

class TennisRoom: public Zone {
  static node Hand;
  static animated Racquet;
  
  static float nextClear;
  static float firing;
  static bool fired;
  
  public:
    TennisRoom(aabbox3df area) : Zone(area) {};
  
    virtual void OnEnter() {
      nextClear = Time + 2;
      Hand = Scene->addEmptySceneNode(Camera);
      Hand->setPosition(VEC(3, -3, 15));
      
      Racquet = Scene->addAnimatedMeshSceneNode(
        Scene->getMesh("../../Projects/A Maze/racquet.3ds"),
        Hand, -1, VEC(0, 3, 0), VEC(100, 115, 180)
      );
    };
  
    virtual void OnLeave() {
      Racquet->remove();
    };
  
    virtual void OnUpdate() {
      // Anim Racquet
      //
      //    Hand-coding of animations is generally considered to be a bad practice, and bad for performance. 
      //    Yet, it is useful for a programming demo.
    
      VEC pos, angle;
      firing -= Elapsed;
    
      if (firing > 6./5) {
        pos = interpolate(Hand->getPosition(), VEC(5, -1, -12), 5);
        angle = interpolate(Racquet->getRotation(), VEC(200, -28, 180), 5);
      } else if (firing > 4./5) {
        pos = interpolate(Hand->getPosition(), VEC(3, 1, 12), 25);
        angle = interpolate(Racquet->getRotation(), VEC(85, 10, 185), 25);
      
        if (not fired) {
          fired = true;
          new Ball();
        }
      } else {
        float step = sin(Time * 1.5 * PI);
        float step2 = sin((Time + .125) * 1.5 * PI);
      
        pos = VEC(3, step / 2. - 3.25, 12 + step / 8.);
        angle = VEC(85 + step2 * 3, 65,  180 + step2 * 2);
      
        if (firing > 0) {
          pos = interpolate(Hand->getPosition(), pos, 5);
          angle = interpolate(Racquet->getRotation(), angle, 5);
        }
      }
    
      Hand->setPosition(pos);
      Racquet->setRotation(angle);
    
      // Balls Physic
      Ball::OnUpdate();
    
      if (Time >= nextClear) {
        Ball::OnCleanup();
        nextClear = Time + 2;
      };
    };
  
    virtual void OnEvent(const SEvent& event) {
      if (event.EventType == EET_MOUSE_INPUT_EVENT and event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN and fired) {
        firing = 8./5;
        fired = false;
      };
    }
};

node TennisRoom::Hand; // Yay for code repetition!
animated TennisRoom::Racquet;

bool TennisRoom::fired = true;
float TennisRoom::firing = 0;
float TennisRoom::nextClear;


// Starts Here
void tennisRoom() {
  new TennisRoom(aabbox3df(-665,210,-800, 10,500,100));
};