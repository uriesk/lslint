list lazy_list_set(list target, integer index, list element)
{
    return llListReplaceList(target, element, index, index);
}

default{timer(){

    list a;
    a[0] = "";  // implicitly references lazy_list_set so no warning emitted

}}
