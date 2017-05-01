default{timer(){

if ([1,2] != [] == 2) 0; // $[E20011] always true
if ([1,2] != [] == 1) 0; // $[E20012] always false

if (3 >= 3) 0; // $[E20011] always true
if (3 >= 2) 0; // $[E20011] always true
if (2 >= 3) 0; // $[E20012] always false
if (3 <= 3) 0; // $[E20011] always true
if (2 <= 3) 0; // $[E20011] always true
if (3 <= 2) 0; // $[E20012] always false
if ((~-36*7^3|5) == 253) 0; // $[E20011]
if (~-3*5 == 14) 0;         // $[E20011]
if (~(-3)*5 == 10) 0;       // $[E20011]
if (~3*5 == -20) 0;         // $[E20011]
if (~(3*5) == -16) 0;       // $[E20011]

}}
