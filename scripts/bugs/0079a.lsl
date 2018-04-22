integer a; // $[E20009] Declared but not used
b;         // $[E10019] Syntax error (it was causing a segfault)
default { state_entry() {} }
