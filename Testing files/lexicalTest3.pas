var a: integer;
    b: real;
    o: boolean;
    s: string;

function r(k: integer) : boolean; forward;
function t(k: integer) : boolean;
begin
end;
function f(x: string; y: string; z:string) : integer;
var c: integer;
begin
end;

begin
  readln(a);
  s := copy('asdc', 1, 3);
  readln(o); {muzu nacitat boolean?? }
  write(a, 5, 3.234234324, 8, 'a'#32'sda', 'b', 'c');
  a := 1;
  if a < b then
  begin
    a := b
  end
  else
  begin
    b := a
  end;
  while a <b do
  begin
    a := a
  end;
  a := 1;
  b := 2;
  a := a + b;
end.
