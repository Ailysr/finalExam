#pragma once

#include "ODE.h"
#include <deque>
#include <vector>
using namespace std;

class OdeSolver {
public:
    virtual ~OdeSolver() = default;
    virtual void integrate(ODE* ode, double dt, valarray<double>& x) = 0;
};

class AdamsSolver : public OdeSolver {
private:
    double current_time = 0.0;
    int order;
    deque<valarray<double>> f_history;
    vector<vector<double>> coefficients;

public:
    AdamsSolver(int order);
    void integrate(ODE* ode, double dt, valarray<double>& x) override;
};

class RungeKuttaSolver : public OdeSolver {
private:
	double current_time = 0.0;
    int order;
public:
    RungeKuttaSolver(int order) :order(order) {};
    void integrate(ODE* ode, double dt, valarray<double>& x) override;
};

class EulerSolver : public OdeSolver {
    //œ‘ æ≈∑¿≠∑®
private:
	double current_time = 0.0;

public:
	EulerSolver() {};
	void integrate(ODE* ode, double dt, valarray<double>& x) override;
    
};
