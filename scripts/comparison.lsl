default{timer(){

if ([1,2] != [] == 2) 0; // $[E20012] always true
if ([1,2] != [] == 1) 0; // $[E20013] always false

if (3 >= 3) 0; // $[E20012] always true
if (3 >= 2) 0; // $[E20012] always true
if (2 >= 3) 0; // $[E20013] always false
if (3 <= 3) 0; // $[E20012] always true
if (2 <= 3) 0; // $[E20012] always true
if (3 <= 2) 0; // $[E20013] always false

}}
