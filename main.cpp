#include <iostream>
#include <string>
#include <valarray>
#include "ODE.h"
#include "OdeSolver.h"

using namespace std;

int main(int argc, char* argv[]) {
    // 默认设置
    string solverType = "RungeKutta";  // 默认求解器
    string problemType = "TransferFunction";  // 默认问题类型

    // 解析命令行参数
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--solver" && i + 1 < argc) {
            solverType = argv[++i];
        }
        else if (arg == "--problem" && i + 1 < argc) {
            problemType = argv[++i];
        }
        else {
            cerr << "Unknown argument: " << arg << endl;
            return 1;
        }
    }

    // 根据 problemType 初始化问题
    ODE* ode = nullptr;
    if (problemType == "TransferFunction") {
        valarray<double> numerator = { 1.0, -2.6, -1.092, -0.02077, 0.0 };
        valarray<double> denominator = { 1.0, 0.72, 1.834, 0.0364, 0.001666 };
        ode = new TransferFunction(numerator, denominator);
    }
    else if (problemType == "SimpleODE") {
        ode = new SimpleODE();
    }
    else {
        cerr << "Unknown problem type: " << problemType << endl;
        return 1;
    }

    // 设置输入函数
    ode->setInputFunction([](double t) { return 1.0; });

    // 根据 solverType 初始化求解器
    OdeSolver* solver = nullptr;
    if (solverType == "RungeKutta") {
        solver = new RungeKuttaSolver(4);
    }
    else if (solverType == "Adams") {
        solver = new AdamsSolver(4);
    }
    else {
        cerr << "Unknown solver type: " << solverType << endl;
        return 1;
    }

    // 仿真设置
    valarray<double> x = { 0, 0, 0, 0 };  // 初始状态
    double dt = 0.1;  // 时间步长
    int steps = 100;  // 仿真步数

    // 仿真循环
    for (int i = 0; i < steps; ++i) {
        solver->integrate(ode, dt, x);
        double y = ode.output(x, i * dt);
        cout << "Step " << i << ": State = ";
        for (double xi : x) {
            cout << xi << " ";
        }
        cout << "Output = " << y << endl;
    }//这是dev新版

    // 清理资源
    delete ode;
    delete solver;

    return 0;
}
