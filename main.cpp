#include "parseXML.h"
#include <iostream>
#include <valarray>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <stdexcept>
#include <memory>
#include "ODE.h"
#include "OdeSolver.h"

using namespace std;

static double u(double t)
{
	return sin(t); // 输入函数 u(t)
};

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

int main(int argc, char* argv[]) {
    // 定义变量
    string selectedProblemType;
    vector<double> coefficients;
    vector<double> numerator;
    vector<double> denominator;
    vector<vector<double>> A;
    vector<double> B;
    vector<double> C;
    double D = 0.0;
    vector<double> initialState;
    double dt = 0.05;
    int steps = 200;
    int solverOrder = 4;  //默认值
    string solverType = "RungeKutta";
    string resultPath = "SimulationResults.csv";

    // 解析命令行参数
    string xmlFile;
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-x" && i + 1 < argc) {
            xmlFile = argv[++i];
        }
    }

    if (xmlFile.empty()) {
        cerr << "No XML configuration file provided. Use -x <file> to specify the XML file." << endl;
        return 1;
    }

    try {
        // 调用 parseXML 函数解析 XML 文件
        parseXML(xmlFile, selectedProblemType, coefficients, numerator, denominator, A, B, C, D, initialState, dt, steps, solverOrder, solverType, resultPath);
    }
    catch (const exception& e) {
        cerr << "Error parsing XML: " << e.what() << endl;
        return 1;
    }

    // 根据 selectedProblemType 创建对应的 ODE 对象
    unique_ptr<ODE> ode;
    if (selectedProblemType == "SimpleODE") {
        //cout << "Coefficients size: " << coefficients.size() << endl;
        //cout << "Initial state size: " << initialState.size() << endl;
        if (coefficients.empty()) {
            cerr << "Error: Missing coefficients for SimpleODE." << endl;
            return 1;
        }
        ode = make_unique<SimpleODE>(valarray<double>(coefficients.data(), coefficients.size()));
    }
    else if (selectedProblemType == "TransferFunction") {
        if (numerator.empty() || denominator.empty()) {
            cerr << "Error: Missing numerator or denominator for TransferFunction." << endl;
            return 1;
        }
        ode = make_unique<TransferFunction>(valarray<double>(numerator.data(), numerator.size()), valarray<double>(denominator.data(), denominator.size()));
        cout << "*****************************" << endl;
        // 获取并打印 A 矩阵
        const auto& A = dynamic_cast<TransferFunction*>(ode.get())->getA();
        const auto& B = dynamic_cast<TransferFunction*>(ode.get())->getB();
        const auto& C = dynamic_cast<TransferFunction*>(ode.get())->getC();
        const auto& D = dynamic_cast<TransferFunction*>(ode.get())->getD();
        cout << "Matrix A:" << endl;
        for (const auto& row : A) {
            for (const auto& value : row) {
                cout << value << " ";
            }
            cout << endl;
        }
        cout << "Matrix B:" << endl;
        for (const auto& value : B) {
            cout << value << " ";
        }
        cout << endl;
        cout << "Matrix C:" << endl;
        for (const auto& value : C) {
            cout << value << " ";
        }
        cout << endl;
        cout << "Matrix D:" << endl;
        cout << D << endl;
        cout << "*****************************" << endl;
    }
    else if (selectedProblemType == "StateSpace") {
        if (A.empty() || B.empty() || C.empty()) {
            cerr << "Error: Missing state-space matrices for StateSpace." << endl;
            return 1;
        }
        valarray<valarray<double>> stateSpaceA(A.size());
        for (size_t i = 0; i < A.size(); ++i) {
            stateSpaceA[i] = valarray<double>(A[i].data(), A[i].size());
        }
        ode = make_unique<TransferFunction>(stateSpaceA, valarray<double>(B.data(), B.size()), valarray<double>(C.data(), C.size()), D);
    }
    else {
        cerr << "Error: Unsupported problem type: " << selectedProblemType << endl;
        return 1;
    }

    // 设置输入函数 u(t)
    ode->setInputFunction(u);

    // 选择求解器
    unique_ptr<OdeSolver> solver;
    if (solverType == "Adams") {
        solver = make_unique<AdamsSolver>(solverOrder);
    }
    else if (solverType == "RungeKutta") {
        solver = make_unique<RungeKuttaSolver>(solverOrder);
    }
    else if (solverType == "Euler") {
        solver = make_unique<EulerSolver>();
    }
    else {
        cerr << "Unsupported solver type: " << solverType << endl;
        return 1;
    }

    // 打开输出文件
    ofstream outfile(resultPath);
    if (!outfile.is_open()) {
        cerr << "Error opening file for writing: " << resultPath << endl;
        return 1;
    }

    // 写入文件头
    outfile << "Time,State";
    int order = ode->getOrder();
	for (int i = 0; i < order; ++i) {
		outfile << ",";
	}
	outfile << "Output\n";
    writeToFile(outfile, 0, valarray<double>(initialState.data(), initialState.size()), initialState[0]);

    // 仿真循环
    valarray<double> x(initialState.data(), initialState.size());
    for (int i = 1; i <= steps; ++i) {
        solver->integrate(ode.get(), dt, x);
        double y = ode->output(x, i * dt);
        writeToFile(outfile, i * dt, x, y);
    }

    outfile.close();
    cout << "Simulation completed. Results saved to: " << resultPath << endl;
    return 0;
}
