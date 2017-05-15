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

      if (0) 0;                   // $[E20012] false
      if (0.0) 0;                 // $[E20012] false
      if (<0.0,0.0,0.0>) 0;       // $[E20012] false
      if (<0.0,0.0,0.0,1.0>) 0;   // $[E20012] false
      if (<0.0,0.0,0.0,0.0>) 0;   // $[E20011] true
      if (<0.0,0.0,0.0,-1.0>) 0;  // $[E20011] true
      if ("") 0;                  // $[E20012] false
      if ((key)"") 0;             // TODO, should be [E20012] false
      if ((key)TEXTURE_BLANK) 0;  // TODO, should be [E20011] true
      if ((key)NULL_KEY) 0;       // TODO, should be [E20012] false
      if ((key)"AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA") 0; // TODO, should be [E20011] true
      if ([]) 0;                  // $[E20012] false
      if (["", ""]) 0;            // $[E20011] true

      if ("\t\t\t\t\t\t" == "                        ") 0; // $[E20011] true
   }
}
