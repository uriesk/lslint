/* comment A */
a // $[E20009] unused
() { }
/* comment
   A */
b // $[E20009] unused
() { }
/*
 *  comment B
 **/
c // $[E20009] unused
() { }
default {
   /* comment B */
   state_entry() {
      /* comment C */
   }
}
