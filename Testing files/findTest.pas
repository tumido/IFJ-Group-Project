var i:integer;
    s:string;
    c:string;
begin
  readln(s);
  readln(c);
  i := find(s,c);
  write('Podretezec nalezen na pozici: ', i, ''#10'')
end.
