var a:string;
    i:integer;
    b:boolean;
    s:string;


begin
  i := 1 + 2;
  write(i, ''#10'');
  i := 1;
  write(i, ''#10'');
  write('Zadejte string'#10'');
  readln(a);
  write('Zadal jsi: ',a, ''#10'', 'Zkopiruju prvnich 5 pismen a vypisu je: ');
  s := copy(a, 0, 5);
  write(s, ''#10'Zadejte int'#10'');
  readln(i);
  write('Zadal jsi: ', i, ''#10'');
  readln(b)
end.
