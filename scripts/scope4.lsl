// Visibility rules

integer a = b;   // $[E10006] Not defined
integer x = x;   // $[E10006] Not defined (checks pre/post order correctness)

fn1(integer x)   // $[E20001] shadows global x
{
    // All globals are visible by functions, regardless of order
    a;              // ok
    b;              // ok
    x;              // ok
    fn2();          // ok
    integer x = 1;  // $[E20001] shadows parameter x
    if (x) x;       // $[E20011] condition always true

    // Locals are only accessible after their declaration
    c;              // $[E10006] Not defined
    {
        c;          // $[E10006] Not defined
    }
    integer c;      // defined here
    c;              // ok
    {
        // Visible on inner blocks
        c;          // ok;
        d;          // $[E10006] Not defined
        integer d;  // defined here
        d;          // ok
    }
    // d goes out of scope when closing the above block
    d;              // $[E10006] Not defined

    // Labels are visible before being declared, but not in nested blocks
    jump f;         // $[E10006] Not defined
    {
        jump f;     // ok
        @f;
        @g;
        jump h;     // ok
        jump g;     // ok
    }
    @h;
    @j;

    jump f;         // $[E10006] Not defined
    jump j;         // ok
    if (TRUE)       // $[E20011] Condition is always true
        state b;    // $[E20005] side effects, $[E10005] is a variable
}

fn2() {}

integer b;

default{timer(){ fn1(1); x; }}

state b             // $[E10001] Already defined
{timer(){}}
