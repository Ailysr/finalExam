#include "OdeSolver.h"
#include <stdexcept>
//#include<valarray>
using namespace std;

AdamsSolver::AdamsSolver(int order) : order(order) {
    if (order < 1 || order > 8) {
        throw invalid_argument("Unsupported order, order must be between 1 and 8");
    }
	//显式Adams-Bashforth方法的系数
    coefficients = {
        {1.0},
        {1.5, -0.5},
        {23.0 / 12.0, -4.0 / 3.0, 5.0 / 12.0},
        {55.0 / 24.0, -59.0 / 24.0, 37.0 / 24.0, -3.0 / 8.0},
        {1901.0 / 720.0, -1387.0 / 360.0, 109.0 / 30.0, -637.0 / 360.0, 251.0 / 720.0},
        {4277.0 / 1440.0, -2641.0 / 480.0, 4991.0 / 720.0, -3649.0 / 720.0, 959.0 / 480.0, -95.0 / 288.0},
        {198721.0 / 60480.0, -18637.0 / 2520.0, 235183.0 / 20160.0, -10754.0 / 945.0, 135713.0 / 20160.0, -5603.0 / 2520.0, 19087.0 / 60480.0},
        {16083.0 / 4480.0, -1152169.0 / 120960.0, 242653.0 / 13440.0, -296053.0 / 13440.0, 2102243.0 / 120960.0, -115747.0 / 13440.0, 32863.0 / 13440.0, -5257.0 / 17280.0}
    };
}

void AdamsSolver::integrate(ODE* ode, double dt, valarray<double>& x) {
    valarray<double> f_curr = ode->f(x, current_time);

    if (f_history.size() < order) {
        f_history.push_back(f_curr);
        // 使用欧拉法进行积分
        x += f_curr * dt;
    }
    else {
        f_history.pop_front();
        f_history.push_back(f_curr);
        
        // 使用Adams方法进行积分
        size_t order = f_history.size();
        valarray<double> delta_x(x.size());
        for (int i = 0; i < order; ++i) {
            delta_x += coefficients[order - 1][i] * f_history[order - 1 - i];
        }
        x += delta_x * dt;
    }
    current_time += dt;
}


void RungeKuttaSolver::integrate(ODE* ode, double dt, valarray<double>& x) {
    // Implement the Runge-Kutta integration method here
    size_t n = order;
    vector<valarray<double>> k(n, valarray<double>(x.size()));
    vector<double> c(n), a(n * n), b(n);
    switch (order) {
	case 1:
		c = { 0.0 };
		a = { 0.0 };
		b = { 1.0 };
		break;
	case 2:
		c = { 0.0, 0.5 };
		a = { 0.0, 0.5, 
            0.0, 0.0 
        };
		b = { 0.0, 1.0 };
		break;

    case 3:
        c = { 0.0, 0.5, 1.0 };
        a = { 
            0.0, 0.5, 0.0, 
            0.0, 0.5, 0.0,
            0.0, 1.0, 0.0 
        };
        b = { 1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0 };
        break;
    case 4: 
        c = { 0.0, 0.5, 0.5, 1.0 };
        a = {
            0.0, 0.0, 0.0, 0.0,   // k1 不依赖其他阶段
            0.5, 0.0, 0.0, 0.0,   // k2 依赖 k1（系数 0.5）
            0.0, 0.5, 0.0, 0.0,   // k3 依赖 k2（系数 0.5）
            0.0, 0.0, 1.0, 0.0    // k4 依赖 k3（系数 1.0）
        };
        b = { 1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0 };
		break;
	default:
		throw invalid_argument("Unsupported order, order must be between 1 and 4");
    }
    for (size_t i = 0; i < n; ++i) {
        valarray<double> y_temp = x;
        for (size_t j = 0; j < i; ++j) {
            y_temp += a[i * n + j] * k[j] * dt;
        }
        k[i] = ode->f(y_temp, current_time + c[i] * dt);
    }

    for (size_t i = 0; i < n; ++i) {
        x += b[i] * k[i] * dt;
    }

    current_time += dt;
}

