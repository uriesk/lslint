default{timer(){

    list L;

    rotation q = (rotation) L [1];

    L [1] = q;

    L[2] = (integer)L[1];

    L = L[2] = (integer)L[1]; // show that it's a list

}}
