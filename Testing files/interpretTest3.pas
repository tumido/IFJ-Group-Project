var s:string;
    b:boolean;
    i:real;
begin
  b := true;
  i := 113242.123;
  write('ach jo, i bz melo mit hodnotu 113242, je to tak? i = ', i);
  while b do
  begin
    b := false;
    write('prirazeni vstupni podminky');
    write('jsem ve whilu')
  end;
  i := 3 * 2.0;
  write(''#10'test scitani: i := 1 + 1; i =', i, ''#10'');
  write('konec while'#10'')
end.
