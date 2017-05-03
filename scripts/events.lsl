boy(integer
            girl       // $[E20009] declared but never used
                ) {}


state_entry            // $[E10031] can't be used as function name
           () {
}

default {
   state_entry() {
      // FIXME: Wrong error location
      boy              // $[E10011] wrong arg type (integer vs string)
         (
          "foo"        // Should be here: [E10011] wrong arg type
               );
   }
   on_rez(integer paramie) {
   }
   // FIXME: Wrong error location for E10027
   on_rez              // $[E10032] multiple handlers, $[E10027] wrong type
         (
          string       // Should be here: [E10027] wrong parameter type
                 paramie) {
   }
   // FIXME: Wrong error location for E10027
   object_rez          // $[E10027] wrong type
             (
              float    // Should be here: [E10027] wrong parameter type
                    s) {
   }
   // FIXME: Wrong error location for E10029
   listen              // $[E10029] too few params
         (integer
                  channel    // Acceptable location for [E10029]
                         )   // Correct location for [E10029]
                           {
   }
   // FIXME: Wrong error location for E10028
   changed             // $[E10028] too many params
          (integer
                   changed           // $[E10005] it's an event name
                          ,          // Correct location for [E10028]
                            integer  // Acceptable location for [E10028]
                                    extra) {
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
