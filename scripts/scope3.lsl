test() { }
default {
    state_entry() {
        integer test = 1; // works fine
        test();
        test = 2;
        if (llGetAttached()) integer test; // $[E10036], but no E10001
    }
}
