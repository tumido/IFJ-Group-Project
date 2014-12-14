var i:integer;
    b: boolean;
    s:string;

function a(k:integer): integer;
var x:string;
    i:integer;
begin
  write('Jsem ve funkci, a mam parametr s hodnotou ', k, '. Zadej string: ');
  readln(x);
  write(''#10'Zadal jsi: ', x, ''#10'Vytvarim lokalni i s hodnotou 26, globalni ma hodnotu k = ', k,  ''#10'');
  a := 10;
  write('Hodnota a je:', a, ''#10'')
end;

begin
  readln(s);
  i := a(5);
  write('Hodonta i je: ', i, ''#10'');
  write('s =', s, ''#10'')
end.
