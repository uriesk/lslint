default{timer(){

    print("yay");                // $[E20020] print does nothing
    print(3);                    // $[E20020]
    print(<4,5,6>);              // $[E20020]

    string s = print("yay");     // $[E20020]
    integer i = print(3);        // $[E20020]
    vector v = print(<4,5,6>);   // $[E20020]

    s = print(<4,5,6>);          // $[E20020] $[E10002] string = vector invalid

    s; i; v; // avoid E20009
}}
