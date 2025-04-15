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

static double u(double t) {
    // ������Ҫ���������ź� u(t)

    return (2);
}

int main() {
   // ��������΢�ַ��̵�ϵ�������磺a3 * x''' + a2 * x'' + a1 * x' + a0 * x = 0
   valarray<double> coefficients = { 1.0 ,0.7 ,1 }; // ʾ��ϵ��

   // ���� SimpleODE ����
   SimpleODE simpleOde(coefficients);

   // ���������ź� u(t)
   simpleOde.setInputFunction(u);

   // �����ʼ״̬
   valarray<double> x = { 0.0 ,0.0}; // ��ʼ״̬����
   double y = x[0]; // ��ʼ���

   // ѡ��΢�ַ��������
   RungeKuttaSolver solver(4); // �޸���ȥ�����ţ�������������Ǻ���ָ��
   //EulerSolver solver;
   double dt = 0.05; // ʱ�䲽��
   int steps = 200; // ���� 100 ��

   // ��������ļ�
   ofstream outfile("D:\\Research\\�γ�\\��һ\\���з��漼��\\homework\\SimpleODE_RK4.csv");
   if (!outfile.is_open()) {
       cerr << "Error opening file for writing" << endl;
       return 1;
   }

   // д���ļ�ͷ
   outfile << "Time,State,Output\n";
   writeToFile(outfile, 0, x, y);

   // ����ѭ��
   for (int i = 1; i <= steps; ++i) {
       solver.integrate(&simpleOde, dt, x); // ʹ�������������һ��״̬
       y = simpleOde.output(x, i * dt);    // �������
       cout << "Step " << i << ": ";
       for (double xi : x) {
           cout << xi << " ";
       }
       cout << y << endl;

       // д���ļ�
       writeToFile(outfile, i * dt, x, y);
   }

   // �ر��ļ�
   outfile.close();

   return 0;
}