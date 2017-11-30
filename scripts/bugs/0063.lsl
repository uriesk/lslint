default{timer(){

    vector v;
    ZERO_VECTOR.x;             // $[E10008] Invalid member ZERO_VECTOR.x
    ZERO_ROTATION.s;           // $[E10008]
    TOUCH_INVALID_TEXCOORD.z;  // $[E10008]
    <1,0,0>.x;                 // $[E10019] syntax error, unexpected PERIOD
    llGetPos().z;              // $[E10019]
    v.s;                       // $[E10008]
    v.y;                       // works

}}
