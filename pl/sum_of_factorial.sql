declare
  num1 number := &start_num;
  num2 number := &end_num;
  fact_sum number :=0;
  fact number;
  
  function factorial(num number) return number is
    fact number :=1;
    i int;
  begin
    if(num<=0) then
      return 1;
    end if;
    for i in 2..num loop
      fact:=fact*i;
    end loop;
    return fact;
  end factorial;
  
begin
  dbms_output.put_line(chr(10));
  for i in num1..num2 loop
    fact:=factorial(i);
    dbms_output.put_line('Factorial of ' || i || ' : ' || fact);
    fact_sum := fact_sum + fact;
  end loop;
  dbms_output.put_line('Sum of factorial: ' || fact_sum);
end;
/
