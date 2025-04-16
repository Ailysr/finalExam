# 使用说明
## 程序说明
本项目是为了完成NWPU课程飞行仿真技术课程的作业。题目是：`使用面向对象技术，设计一个支持阿当姆斯（Adams）算法和龙格库塔（Runge-Kutta）算法（至少支持4阶阿当姆斯和龙格库塔）的ODE求解器，使用C++语言实现`
## 大体实现思路
通过面向对象，将数值求解器与求解问题分开实现。
定义了基类`ODE`和`ODESolver`，利用继承，多态的思想，实现`ODE`的子类`SimpleODE`，`TransferFunction`，以及`ODESolver`的子类`AdamsSolver`，`RungeKuttaSolver`，`EulerSolver`。
定义了配置文件*config.xml*，在其中可以自由定义求解问题和求解器的配置。
通过生成解决方案，使用命令行方式调用*finalExam.exe -x config.xml*，一键式完成配置文件的读入，问题的求解，以及仿真结果的保存。
## 相关结果

## 使用流程
