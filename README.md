# 使用说明
`master`分支为成熟版，`dev`分支仍在开发调试中
## 程序说明
本项目是为了完成NWPU课程飞行仿真技术课程的作业。题目是：`使用面向对象技术，设计一个支持阿当姆斯（Adams）算法和龙格库塔（Runge-Kutta）算法（至少支持4阶阿当姆斯和龙格库塔）的ODE求解器，使用C++语言实现`
## 大体实现思路
通过面向对象，将数值求解器与求解问题分开实现。
定义了基类`ODE`和`ODESolver`，利用继承，多态的思想，实现`ODE`的子类`SimpleODE`，`TransferFunction`，以及`ODESolver`的子类`AdamsSolver`，`RungeKuttaSolver`，`EulerSolver`。
## 相关结果

## 使用流程
在主函数中，定义相关问题，进行仿真求解，数据会自动保存。

**问题定义模板**
---
*****
