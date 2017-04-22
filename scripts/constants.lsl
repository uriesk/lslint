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
   }
}
