#pragma once
#include "FaceRenderer.h"
class MacetaFace {
public:
 explicit MacetaFace(TFT_eSPI& display):renderer(display){}
 bool begin();
 void update();
 void setMood(Mood mood, uint32_t durationMs=0);
 Mood mood()const{return animation.frame.mood;}
 void setRandomBrows(bool v){behavior.randomBrows=v;}
 void setAutoBlink(bool v){behavior.autoBlink=v;}
 void setAutoLook(bool v){behavior.autoLook=v;}
 void setAutoGestures(bool v){behavior.autoGestures=v;}
 void setBrowStyle(BrowStyle s){behavior.setBrow(s,millis());}
 void look(float x,float y){behavior.look(x,y,millis());}
 void blink(){behavior.blink(millis());}
 float fps()const{return measuredFps;}
 uint32_t pushedStrips()const{return renderer.pushedStrips();}
private:
 FaceRenderer renderer;FaceAnimation animation;FaceBehavior behavior;
 bool ready=false;uint32_t lastFrame=0,fpsStart=0,frames=0;float measuredFps=0;
};
