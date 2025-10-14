DROP TABLE Person CASCADE CONSTRAINTS;

CREATE TABLE Person (
    pid NUMBER PRIMARY KEY,
    pname VARCHAR2(50),
    DOB DATE
);

INSERT INTO Person VALUES (1, 'Ravi', TO_DATE('15-06-2001', 'DD-MM-YYYY'));
INSERT INTO Person VALUES (2, 'Meena', TO_DATE('23-03-1999', 'DD-MM-YYYY'));
INSERT INTO Person VALUES (3, 'Ajay', TO_DATE('10-12-1985', 'DD-MM-YYYY'));

COMMIT;


SET SERVEROUTPUT ON;

DECLARE
    CURSOR person_cur IS
        SELECT pid, pname, DOB FROM Person;

    v_pid Person.pid%TYPE;
    v_name Person.pname%TYPE;
    v_dob Person.DOB%TYPE;
    v_age NUMBER;
BEGIN
    OPEN person_cur;
    LOOP
        FETCH person_cur INTO v_pid, v_name, v_dob;
        EXIT WHEN person_cur%NOTFOUND;

        v_age := TRUNC(MONTHS_BETWEEN(SYSDATE, v_dob) / 12);

        DBMS_OUTPUT.PUT_LINE('PID: ' || v_pid || ' Name: ' || v_name || ' Age: ' || v_age);
    END LOOP;
    CLOSE person_cur;
END;
/





DROP TABLE Employee CASCADE CONSTRAINTS;

CREATE TABLE Employee (
    empid NUMBER PRIMARY KEY,
    empname VARCHAR2(50),
    joining_date DATE,
    relieving_date DATE,
    salary NUMBER
);

INSERT INTO Employee VALUES (101, 'Anita', TO_DATE('01-01-2010', 'DD-MM-YYYY'), TO_DATE('31-12-2020', 'DD-MM-YYYY'), 40000);
INSERT INTO Employee VALUES (102, 'Binu', TO_DATE('15-06-2015', 'DD-MM-YYYY'), TO_DATE('30-06-2021', 'DD-MM-YYYY'), 35000);
INSERT INTO Employee VALUES (103, 'Cyril', TO_DATE('10-10-2018', 'DD-MM-YYYY'), NULL, 30000); -- still working

COMMIT;


DECLARE
    CURSOR emp_cur IS
        SELECT empid, empname, joining_date, relieving_date
        FROM Employee
        WHERE relieving_date IS NOT NULL;

    v_id Employee.empid%TYPE;
    v_name Employee.empname%TYPE;
    v_join Employee.joining_date%TYPE;
    v_rel Employee.relieving_date%TYPE;
    v_years NUMBER;
BEGIN
    OPEN emp_cur;
    LOOP
        FETCH emp_cur INTO v_id, v_name, v_join, v_rel;
        EXIT WHEN emp_cur%NOTFOUND;

        v_years := TRUNC(MONTHS_BETWEEN(v_rel, v_join) / 12);

        DBMS_OUTPUT.PUT_LINE('EmpID: ' || v_id || ' Name: ' || v_name || ' Service: ' || v_years || ' years');
    END LOOP;
    CLOSE emp_cur;
END;
/



DECLARE
    CURSOR pension_cur IS
        SELECT empid, empname, joining_date, relieving_date, salary
        FROM Employee
        WHERE relieving_date IS NOT NULL;

    v_id Employee.empid%TYPE;
    v_name Employee.empname%TYPE;
    v_join Employee.joining_date%TYPE;
    v_rel Employee.relieving_date%TYPE;
    v_sal Employee.salary%TYPE;
    v_years NUMBER;
    v_pension NUMBER;
BEGIN
    OPEN pension_cur;
    LOOP
        FETCH pension_cur INTO v_id, v_name, v_join, v_rel, v_sal;
        EXIT WHEN pension_cur%NOTFOUND;

        v_years := TRUNC(MONTHS_BETWEEN(v_rel, v_join) / 12);
        v_pension := (v_years * v_sal) / 100;

        DBMS_OUTPUT.PUT_LINE('EmpID: ' || v_id || ' Name: ' || v_name || ' Pension: ' || v_pension);
    END LOOP;
    CLOSE pension_cur;
END;
/



DROP TABLE EMPLOYEE2 CASCADE CONSTRAINTS;

CREATE TABLE EMPLOYEE2 (
    empid NUMBER PRIMARY KEY,
    empname VARCHAR2(50),
    designation VARCHAR2(50),
    dept VARCHAR2(50),
    salary NUMBER
);

INSERT INTO EMPLOYEE2 VALUES (201, 'Deepa', 'Manager', 'HR', 55000);
INSERT INTO EMPLOYEE2 VALUES (202, 'Eby', 'Clerk', 'Accounts', 25000);
COMMIT;


CREATE OR REPLACE TRIGGER emp_count_trigger
AFTER INSERT OR DELETE OR UPDATE
ON EMPLOYEE2
DECLARE
    v_count NUMBER;
BEGIN
    SELECT COUNT(*) INTO v_count FROM EMPLOYEE2;
    DBMS_OUTPUT.PUT_LINE('Total number of tuples in EMPLOYEE2: ' || v_count);
END;
/




INSERT INTO EMPLOYEE2 VALUES (203, 'Fayiz', 'Developer', 'IT', 40000);
UPDATE EMPLOYEE2 SET salary = 60000 WHERE empid = 201;
DELETE FROM EMPLOYEE2 WHERE empid = 202;




DROP TABLE EMPLOYEE3 CASCADE CONSTRAINTS;
DROP TABLE INCREMENT CASCADE CONSTRAINTS;

CREATE TABLE EMPLOYEE3 (
    empid NUMBER PRIMARY KEY,
    empname VARCHAR2(50),
    salary NUMBER
);

CREATE TABLE INCREMENT (
    empid NUMBER,
    incr NUMBER
);

INSERT INTO EMPLOYEE3 VALUES (301, 'Gokul', 20000);
INSERT INTO EMPLOYEE3 VALUES (302, 'Hari', 30000);
COMMIT;





CREATE OR REPLACE TRIGGER salary_increment_trigger
AFTER UPDATE OF salary ON EMPLOYEE3
FOR EACH ROW
DECLARE
    v_diff NUMBER;
BEGIN
    v_diff := :NEW.salary - :OLD.salary;

    IF v_diff > 1000 THEN
        INSERT INTO INCREMENT(empid, incr)
        VALUES (:OLD.empid, v_diff);

        DBMS_OUTPUT.PUT_LINE('Increment record inserted for EmpID: ' || :OLD.empid || ', Increment: ' || v_diff);
    END IF;
END;
/





UPDATE EMPLOYEE3 SET salary = 22000 WHERE empid = 301; -- diff = 2000 → should insert
UPDATE EMPLOYEE3 SET salary = 30500 WHERE empid = 302; -- diff = 500 → no insert

SELECT * FROM INCREMENT;