key g_k1;
key g_k2 = "";
key g_k3 = "x";
key g_k4 = "GGGGGGGG-GGGG-GGGG-GGGG-GGGGGGGGGGGG";
key g_k5 = NULL_KEY;
key g_k6 = "00000000-0000-0000-0000-000000000000";
key g_k7 = TEXTURE_BLANK;
key g_k8 = "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA";

default {
   state_entry() {
      integer x = PRIM_TEXTURE;
      if (x == 17) 0;              // $[E20011] always true
      integer y = PRIM_GLOW + PRIM_TEXTURE;
      if (y == 42) 0;              // FIXME: should be $ [E20011] always true
      integer
              PARCEL_DETAILS_DESC  // $[E10005] invalid
                                 ;
      integer
              PARCEL_DETAILS_NAME  // $[E10005] invalid
                                  = 5;
      if (PARCEL_DETAILS_NAME == 0) 0; // $[E20011] true (regression test)

      PRI_GLOW                     // $[E10006] undeclared
               = 2;
      PRIM_GLOW                    // $[E10005] invalid
                = 2;

      llOwnerSay(
                 L"This is a long(?) string"   // $[E20019] Prepends a quote
                                            );

      1.0 + 1. + .1 +
                      .      // $[E10019] Syntax error
                         + 1.0E+01 + 1.e1 + .1e1;
      00.e-1 + 1e1 + 1e+1 + 1e-1;

      1.0f + 1.f + .1f +
                         .f  // $[E10019]
                            + 1.0E+01f + 1.e1f + .1e1f;
      00.e-1f +
                1e1f         // $[E10019] f is illegal if there's no period
                     +
                       1E+2f // $[E10019]
                            ;

      if (/* $[E20011] true $[E20019] prepends quote */ L"a
\b" == "\"a\nb") 0;

      if (0) 0;                   // $[E20012] false
      if (0.0) 0;                 // $[E20012] false
      if (<0.0,0.0,0.0>) 0;       // $[E20012] false
      if (<0.0,0.0,0.0,1.0>) 0;   // $[E20012] false
      if (<0.0,0.0,0.0,0.0>) 0;   // $[E20011] true
      if (<0.0,0.0,0.0,-1.0>) 0;  // $[E20011] true
      if ("") 0;                  // $[E20012] false

      key k1 = TEXTURE_BLANK;
      key k2 = NULL_KEY;
      key k3 = "";
      key k4 = "x";

      if (k1) 0;                  // $[E20011] true
      if (k2) 0;                  // $[E20012] false
      if (k3) 0;                  // $[E20012] false
      if (k4) 0;                  // $[E20012] false
      if (g_k1) 0;                // $[E20012] false
      if (g_k2) 0;                // $[E20012] false
      if (g_k3) 0;                // $[E20012] false
      if (g_k4) 0;                // $[E20012] false
      if (g_k5) 0;                // $[E20012] false
      if (g_k6) 0;                // $[E20012] false
      if (g_k7) 0;                // $[E20011] true
      if (g_k8) 0;                // $[E20011] true

      if ((key)"") 0;             // $[E20012] false
      if ((key)TEXTURE_BLANK) 0;  // $[E20011] true
      if ((key)NULL_KEY) 0;       // $[E20012] false
      if ((key)"AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA") 0; // $[E20011] true
      if ([]) 0;                  // $[E20012] false
      if (["", ""]) 0;            // $[E20011] true

      if ("\t\t\t\t\t\t" == "                        ") 0; // $[E20011] true

      if ((list)x == [""]) 0;     // $[E20010] only length $[E20011] true
      if ((list)x != [""]) 0;     // $[E20010] only length $[E20012] false
   }
}
