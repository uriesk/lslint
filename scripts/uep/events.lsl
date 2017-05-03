default {
    touch_start(integer n) {               // $[E20014] unused event parameter
        if (llDetectedKey(0) != llGetOwner())
            return;
    }

    money(key id, integer amount) {}       // $[E20014] $[E20014]

    state_entry() {}

    timer() {}

    run_time_permissions(integer perms) {} // $[E20014]

    sensor(integer n) {}                   // $[E20014]

    no_sensor() {}
}
