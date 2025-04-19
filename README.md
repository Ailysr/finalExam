This is a Milestone Version, which could set PROLEM to de solved and SOLVER you wanna use.
## Parameters defined in the CONFIG file `config.xml`
**PROBLEM**
- ProblemType:
  - SimpleODE
    - InputFunction
    - InitialState 
    - coefficients
  - TransferFunction
    -  InputFunction
    -  InitialState 
    -  numerator
    -  denominator
  - StateSpace
    -  inputFunction
    -  initialState
    -  A
    -  B
    -  C
    -  D

**SOLVER**
  - AdamsSolver
    - order
    - steps
    - dt
  - RungeKuttaSolver
    - order
    - steps
    - dt
  - EulerSolver
    -  steps
    -  dt

**ReultPath**
The path where you wanna save the simulation data.
## Usage of this Program
1. IDE: **Visual Studio 2022** is the IDE I used so I recommend it to you. 
2. Config description: Based on the template in the original configuration file, modify to describe your specific problem.
3. Run: Run cmd under path `...\ x64 \ Debug` and enter the command `finalExam.exe -x config.xml`

## Result 
1. The command line will display the specific configuration information of your simulation problem.
2. The Simulation result will be save (`csv` format) in your path.

## Contribute to this Program
If there are any problems in operation, you can give a feedback.
If you got any idea, just code it and push it here in the `dev` branch, I really appreciate and expect your contribution.
Let's make this program better!
