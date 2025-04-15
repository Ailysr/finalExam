#include <iostream>
#include <valarray>
#include <vector>
#include <fstream>
#include <cmath>
#include "ODE.h"
#include "OdeSolver.h"

using namespace std;

void writeToFile(ofstream& outfile, double time, const valarray<double>& x ,double y) {
    outfile << time << ",";
	size_t n = x.size();
    for (size_t i = 0; i < n; i++) {
        outfile << x[i] ;
		if (i < n - 1) {
			outfile << ",";
		}
    }

    outfile << "," << y << "\n";
}

void showABCD(TransferFunction ode) {
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

double u(double t) {
    // 根据需要定义输入信号 u(t)
	
    return (t);
}

int main() {
    // 定义状态空间矩阵
    valarray<valarray<double>> A = { valarray<double>{-0.02, -0.1, 0, -9.8},
                                     valarray<double>{-0.001, -0.4, 1, 0},
                                     valarray<double>{0.0, -1.7,  -0.3, 0}, 
                                     valarray<double>{0, 0, 1, 0 }
    };
    valarray<double> B = { 0, 0, -2.6, 0};
    valarray<double> C = { 1, 0, 0, 0 };
    double D = 0;
    TransferFunction ode(A,B,C,D);
    ode.setInputFunction(u);  // 设置输入信号
    valarray<double> x = { 0, 0, 0, 0};  // 初始状态
	double y = 0;  // 初始输出

	RungeKuttaSolver solver(4);  //选择微分方程求解器：RungeKuttaSolver/AdamsSolver
    
    double dt = 0.1;  // 时间步长

    int steps = 100;  // 仿真100步

    // 创建输出文件
    ofstream outfile("D:/Research/课程/研一/飞行仿真技术/homework/Transfer_RK4.csv");
    if (!outfile.is_open()) {
        cerr << "Error opening file for writing" << endl;
        return 1;
    }
    //写入文件头
    outfile << "Time,State\n";
    writeToFile(outfile, 0, x, y);
    for (int i = 1; i <= steps; ++i) {
        solver.integrate(&ode, dt, x);
		y = ode.output(x, i * dt);  // 计算输出
        cout << "Step " << i << ": ";
        for (double xi : x) {
            cout << xi << " ";
        }
		cout <<  y << endl;
        cout << "\n" << endl;

        // 写入文件
        writeToFile(outfile, i * dt, x, y);
    }
    // 关闭文件
    outfile.close();
    showABCD(ode);
    return 0;
}
