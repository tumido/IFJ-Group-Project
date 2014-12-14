var s:string;
    b:boolean;
    i:integer;
begin
  b := true;
  i := 113242;
  write('ach jo, i bz melo mit hodnotu 113242, je to tak? i = ', i);
  while b do
  begin
    b := false;
    write('prirazeni vstupni podminky');
    write('jsem ve whilu')
  end;
  write(''#10'test scitani: i := 1 + 1; i =', i, ''#10'');
  i := 1+1;
  write('konec while'#10'')
end.
