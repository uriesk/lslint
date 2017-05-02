list lazy_list_set(list target, integer idx, list value)
{
    while (llGetListLength(target) < idx)
        target += ""; // fill with blanks
    return llListReplaceList(target, value, idx, idx);
}

default{timer(){

    list L;

    L[3] = 0;

}}
