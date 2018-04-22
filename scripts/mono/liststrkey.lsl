default
{
    state_entry()
    {
        key k = llGetObjectName();
        string s = llGetKey();

        list ls = (list)k;  // no E20021 in mono
        ls = ls + (list)s;  // no E20021 in mono

        if (llGetListEntryType(ls, 0) != TYPE_STRING
         && llGetListEntryType(ls, 1) != TYPE_KEY)
        {
            // Printed in Mono because it's safe
            llOwnerSay("Mono is safe");
        }
    }
}
