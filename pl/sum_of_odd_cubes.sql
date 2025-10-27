declare
  num1 number := &start_num;
  num2 number := &end_num;
  cube_sum number := 0;
  num_cube number;
begin
  dbms_output.put_line(chr(10));
  for i in num1..num2 loop
    if mod(i,2)!=0 then
      select power(i,3) into num_cube from dual;
      dbms_output.put_line('Cube of ' || i || ' : ' || num_cube);
      cube_sum := num_cube + cube_sum;
    end if;
  end loop;
  dbms_output.put_line('Sum of odd cubes : ' || cube_sum);
end;
/
