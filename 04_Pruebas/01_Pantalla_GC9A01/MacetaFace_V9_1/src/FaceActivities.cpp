#include "FaceActivities.h"
#include <math.h>
static float smooth(float a){a=fmaxf(0,fminf(1,a));return a*a*(3-2*a);}
static float blink(float t,float s,float c,float o){float a=t-s;return a<0||a>=c+o?1:a<c?1-smooth(a/c):smooth((a-c)/o);}
void applyActivity(FaceFrame& f,const FaceBehavior& b,uint32_t duration,float k,float g,float motion){
 int m=(int)f.mood;float natural=m==13?2.4f:m==14?5.5f:m==16?5:m>=37?12:10;
 float u=duration?fminf(1,f.age*1000/duration):fmodf(f.age,natural)/natural;
 float t=u*natural;f.phase=t;f.winkL=f.winkR=1;f.pupil=19.5f;
 if(m<13)return;
 float x=b.targetX,y=b.targetY,dx=0,dy=0;int s=f.variant;
 switch(m){
 case 13:{float a=smooth(u/.12f),v=smooth((u-.32f)/.14f),z=smooth((u-.64f)/.12f);x=s*a*(1-2*v)*(1-z);y=(u<.64f?-.15f*x:.8f*s*z*(1-smooth((u-.87f)/.13f)));dx=x*27;dy=y*18;f.expr.eye=1.04;f.arch=15;break;}
 case 14:{float a=u*5.5f;f.expr.eye=(a<.65f?0:a<1.9f?.62f*smooth((a-.65f)/1.25f):.62f+.38f*smooth((a-1.9f)/2))*blink(a,1.95f,.18f,.47f)*blink(a,3.05f,.14f,.38f);f.happy=smooth((a-3.8f));f.expr.smile=.12f+.88f*f.happy;f.expr.brow=5*(1-smooth(a/3.9f))-3*f.happy;x=.35f*sinf(a*2)*(1-f.happy);y=0;break;}
 case 15:dx=sinf(t*25)*2.7f;f.pupil=15;break;
 case 16:x=.7;y=.6;break;
 case 17:case 21:case 30:x=y=0;break;
 case 18:f.pupil=16;break;
 case 19:dx=sinf(t*34)*2.5f;dy=cosf(t*31);break;
 case 20:x=0;y=.6;break;
 case 22:{float w=fmodf(t,4.5f);float a=1-smooth((w-.6f)/.16f)*smooth((1.4f-w)/.2f);if(s<0)f.winkL=a;else f.winkR=a;break;}
 case 23:case 49:dy=sinf(t*13)*3;break;
 case 24:x=s*.5f;y=-.9f;break;
 case 25:case 39:f.expr.eye=.15f+.85f*powf(fmaxf(0,sinf(t*1.7f)),4);dx=sinf(t*2)*5;dy=sinf(t*2)*2;break;
 case 26:x=sinf(t*.9f)*.8f;y=-.7f;break;
 case 27:x=sinf(t*.85f)*.9f;y=.15f;break;
 case 33:dy=sinf(t*2)*1.5f;x=y=0;break;
 case 28:x=s*.85f;y=-.1f;if(s<0)f.raiseL=-10;else f.raiseR=-10;break;
 case 29:x=sinf(t*1.5f);y=sinf(t*1.1f)*.7f;dx=x*20;dy=y*14;f.raiseL=-fmaxf(0,-x)*10;f.raiseR=-fmaxf(0,x)*10;break;
 case 31:x=s*sinf(t*.8f)*.65f;y=.8f;break;
 case 32:dy=sinf(t*5)*2;break;
 case 34:if(fmodf(t,4)<2){if(s<0)f.winkL=0;else f.winkR=0;}break;
 case 36:x=sinf(t*1.7f);y=0;break;
 case 37:x=sinf(t*2.5f)*.9f;y=.8f;break;
 case 40:x=sinf(t*1.5f);y=sinf(t*.9f)*.65f;dx=x*19;dy=y*15;break;
 case 41:f.expr.eye=t>7?.03f:t>5.5f?1.2f:.72f;x=sinf(t*4)*.75f;y=t>5.5f?0:.8f;break;
 case 42:x=.9f;y=.8f-fminf(1,fmodf(t,3)/2)*1.5f;dx=x*10;break;
 case 43:f.expr.eye=fmodf(t,4)<2?.05f:.5f;x=.7f;break;
 case 44:f.expr.eye=fmodf(t,6)>2?.03f:1;x=.7f;y=.5f;break;
 case 45:x=.8f;y=.5f-fminf(1,t/6)*1.5f;dx=x*12;dy=y*12;break;
 case 46:x=.8f;y=.3f;break;
 case 47:x=sinf(t*1.7f)*.9f;y=.85f;break;
 case 48:f.expr.eye=fmodf(t,5)>3.6f&&fmodf(t,5)<3.9f?.05f:1;break;
 case 50:f.expr.eye=t>8?.03f:.6f;dy=-fmaxf(0,sinf(t*2.7f))*3;break;
 case 51:f.expr.eye=fmodf(t,3)>1.3f?.03f:.92f;x=.75f;y=.65f;break;
 }
 // Extended expressions own their movement; smoothly approach each pose.
 float a=m==13?fminf(1,motion*2.6f):motion; f.x+=(dx-f.x)*a;f.y+=(dy-f.y)*a;f.gazeX+=(x-f.gazeX)*g;f.gazeY+=(y-f.gazeY)*g;
}
