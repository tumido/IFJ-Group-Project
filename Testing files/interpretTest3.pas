VAR s:string;
    b:integer;
    i:real;
begin
  b := 5;
  i := 113242.123;
  write('ach jo, i bz melo mit hodnotu 113242, je to tak? i = ', i);
  while b > 0 do
  begin
    b := b - 1;
    write('prirazeni vstupni podminky');
    write('jsem ve whilu b = ', b, ''#10'')
  end;
  i := 3 * 2.0;
  {  s := 'asdsad' + 'vvvv'; }
  write(''#10'test scitani: i := 1 + 1; i =', i, ''#10'', true);
  write('konec while'#10'', s, ''#10'')
end.
