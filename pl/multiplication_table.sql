declare
  num number := &start_num;
begin
  dbms_output.put_line(chr(10));
  for i in 1..10 loop
    dbms_output.put_line(num || 'x' || i || '=' || num*i);
  end loop;
end;
/
