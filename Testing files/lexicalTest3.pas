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
  s := 'ahoj svete';
  a := 1;
  write(a);
  if true then
  begin
    a := a;
  write(a)
  end
  else
  begin
    a := a;
   write(b)
  end;
  while true do
  begin
    a := a;
    write(a)
  end;
  a := 1;
  b := 2;
  a := a + b
end.
