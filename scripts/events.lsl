boy(integer
            girl       // $[E20009] declared but never used
                ) {}


state_entry            // $[E10031] can't be used as function name
           () {
}

default {
   state_entry() {
      boy(
          "foo"        // $[E10011] wrong arg type (integer vs string)
               );
      boy(1,
             "foo"     // $[E10012] too many arguments
                  );

      boy              // $[E10013] too few arguments
         (
          )            // Ideal location for E10013 above
           ;
   }
   on_rez(integer paramie) {
   }
   // TODO: Improve error location for E10027
   on_rez              // $[E10032] multiple handlers
         (
          string       // Should be here: [E10027] wrong parameter type
                 paramie   // Reported here: $[E10027] wrong parameter type
                        ) {
   }
   // TODO: Improve error location for E10027
   object_rez
             (
              float    // Should be here: [E10027] wrong parameter type
                    s  // Reported here: $[E10027] wrong parameter type
                     ) {
   }
   // TODO: Improve error location for E10028
   changed
          (integer
                   changed           // $[E10005] it's an event name
                          ,          // Correct location for [E10028]
                            integer  // Acceptable location for [E10028]
                                    extra  // $[E10028] too many params
                                         ) {
   }
   // TODO: Improve error location for E10029
   listen              // $[E10029] too few params
         (integer
                  channel    // Acceptable location for [E10029]
                         )   // Correct location for [E10029]
                           {
   }
   foo                 // $[E10030] invalid event
      () {
   }
   bar                 // $[E10030] invalid event
      (integer baz) {
   }
   TEXTURE_BLANK       // $[E10030] $[E10005] attempt to use as event but it's constant (FIXME: error message can be improved)
                (integer x) {
   }
}
