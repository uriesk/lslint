default{timer(){

  {jump x; @x;}
  {jump x;
   @
    x    // $[E20022] Jumps to last label
     ;}
}}
