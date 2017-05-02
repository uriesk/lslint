override(integer x)
{
  x;
}


myoverride()
{
    override(3); // $[E10012] too many arguments (only the one below counts)
}

override()
{
}

llSay // $[E10031] is a library function
(integer a, string b){a;b;}

default{timer(){myoverride();}}
