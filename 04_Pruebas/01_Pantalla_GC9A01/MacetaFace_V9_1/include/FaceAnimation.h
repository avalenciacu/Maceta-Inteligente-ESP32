#pragma once
#include "FaceBehavior.h"
struct FaceFrame {
 Mood mood=Mood::NORMAL;
 Expression expr=expressionFor(Mood::NORMAL);
 float x=0,y=0,gazeX=0,gazeY=0,arch=15,raiseL=0,raiseR=0,opening=0,age=0;
 float happy=0, skeptical=0;
 float phase=0, winkL=1,winkR=1,pupil=19.5f;
 int variant=1;
};
class FaceAnimation {
public:
 FaceFrame frame;
 void begin(Mood m,uint32_t now);
 void onMood(Mood m,uint32_t now);
 void setDuration(uint32_t ms){duration=ms;}
 void update(const FaceBehavior& b,uint32_t now);
private:
 Expression current=expressionFor(Mood::NORMAL);
 uint32_t entered=0,last=0,duration=0;
};
