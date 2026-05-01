PROGRAM TAIL(INPUT,OUTPUT);
uses  DOS;
VAR
a_Str, b_str:String[255];
jj,kk,mm,nn,pp:integer;
whr:String[255];
BEGIN

      kk:=0;
     WHILE NOT(EOF(INPUT))
{     and (kk<mm) {r}
     DO BEGIN
         mm:=1; nn:=0; pp:=0;
         READLN(INPUT,A_Str);
         FOR JJ:=1 TO length(a_str) DO begin
           if (a_str[jj]=' ') then nn:=nn+1;
           if ((a_str[jj]<>' ') or (nn<3))
             and (a_str[jj]>=' ') and (a_str[jj]<>',')
            then begin
              b_str[mm]:=a_str[jj];
              if (a_str[jj]<>' ') then nn:=1;
              if (a_str[jj]='.') then pp:=1;
              if (a_str[jj]=' ') and (a_str[jj+1]=' ')
                 then b_str[mm]:=',';
              mm:=mm+1;
             end;
          end;
         kk:=kk+1;
         b_str[0]:=char(mm-1);
{         writeln(output,a_str,'':5,length(a_str),'':5,nn);{}
         if (nn>0) and (pp>0) then writeln(output,b_str);
       END;
END.
