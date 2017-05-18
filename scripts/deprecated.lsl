default{timer(){

string s;
key k;
integer i;
float f;
vector v;
//rotation r;
list l;

llSoundPreload(s);            // $[E10004] use llPreloadSound
llSound(s,f,i,i);             // $[E10004] use llPlaySound, llLoopSound, or llTriggerSound
llMakeExplosion(i,f,f,f,f,s,v); // $[E10004] use llParticleSystem
llMakeFire(i,f,f,f,f,s,v);    // $[E10004] use llParticleSystem
llMakeFountain(i,f,f,f,f,i,s,v,f); // $[E10004] use llParticleSystem
llMakeSmoke(i,f,f,f,f,s,v);   // $[E10004] use llParticleSystem
llRemoteLoadScript(k,s,i,i);  // $[E10004] use llRemoteLoadScriptPin and llSetRemoteScriptAccessPin
s = llXorBase64Strings(s,s);  // $[E10004] use llXorBase64
s = llXorBase64StringsCorrect(s,s); // $[E10004] use llXorBase64
llRemoteDataSetRegion();      // $[E10004] use llOpenRemoteDataChannel
llSetPrimURL(s);              // $[E10004] use llSetPrimMediaParams
llRefreshPrimURL();           // $[E10004] use llSetPrimMediaParams
llTakeCamera(k);              // $[E10004] use llSetCameraParams
llReleaseCamera();            // $[E10004] use llClearCameraParams
llPointAt(v);                 // $[E10003] deprecated without replacement
llStopPointAt();              // $[E10003] deprecated without replacement
f = llCloud(v);               // $[E10003] deprecated without replacement
llClearExperiencePermissions(k); // $[E10003] deprecated without replacement
llCollisionSprite(s);         // $[E10003] deprecated without replacement
l = llGetExperienceList(k);   // $[E10003] deprecated without replacement

llGodLikeRezObject(k,v);      // $[E10037] requires god mode
llSetInventoryPermMask(s,i,i);// $[E10037] requires god mode
llSetObjectPermMask(i,i);     // $[E10037] requires god mode

}}
