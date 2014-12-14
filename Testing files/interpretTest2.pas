var i:integer;
    b: boolean;
    s:string;

function a(k:integer): integer;
var x:string;
    i:integer;
begin
  if k > 0 then begin
  k := k - 1;
  a := a(k)
  end
  else
  begin
  a := 0
  end
end;

begin
  i := a(5);
  write('Hodonta i je: ', i, ''#10'')
end.
