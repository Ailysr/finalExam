#include "ODE.h"

using namespace std;

// ͨ�����ӷ�ĸ����ʽ�Ĺ��캯��
TransferFunction::TransferFunction(const valarray<double>& numerator, const valarray<double>& denominator)
   : numerator(numerator), denominator(denominator) {
   if (denominator.size() == 0) {
       throw invalid_argument("Denominator must have at least one element");
   }
   tf2StateSpace();
}
// ͨ��״̬�ռ����Ĺ��캯��
TransferFunction::TransferFunction(const valarray<valarray<double>>& A, const valarray<double>& B, const valarray<double>& C, double D)
    : A(A), B(B), C(C), D(D) {
}

pair<valarray<double>, valarray<double>> TransferFunction::polynomialDivision(
    const valarray<double>& num, const valarray<double>& den)
{
    if (den.size() == 0 || abs(den[0]) < 1e-12) {
        throw invalid_argument("Denominator polynomial cannot be zero.");
    }

    valarray<double> remainder = num;
    size_t deg_num = num.size() - 1;
    size_t deg_den = den.size() - 1;

    // quotient degree = deg_num - deg_den + 1 (if num is of higher degree)
    if (deg_num < deg_den) {
        // ��Ϊ 0������ʽ�޷�����
        return { valarray<double>(0.0, 1), remainder };
    }

    valarray<double> quotient(0.0, deg_num - deg_den + 1);

    for (size_t i = 0; i <= deg_num - deg_den; ++i) {
        // ��ǰ�������ϵ��
        double coeff = remainder[i] / den[0];
        quotient[i] = coeff;

        // ���쵱ǰ����������Գ�ʽ�Ķ���ʽ��������
        for (size_t j = 0; j <= deg_den; ++j) {
            remainder[i + j] -= coeff * den[j];
        }
    }

    // ��ȡ�������֣��� quotient ֮����
    size_t remainder_start = deg_num - deg_den + 1;
    valarray<double> real_remainder = remainder[remainder_start != remainder.size() ? slice(remainder_start, remainder.size() - remainder_start, 1) : slice(0, 0, 1)];

    return {quotient, real_remainder };
}

void TransferFunction::tf2StateSpace() {
    size_t n = denominator.size() - 1; // ϵͳ�Ľ���
    
    pair<valarray<double>, valarray<double>> result = polynomialDivision(numerator, denominator);
    valarray<double> q = result.first;
    valarray<double> r = result.second;
    size_t m = r.size() - 1;   // ���ӵĽ���

    // ��ʼ��״̬�ռ����
    A.resize(n, valarray<double>(n));
    B.resize(n);
    C.resize(n);
    // ���� D
	D = q[0];

    // ������ A
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i == n - 1) {
                A[i][j] = -denominator[n - j] / denominator[0];
            }
            else if (j == i + 1) {
                A[i][j] = 1.0;
            }
            else {
                A[i][j] = 0.0;
            }
        }
    }

    // ������ B
    for (size_t i = 0; i < n; ++i) {
        if (i == n - 1) {
            B[i] = 1.0 / denominator[0];
        }
        else {
            B[i] = 0.0;
        }
    }

    // ������ C
	
    for (size_t i = 0; i < n; ++i) {
        if (i <= m) {
            C[i] = r[m - i] / denominator[0];
        }
        else {
            C[i] = 0.0;
        }
    }
}

valarray<double> TransferFunction::f(const valarray<double>& x, double t) {
   double input = u(t);
   valarray<double> dxdt(x.size());

   for (size_t i = 0; i < x.size(); ++i) {
       dxdt[i] = 0.0;
       for (size_t j = 0; j < x.size(); ++j) {
           dxdt[i] += A[i][j] * x[j];
       }
       dxdt[i] += B[i] * input;
   }
   return dxdt;
}
double TransferFunction::output(const valarray<double>& x, double t) {
    return (C * x).sum() + D * u(t);
};


SimpleODE::SimpleODE(const valarray<double>& coefficients) : coefficients(coefficients) {
	if (coefficients.size() == 0) {
		throw invalid_argument("Coefficients must have at least one element");
	}
	numerator = valarray<double>{ 1.0 }; // ����Ϊ1
	denominator = coefficients; // ��ĸΪ΢�ַ��̵�ϵ��
	TransferFunction tf(numerator, denominator); // ͨ�����ӷ�ĸ����ʽ�Ĺ��캯������Ȼ��ת��Ϊ״̬�ռ�ģ��
	// ͨ��״̬�ռ����Ĺ��캯��
	A = tf.getA();
	B = tf.getB();
	C = tf.getC();
	D = tf.getD();
}


valarray<double> SimpleODE::f(const valarray<double>& x, double t) {
    double input = u(t);
    valarray<double> dxdt(x.size());

    for (size_t i = 0; i < x.size(); ++i) {
        dxdt[i] = 0.0;
        for (size_t j = 0; j < x.size(); ++j) {
            dxdt[i] += A[i][j] * x[j];
        }
        dxdt[i] += B[i] * input;
    }
    return dxdt;
}
double SimpleODE::output(const valarray<double>& x, double t) {// ���������״̬������ĳ����ϣ�����ֱ�ӷ��� x[0]��λ�ã�
    return (C * x).sum() + D * u(t);
};
