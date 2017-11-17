// Check whether order alters outcome
// Check multiple definitions
// Check overriding of a function other than lazy_list_set

x()
{
    list a;
    a[0] = "";
}

list lazy_list_set(list target, integer index, list element)
{
    return llListReplaceList(target, element, index, index);
}

list lazy_list_set(list target, integer index, list element)
{
    return llListReplaceList(target, element, index, index);
}

x(){}  // $[E10001] Duplicate identifier

default{timer(){

    x();

}}
