Program chk_flg(input);
uses DOS;
type str255 = string[255];
var  inf : text;
     ofl : text;
     j,k: integer;
     a_str,b_str:str255;
begin
     readln(input,a_str);
{     writeln(a_str,'*',a_str[length(a_str)-1],'*');{}
     if (a_str[length(a_str)-1]='X')
        then halt(2) else halt(1);
end.