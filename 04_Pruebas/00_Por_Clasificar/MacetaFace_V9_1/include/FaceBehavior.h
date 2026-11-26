#pragma once
#include "FaceExpressions.h"
class FaceBehavior {
public:
 void begin(uint32_t now);
 void onMood(Mood m,uint32_t now);
 void update(Mood m,uint32_t now);
 void look(float x,float y,uint32_t now);
 void blink(uint32_t now);
 void setBrow(BrowStyle style,uint32_t now);
 bool autoBlink=true,autoLook=true,randomBrows=true,autoGestures=true;
 float targetX=0,targetY=0,raiseL=0,raiseR=0;
 BrowStyle brow=BrowStyle::CURVED;
 uint32_t blinkAt=0; bool blinking=false;
private:
 uint32_t nextLook=0,nextBlink=0,nextBrow=0,nextAsym=0;
 int lastSide=1,side=0;
};
class MoodShuffle {
public: Mood next(Mood current);
private: Mood bag[52]; int pos=52;
};
