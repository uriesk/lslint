default
{
    state_entry()
    {
        integer i = 5;
        switch (i)
        {
            case i * 0 + 1:
                llOwnerSay("i is 1");
                break;
            case 2 // unlike C, colon is not mandatory if a {} block follows
                {
                    llOwnerSay("i is 2");
                    break;
                }
            case 2:
            case 3:
                break;

            default:
                llOwnerSay("i isn't 1 or 2");
                switch (i+1)
                {
                    case 6:
                        llOwnerSay("i is 5");
                        break;
                }
            default {}
        }
    }
}
