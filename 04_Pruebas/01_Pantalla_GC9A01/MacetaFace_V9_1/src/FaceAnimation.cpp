#include "FaceAnimation.h"
#include "FaceConfig.h"
#include <math.h>
#include "FaceActivities.h"
static float smooth(float x){x=fmaxf(0,fminf(1,x));return x*x*(3-2*x);}
static void approach(float& a,float b,float k){a+=(b-a)*k;}
void FaceAnimation::begin(Mood m,uint32_t now){frame.mood=m;current=expressionFor(m);frame.expr=current;entered=last=now;}
void FaceAnimation::onMood(Mood m,uint32_t now){frame.mood=m;entered=now;frame.opening=0;frame.variant=random(2)?1:-1;}
void FaceAnimation::update(const FaceBehavior& b,uint32_t now){
 float dt=fminf((now-last)*.001f,.15f);last=now;frame.age=(now-entered)*.001f;
 float k=1-expf(-7*dt),g=1-expf(-15*dt),f=1-expf(-6*dt);
 Expression t=expressionFor(frame.mood);
 approach(current.eye,t.eye,k);approach(current.smile,t.smile,k);approach(current.mouth,t.mouth,k);approach(current.brow,t.brow,k);approach(current.tilt,t.tilt,k);approach(current.heat,t.heat,k);approach(current.blush,t.blush,k);
 frame.expr=current;
 bool moving=frame.mood==Mood::CURIOUS||frame.mood==Mood::SURPRISED;
 if((int)frame.mood<13){approach(frame.x,moving?b.targetX*FaceConfig::FACE_X:0,f);approach(frame.y,moving?b.targetY*FaceConfig::FACE_Y:0,f);}
 bool rest=frame.mood==Mood::SLEEPING||frame.mood==Mood::TIRED_YAWNING||frame.mood==Mood::PETTED;
 if((int)frame.mood<13){approach(frame.gazeX,rest?0:b.targetX,g);approach(frame.gazeY,rest?0:b.targetY,g);}
 approach(frame.arch,b.brow==BrowStyle::CURVED?15:b.brow==BrowStyle::SOFT?5:0,k);
 approach(frame.raiseL,b.raiseL,f);approach(frame.raiseR,b.raiseR,f);
 approach(frame.happy,frame.mood==Mood::HAPPY?1:0,k);approach(frame.skeptical,frame.mood==Mood::SKEPTICAL?1:0,k);
 if(b.blinking&&frame.mood!=Mood::TOUCH_ALERT&&frame.mood!=Mood::WAKE_UP){float q=(now-b.blinkAt)/290.f;if(q<1)frame.expr.eye*=q<.32f?1-smooth(q/.32f):smooth((q-.32f)/.68f);}
 float opening=0;
 if(frame.mood==Mood::TIRED_YAWNING){float c=fmodf(duration?frame.age*9000.f/duration:frame.age,9);if(c>=2&&c<3.25f)opening=smooth((c-2)/1.25f);else if(c>=3.25f&&c<4.15f)opening=1;else if(c>=4.15f&&c<5.4f)opening=1-smooth((c-4.15f)/1.25f);frame.expr.eye*=1-.38f*opening;}
 approach(frame.opening,opening,k);
 applyActivity(frame,b,duration,k,g,f);
}
