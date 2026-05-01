Program xfer(input, output);
type str255 = string[255];
var  inf : text;
     ofl : text;
     j,k,m: integer;
     a_str,b_str:str255;
     sme: boolean;
begin
     readln(input,b_str);
     a_str:=b_str;
     j:=1;
     readln(input,b_str);
     while not(eof(input)) do begin
        k:=length(b_str);
        sme:=true;
        for m:=75 to k do if (a_str[m]<>b_str[m]) then sme:=false;
        if (sme) then j:=j+1
           else begin
              writeln(output,j:5,chr(9),a_str);
              a_str:=b_str;
              j:=1;
            end;
         readln(input,b_str);
       end;
       writeln(output,j:5,chr(9),a_str);
end.