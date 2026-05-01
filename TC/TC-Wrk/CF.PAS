Program chk_flg(input);
uses DOS;
type str255 = string[255];
var  inf : text;
     ofl : text;
     j,k: integer;
     a_str,b_str:str255;
begin
     readln(input,a_str);
{     writeln(a_str,'*',a_str[length(a_str)-1],'*',
                       a_str[length(a_str)],'*');{}
     if (a_str[length(a_str)-1]=' ') and
        (a_str[length(a_str)]='0') then halt(1) else halt(0);
end.