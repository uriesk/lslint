// Good:

integer good_i00;
integer good_i01 = 1;
integer good_i02 = -1;
integer good_i03 = - 1;
integer good_i04 = TRUE;
integer good_i05 = FALSE;
integer good_i06 = ALL_SIDES;
integer good_i07 = -ALL_SIDES;
integer good_i08 = - ALL_SIDES;
integer good_i09 = good_i01;
integer good_i10 = good_i09;
integer good_i11 = good_i00;

float good_f00;
float good_f01 = 1;
float good_f02 = -1;
float good_f03 = - 1;
float good_f04 = TRUE;
float good_f05 = FALSE;
float good_f06 = -ALL_SIDES;
float good_f07 = - ALL_SIDES;
float good_f08 = good_i01;
float good_f09 = good_f01;
float good_f10 = 1.0;
float good_f11 = -1.0;
float good_f12 = - 1.0;
float good_f13 = PI;
float good_f14 = -PI;
float good_f15 = - PI;

string good_s00;
string good_s01 = "ok";
string good_s02 = L"x";      // $[E20019] prepends quote, like "x
string good_s03 = good_s01;
string good_s04 = good_s00;  // TODO: Should emit warning in LSO
key good_k00;
string good_s05 = good_k00;  // TODO: Should emit warning in LSO
key good_k01 = "ok";
string good_s06 = good_k01;
key good_k02 = L"x";         // $[E20019]
key good_k03 = good_s00;     // TODO: Should emit warning in LSO
key good_k04 = good_s01;
key good_k05 = good_k01;
key good_k06 = good_k00;     // TODO: Should emit warning in LSO

vector good_v00;
vector good_v01 = <0, 0, 0>;
vector good_v02 = <-0.0, 0.0, 0.0>;
vector good_v03 = <- 0.0, 1, -2>;

rotation good_r00;
rotation good_r01 = <1,2,3,4>;
rotation good_r02 = good_r01;

list good_l00;
list good_l01 = [];
list good_l02 = [<1,2,3>, <-1,2,3>, -1, TRUE, -ALL_SIDES, PI, - PI];

// Bad:

integer bad_i01 = -TRUE;     // $[E10020] global initializer must be constant
integer bad_i02 = -FALSE;    // $[E10020]
integer bad_i03 = -ZERO_VECTOR;    // $[E10020]
integer bad_i04 = -ZERO_ROTATION;  // $[E10020]
integer bad_i05 = -EOF;      // $[E10020]
integer bad_i06 = -good_i01; // $[E10020]
integer bad_i07 = ~1;        // $[E10020]
integer bad_i08 = 1 - 1;     // $[E10020]
integer bad_i09 = -1 - 1;    // $[E10020]
integer bad_i10 = 1 + -1;    // $[E10020]
float bad_f01 = -good_i01;   // $[E10020]
float bad_f02 = -good_f01;   // $[E10020]
float bad_f03 = -TRUE;       // $[E10020]
float bad_f04 = -good_f01;   // $[E10020]
string bad_s01 = - "what?";  // $[E10020]
vector bad_v01 = -<1,2,3>;   // $[E10020]
vector bad_v02 = <1,2,[]>;   // $[E10020]
vector bad_r01 = -r00;       // $[E10020]
list bad_l01 = [-TRUE];      // $[E10020]
list bad_l02 = [-<1,1,1>];   // $[E10020]
list bad_l03 = [1,[2,3],4];  // $[E10020] TODO: add "Lists can't contain lists"
list bad_l04 = -[1];         // $[E10020] (dubious - E10002 would be more appropriate here)

default{timer(){

// Typecasts

// Good
(string)1;
(string)-1;
(string) - 1;
(string) TRUE;
(string) FALSE;
(string) ALL_SIDES;
(string)-ALL_SIDES;
(string) - ALL_SIDES;
(string)-1.0;
(string) - 1.0;
(string)"";
(string)(-TRUE);
(string)(-<0,0,0>);
(string)good_i01;
(string)[-TRUE, -<0,0,0>, ~1];

// Bad
(string)-TRUE;     // $[E10019] syntax error
(string) - FALSE;  // $[E10019]
(string)~1;        // $[E10019]
(string)-<0,0,0>;  // $[E10019]
(string)-good_i01; // $[E10019]
(string)[-"nope"]; // $[E10002] invalid operator

if (good_i00 == 0)               0; // $[E20011] always true
if (good_i00 == good_i00)        0; // $[E20011]
if (good_i01 == 1)               0; // $[E20011]
if (good_i09 == 1)               0; // $[E20011]
if (good_i10 == 1)               0; // $[E20011]
if (good_i11 == 0)               0; // $[E20011]
if (good_f00 == 0.)              0; // $[E20011]
if (good_s00 == "")              0; // $[E20011]
if (good_k00 == "")              0; // $[E20011]
if (good_k00 == good_k00)        0; // $[E20011]
if (good_v00 == ZERO_VECTOR)     0; // $[E20011]
if (good_v00 == good_v01)        0; // $[E20011]
if (good_r00 == ZERO_ROTATION)   0; // $[E20011]
if (good_r02 == <1,2,3,4>)       0; // $[E20011]
if (good_l00 == [])              0; // $[E20011]
if (good_l01 == [])              0; // $[E20011]
if (good_l00 == good_l01)        0; // $[E20011]
if (good_l02 == [0,0,0,0,0,0,0]) 0; // $[E20011] $[E20010] compares lengths

// Use variables to avoid extra warnings
good_i00; good_i01; good_i02; good_i03; good_i04;
good_i05; good_i06; good_i07; good_i08; good_i09;

good_f00; good_f01; good_f02; good_f03; good_f04;
good_f05; good_f06; good_f07; good_f08; good_f09;
good_f10; good_f11; good_f12; good_f13; good_f14;
good_f15;

good_s00; good_s01; good_s02; good_s03; good_s04;
good_s05; good_s06;

good_k00; good_k01; good_k02; good_k03; good_k04;
good_k05; good_k06;

good_v00; good_v01; good_v02; good_v03;

good_l00; good_l01; good_l02;

}}
