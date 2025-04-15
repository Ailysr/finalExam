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
    // ��ȡ����ӡ���� A
    const valarray<valarray<double>>& A = ode.getA();
    cout << "Matrix A:" << endl;
    for (const auto& row : A) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    // ��ȡ����ӡ���� B
    const valarray<double>& B = ode.getB();
    cout << "Matrix B:" << endl;
    for (double val : B) {
        cout << val << "";
    }
    cout << endl;

    // ��ȡ����ӡ���� C
    const valarray<double>& C = ode.getC();
    cout << "Matrix C:" << endl;
    for (double val : C) {
        cout << val << " ";
    }
    cout << endl;

    // ��ȡ����ӡ D
    double D = ode.getD();
    cout << "D: " << D << endl;
}

double u(double t) {
    // ������Ҫ���������ź� u(t)
	
    return (t);
}

int main() {
    // ����״̬�ռ����
    valarray<valarray<double>> A = { valarray<double>{-0.02, -0.1, 0, -9.8},
                                     valarray<double>{-0.001, -0.4, 1, 0},
                                     valarray<double>{0.0, -1.7,  -0.3, 0}, 
                                     valarray<double>{0, 0, 1, 0 }
    };
    valarray<double> B = { 0, 0, -2.6, 0};
    valarray<double> C = { 1, 0, 0, 0 };
    double D = 0;
    TransferFunction ode(A,B,C,D);
    ode.setInputFunction(u);  // ���������ź�
    valarray<double> x = { 0, 0, 0, 0};  // ��ʼ״̬
	double y = 0;  // ��ʼ���

	RungeKuttaSolver solver(4);  //ѡ��΢�ַ����������RungeKuttaSolver/AdamsSolver
    
    double dt = 0.1;  // ʱ�䲽��

    int steps = 100;  // ����100��

    // ��������ļ�
    ofstream outfile("D:/Research/�γ�/��һ/���з��漼��/homework/Transfer_RK4.csv");
    if (!outfile.is_open()) {
        cerr << "Error opening file for writing" << endl;
        return 1;
    }
    //д���ļ�ͷ
    outfile << "Time,State\n";
    writeToFile(outfile, 0, x, y);
    for (int i = 1; i <= steps; ++i) {
        solver.integrate(&ode, dt, x);
		y = ode.output(x, i * dt);  // �������
        cout << "Step " << i << ": ";
        for (double xi : x) {
            cout << xi << " ";
        }
		cout <<  y << endl;
        cout << "\n" << endl;

        // д���ļ�
        writeToFile(outfile, i * dt, x, y);
    }
    // �ر��ļ�
    outfile.close();
    showABCD(ode);
    return 0;
}
