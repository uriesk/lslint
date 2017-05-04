default {
   state_entry() {
      integer
             x                     // $[E20009] unused
               = PRIM_TEXTURE;
      integer
              y                    // $[E20009] unused
                = PRIM_GLOW + PRIM_TEXTURE;
      integer
              // FIXME: should not emit E20009
              PARCEL_DETAILS_DESC  // $[E20009] unused??, $[E10025] invalid
                                 ;
      integer
              // FIXME: should not emit E20009
              PARCEL_DETAILS_NAME  // $[E20009] unused??, $[E10025] invalid
                                  = 5;

      PRI_GLOW                     // $[E10006] undeclared
               = 2;
      PRIM_GLOW                    // $[E10024] invalid
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
   }
}
