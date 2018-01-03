list g1;
key a = "";
list g2 = [1, 2, 3, a];
list g3 = [1, 2, 3,
                    g2   // $[E10038] Lists can't contain lists
                      ];
list g4 =
          [              // $[E10020] Global initializer must be constant
                         // (in globals, the syntax doesn't allow [[]])
           []];

default{timer(){
  g1 = [
        g2               // $[E10038]
          ,
           g3            // $[E10038]
             ,
              llGetPrimitiveParams      // $[E10038]
                                  ([])];
  g2 = [
        [                // $[E10038]
         ]];

  g3 = [
        (                // $[E10038]
         list)0];
}}
