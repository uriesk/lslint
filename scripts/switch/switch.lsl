default
{
    state_entry()
    {
        integer i = 5;
        switch (
                i     // $[E20016] Constant switch expression
                 )
        {
            case i * 0 + 1:
                llOwnerSay("i is 1");
                break;
            case 2 // unlike C, colon is not mandatory if a {} block follows
                {
                    llOwnerSay("i is 2");
                    break;
                }
            case
                 2     // $[E20018] Duplicate case label
                  :
            case
                 ""    // $[E10035] Incompatible types
                   :
                break;

            default:
                llOwnerSay("i isn't 1 or 2");
                switch        // $[E20017] no default
                       (
                        i+1   // $[E20016]
                           )
                {
                    case 6:
                        llOwnerSay("i is 5");
                        break;
                }
            default  // $[E10034] multiple defaults
                   {}
        }
        break        // $[E10033] break must be within a switch statement
             ;

        list j = llGetPhysicsMaterial();
        switch (
                j     // $[E20010]
                 ) 
        {
           case []: ;
           case j:  ;
           case 3:  ; // $[E10035]
           case "": ; // $[E10035]
           default: ;
        }
    }
}
