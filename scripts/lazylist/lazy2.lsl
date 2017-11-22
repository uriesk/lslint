list
     lazy_list_set  // $[E20009] declared but never used
                  (list target, integer index, list element)
{
    return llListReplaceList(target, element, index, index);
}

default{timer(){}}