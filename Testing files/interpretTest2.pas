var i:integer;
    b: boolean;

function a(k:integer): integer;
var x:string;
    i:integer;
begin
  write('Jsem ve funkci, a mam parametr s hodnotou ', k, '. Zadej string: ');
  readln(x);
  write(''#10'Zadal jsi: ', x, ''#10'Vytvarim lokalni i s hodnotou 26, globalni ma hodnotu 11'#10'');
  i := 26;
  a := 11
end;

begin
  i := a(5);
  write('Hodonta i je: ', i, ''#10'')
end.
