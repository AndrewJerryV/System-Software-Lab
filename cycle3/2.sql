SET SERVEROUTPUT ON;

DECLARE
  CURSOR cur_relieved IS
    SELECT empid, empname, joining_date, relieving_date, salary
    FROM Employee
    WHERE relieving_date IS NOT NULL;

  v_empid    Employee.empid%TYPE;
  v_empname  Employee.empname%TYPE;
  v_join     Employee.joining_date%TYPE;
  v_relieve  Employee.relieving_date%TYPE;
  v_sal      Employee.salary%TYPE;
  v_service  NUMBER;
  v_pension  NUMBER;

  FUNCTION years_of_service(p_start IN DATE, p_end IN DATE) RETURN NUMBER IS
  BEGIN
    RETURN FLOOR(MONTHS_BETWEEN(p_end, p_start) / 12);
  END;
BEGIN
  OPEN cur_relieved;
  LOOP
    FETCH cur_relieved INTO v_empid, v_empname, v_join, v_relieve, v_sal;
    EXIT WHEN cur_relieved%NOTFOUND;

    v_service := years_of_service(v_join, v_relieve);
    v_pension := (v_service * v_sal) / 100;

    -- Printing output with new lines
    DBMS_OUTPUT.PUT_LINE('Employee ID : ' || v_empid);
    DBMS_OUTPUT.PUT_LINE('Name        : ' || v_empname);
    DBMS_OUTPUT.PUT_LINE('Joining Date: ' || TO_CHAR(v_join,'DD-MON-YYYY'));
    DBMS_OUTPUT.PUT_LINE('Relieving   : ' || TO_CHAR(v_relieve,'DD-MON-YYYY'));
    DBMS_OUTPUT.PUT_LINE('Service     : ' || v_service || ' years');
    DBMS_OUTPUT.PUT_LINE('Salary      : ' || v_sal);
    DBMS_OUTPUT.PUT_LINE('Pension     : ' || v_pension);
    DBMS_OUTPUT.PUT_LINE('-------------------------------------');
  END LOOP;
  CLOSE cur_relieved;
END;
/