#pragma once
#include <TFT_eSPI.h>
#include "FaceAnimation.h"
#include "FaceConfig.h"
class FaceRenderer {
public:
 explicit FaceRenderer(TFT_eSPI& display):tft(display),sprite(&display){}
 bool begin();
 void render(const FaceFrame& f);
 uint32_t pushedStrips() const {return pushed;}
private:
 TFT_eSPI& tft; TFT_eSprite sprite;
 int offset=0; int16_t left[240],right[240];
 uint32_t hashes[FaceConfig::STRIPS]={}; bool valid[FaceConfig::STRIPS]={};
 uint32_t pushed=0; uint16_t skin=0;
 void line(int x1,int x2,int y,uint16_t c);
 void ellipse(float x,float y,float rx,float ry,uint16_t c);
 void curve(float x0,float y0,float x1,float y1,float x2,float y2,float width,uint16_t c);
 void eye(float x,float y,const FaceFrame& f);
 void segment(float x,float y,float a,float b,float w,uint16_t c);
 void polygon(const float* xy,int n,uint16_t c);
 void objects(const FaceFrame& f);
 bool specialMouth(float x,float y,const FaceFrame& f);
 void mouth(float x,float y,const FaceFrame& f);
 void effects(const FaceFrame& f);
 void shape(float x,float y,float w,float h,uint16_t c);
};
