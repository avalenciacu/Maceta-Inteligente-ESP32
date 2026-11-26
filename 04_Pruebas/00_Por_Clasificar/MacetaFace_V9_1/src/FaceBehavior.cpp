#include "FaceBehavior.h"
#include "FaceConfig.h"
#include <math.h>
static bool due(uint32_t now,uint32_t t){return static_cast<int32_t>(now-t)>=0;}
static uint32_t interval(long a,long b){return static_cast<uint32_t>(random(a,b+1));}
void FaceBehavior::begin(uint32_t now){nextLook=now+interval(2000,5400);nextBlink=now+interval(3000,7000);nextBrow=now+interval(FaceConfig::BROW_MIN,FaceConfig::BROW_MAX);nextAsym=now+interval(1500,3500);}
void FaceBehavior::onMood(Mood m,uint32_t now){
 side=0;raiseL=raiseR=0;nextAsym=now+interval(1500,3500);nextLook=now+interval(1200,3500);
 if(m==Mood::SAD){targetX=0;targetY=.35f;}
 else if(m==Mood::SKEPTICAL){targetX=random(2)? .8f:-.8f;targetY=0;brow=BrowStyle::FLAT;nextBrow=now+interval(FaceConfig::BROW_MIN,FaceConfig::BROW_MAX);}
 else if(m==Mood::SURPRISED||m==Mood::CURIOUS){targetX=random(2)? .85f:-.85f;targetY=random(-60,61)/100.f;}
 else {targetX=targetY=0;}
}
void FaceBehavior::look(float x,float y,uint32_t now){float d=hypotf(x,y);if(d<1)d=1;targetX=x/d;targetY=y/d;nextLook=now+4000;}
void FaceBehavior::blink(uint32_t now){blinkAt=now;blinking=true;nextBlink=now+interval(3000,7000);}
void FaceBehavior::setBrow(BrowStyle s,uint32_t now){brow=s;nextBrow=now+interval(FaceConfig::BROW_MIN,FaceConfig::BROW_MAX);}
void FaceBehavior::update(Mood m,uint32_t now){
 bool rest=m==Mood::SLEEPING||m==Mood::TIRED_YAWNING||m==Mood::PETTED;
 if(blinking&&now-blinkAt>=290)blinking=false;
 if(autoBlink&&!rest&&due(now,nextBlink))blink(now);
 if(autoLook&&!rest&&m!=Mood::SAD&&m!=Mood::ANGRY&&due(now,nextLook)){
  float x=random(-100,101)/100.f,y=random(-85,86)/100.f;
  if(m==Mood::SKEPTICAL)y*=.3f;
  look(x,y,now);nextLook=now+interval(2000,5400);
 }
 if(randomBrows&&due(now,nextBrow)){
  brow=static_cast<BrowStyle>((static_cast<int>(brow)+random(1,3))%3);
  nextBrow=now+interval(FaceConfig::BROW_MIN,FaceConfig::BROW_MAX);
 }
 if(m==Mood::CURIOUS&&autoGestures&&due(now,nextAsym)){
  if(side==0){side=-lastSide;lastSide=side;nextAsym=now+interval(2200,3800);}
  else{side=0;nextAsym=now+interval(2000,4500);}
 }
 raiseL=(m==Mood::CURIOUS&&autoGestures&&side<0)?-10:0;
 raiseR=(m==Mood::CURIOUS&&autoGestures&&side>0)?-10:0;
}
Mood MoodShuffle::next(Mood current){
 if(pos>=52){for(int i=0;i<52;i++)bag[i]=static_cast<Mood>(i);for(int i=51;i>0;i--){int j=random(i+1);Mood t=bag[i];bag[i]=bag[j];bag[j]=t;}if(bag[0]==current){Mood t=bag[0];bag[0]=bag[1];bag[1]=t;}pos=0;}
 return bag[pos++];
}
