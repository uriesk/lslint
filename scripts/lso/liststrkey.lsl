default
{
    state_entry()
    {
        key k = llGetObjectName();
        string s = llGetKey();

        list ls =
                  (list)    // $[E20021] could result in a string
                        k;
        ls = ls +
                  (list)         // $[E20021] could result in a key
                        s;

        if (llGetListEntryType(ls, 0) == TYPE_STRING
         && llGetListEntryType(ls, 1) == TYPE_KEY)
        {
            // This is printed!
            llOwnerSay("LSO has weird bugs");
        }
    }
}
