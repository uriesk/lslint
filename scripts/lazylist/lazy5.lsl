x(){
  list a;
  a[0] = "";  // $[E10011] Passing integer as argument 2 (invalid arg list)
}

list lazy_list_set(list target, list element, integer index)
{
    return llListReplaceList(target, element, index, index);
}

default{timer(){

    list a;
    a[0] = "";  // $[E10011] 
    x();

}}
