#include <iostream>
#include <valarray>
#include <vector>
#include <fstream>
#include <cmath>
#include "ODE.h"
#include "OdeSolver.h"

using namespace std;

static void writeToFile(ofstream& outfile, double time, const valarray<double>& x, double y) {
    outfile << time << ",";
    size_t n = x.size();
    for (size_t i = 0; i < n; i++) {
        outfile << x[i];
        if (i < n - 1) {
            outfile << ",";
        }
    }

    outfile << "," << y << "\n";
}

static void showABCD(TransferFunction ode) {
    // 获取并打印矩阵 A
    const valarray<valarray<double>>& A = ode.getA();
    cout << "Matrix A:" << endl;
    for (const auto& row : A) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    // 获取并打印矩阵 B
    const valarray<double>& B = ode.getB();
    cout << "Matrix B:" << endl;
    for (double val : B) {
        cout << val << "";
    }
    cout << endl;

    // 获取并打印矩阵 C
    const valarray<double>& C = ode.getC();
    cout << "Matrix C:" << endl;
    for (double val : C) {
        cout << val << " ";
    }
    cout << endl;

    // 获取并打印 D
    double D = ode.getD();
    cout << "D: " << D << endl;
}

static double u(double t) {
    // 根据需要定义输入信号 u(t)

    return (2);
}

int main() {
   // 定义三阶微分方程的系数，例如：a3 * x''' + a2 * x'' + a1 * x' + a0 * x = 0
   valarray<double> coefficients = { 1.0 ,0.7 ,1 }; // 示例系数

   // 创建 SimpleODE 对象
   SimpleODE simpleOde(coefficients);

   // 设置输入信号 u(t)
   simpleOde.setInputFunction(u);

   // 定义初始状态
   valarray<double> x = { 0.0 ,0.0}; // 初始状态向量
   double y = x[0]; // 初始输出

   // 选择微分方程求解器
   RungeKuttaSolver solver(4); // 修复：去掉括号，创建对象而不是函数指针
   //EulerSolver solver;
   double dt = 0.05; // 时间步长
   int steps = 200; // 仿真 100 步

   // 创建输出文件
   ofstream outfile("D:\\Research\\课程\\研一\\飞行仿真技术\\homework\\SimpleODE_RK4.csv");
   if (!outfile.is_open()) {
       cerr << "Error opening file for writing" << endl;
       return 1;
   }

   // 写入文件头
   outfile << "Time,State,Output\n";
   writeToFile(outfile, 0, x, y);

   // 仿真循环
   for (int i = 1; i <= steps; ++i) {
       solver.integrate(&simpleOde, dt, x); // 使用求解器计算下一步状态
       y = simpleOde.output(x, i * dt);    // 计算输出
       cout << "Step " << i << ": ";
       for (double xi : x) {
           cout << xi << " ";
       }
       cout << y << endl;

       // 写入文件
       writeToFile(outfile, i * dt, x, y);
   }

   // 关闭文件
   outfile.close();

   return 0;
}