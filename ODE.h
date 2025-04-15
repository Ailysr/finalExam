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

    // ��� output ����Ϊ���麯��
    virtual double output(const valarray<double>& x, double t) = 0;
    //�����Ǽ����ڸ���״̬ x ��ʱ�� t ������u�µĵ�����仯��
    void setInputFunction(function<double(double)> inputFunc) {
        u = inputFunc;
    }
protected:
	function<double(double)> u; // ���뺯��
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
    //����ʽ�������
    pair<valarray<double>, valarray<double>> polynomialDivision(const valarray<double>& numerator, const valarray<double>& denominator);


public:
    // ͨ�����ӷ�ĸ����ʽ�Ĺ��캯��
    TransferFunction(const valarray<double>& numerator, const valarray<double>& denominator);

    // ͨ��״̬�ռ����Ĺ��캯��
    TransferFunction(const valarray<valarray<double>>& A, const valarray<double>& B, const valarray<double>& C, double D);
    valarray<double> f(const valarray<double>& x, double t) override;
	// ����״̬�ռ�ģ�͵����
    double output(const valarray<double>& x, double t) override;
    // ��� getter ����
    const valarray<valarray<double>>& getA() const { return A; }
    const valarray<double>& getB() const { return B; }
    const valarray<double>& getC() const { return C; }
    double getD() const { return D; }
};

class SimpleODE : public ODE {  
private:  
    valarray<double> coefficients; // �洢΢�ַ��̵�ϵ�������� {a0, a1, ..., an}
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


