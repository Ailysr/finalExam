#pragma once

#include <valarray>
#include <functional>
#include <vector>
#include <stdexcept>
#include <map>
#include <algorithm>

using namespace std;

class ODE {
public:
    virtual ~ODE() = default;
    virtual valarray<double> f(const valarray<double>& x, double t) = 0;

    // 添加 output 方法为纯虚函数
    virtual double output(const valarray<double>& x, double t) = 0;
    //作用是计算在给定状态 x 、时间 t 和输入u下的导数或变化率
    void setInputFunction(function<double(double)> inputFunc) {
        u = inputFunc;
    }
protected:
	function<double(double)> u; // 输入函数
};

class TransferFunction : public ODE {
private:
    valarray<double> numerator;
    valarray<double> denominator;
    valarray<valarray<double>> A;
    valarray<double> B;
    valarray<double> C;
    double D;

	void tf2StateSpace();
    //多项式相除函数
    pair<valarray<double>, valarray<double>> polynomialDivision(const valarray<double>& numerator, const valarray<double>& denominator);


public:
    // 通过分子分母多项式的构造函数
    TransferFunction(const valarray<double>& numerator, const valarray<double>& denominator);

    // 通过状态空间矩阵的构造函数
    TransferFunction(const valarray<valarray<double>>& A, const valarray<double>& B, const valarray<double>& C, double D);
    valarray<double> f(const valarray<double>& x, double t) override;
	// 计算状态空间模型的输出
    double output(const valarray<double>& x, double t) override;
    // 添加 getter 方法
    const valarray<valarray<double>>& getA() const { return A; }
    const valarray<double>& getB() const { return B; }
    const valarray<double>& getC() const { return C; }
    double getD() const { return D; }
};

class SimpleODE : public ODE {  
private:  
    valarray<double> coefficients; // 存储微分方程的系数，例如 {a0, a1, ..., an}
    valarray<double> numerator;
    valarray<double> denominator;
    valarray<valarray<double>> A;
    valarray<double> B;
    valarray<double> C;
    double D;
    //void ode2StateSpace(coefficients);
public:  
    SimpleODE(const valarray<double>& coeffs);
    valarray<double> f(const valarray<double>& x, double t) override;  
    double output(const valarray<double>& x, double t) override; 
};


