# Blsimu
Simulation on laser electron photons in synchrotron radiation sources


---
##Building this package
1.    This package uses the Python3 interpreter on your computer.
To build the package, you need to know compiler and linker options for the Python3. When the version of your Python3 is 3.X (X is a digit number),  they can be obtained by issuing

        % python3.X-config --cflags

  for the compiler option <CPYFLAGS> and

        % python3.X-config --ldflags

  for the linker option <LDPYFLAGS>. 
If you don't know the version number 3.X, find it by

        % python3 --version

  Then edit CMakeLists.txt in this directory (where this README
is involved). Near lines 15 to 23, parameters CPYFLAGS and LDPYFLAGS
are defined. You should set your <CPYFLAGS> and <LDPYFLAGS> to
corresponding parameters. Notice that <LDPYFLAGS> should be
quoted to be set.



2.     Building and example running
  You are reading this README incuded in the source directory
of the package. Call the path to this directory in your computer
<Package_SRCDIR>. Ways of the building and running examples are 
shown below. 

  __(Recommended: Build out-of-source)__

  Define a directory <WorkDir> somewhere OUT OF <Package_SRCDIR>.

        %cp <Package_SRCDIR>/build/* <WorkDir>
        %cd <WorkDir>
        %build <Package_SRCDIR>
        %cp <WorkDir>/examples/?/useBlsimu/*.doprm <WorkDir>/useBlsimu/
        %run


  __(NOT recommended: Build in-source)__

  You will get an executable file <Package_SRCDIR>/exe as the output of below

        %cd <Package_SRCDIR>/Witpy
        %make lib
        %cd <Package_SRCDIR>
        %make lib
        %make

3.   Bug reports

  For a bug report, please send email to schin@spring8.or.jp.

---
##Using this package

When you run the package in the directory <WorkDir>, an output
file blsimu.out is created in <WorkDir>/useBlsimu.

###Content of the output
Data format: 
A typical line of an ASCII data file looks like 

9.698450e+02 1.899630e-04 4.790170e-06 9.698450e+02 7.998145e+03 -1.023006e-01 6.962928e-03 7.998145e+03 9.768953e+02 -2.385339e-02 6.450044e-05 9.768953e+02 6.469310e+03 -1.098259e+02 2.979221e-02 6.468378e+03 1.528835e+03 -8.072750e-02 -6.377921e-02 1.528835e+03

A line is thus composed of 20 real variables. 

*Num  Name  Explanation*

*      1     * X0S       incident electron position s
*      2     * X0X       incident electron position x
*      3     * X0Y       incident electron position y
*      4     * X0Z       incident electron position z
*      5     * P0E       incident electron energy
*      6     * P0X       incident electron momentum x
*      7     * P0Y       incident electron momentum y
*      8     * P0Z       incident electron momentum z
*      9     * X1S       secondary electron position s
*     10     * X1X       secondary electron position x
*     11     * X1Y       secondary electron position y
*     12     * X1Z       secondary electron position z
*     13     * P1E       secondary electron energy
*     14     * P1X       secondary electron momentum x
*     15     * P1Y       secondary electron momentum y
*     16     * P1Z       secondary electron momentum z
*     17     * PGE       gamma energy
*     18     * PGX       gamma momentum x
*     19     * PGY       gamma momentum y
*     20     * PGZ       gamma momentum z
********************************************************

* s is the length along accelerator ring from a fixed reference point
* x directs toward outside the ring, y to downward and z to the beam direction. 


