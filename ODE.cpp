#include "ODE.h"

using namespace std;

// 通过分子分母多项式的构造函数
TransferFunction::TransferFunction(const valarray<double>& numerator, const valarray<double>& denominator)
   : numerator(numerator), denominator(denominator) {
   if (denominator.size() == 0) {
       throw invalid_argument("Denominator must have at least one element");
   }
   tf2StateSpace();
}
// 通过状态空间矩阵的构造函数
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
        // 商为 0，多项式无法整除
        return { valarray<double>(0.0, 1), remainder };
    }

    valarray<double> quotient(0.0, deg_num - deg_den + 1);

    for (size_t i = 0; i <= deg_num - deg_den; ++i) {
        // 当前被除项的系数
        double coeff = remainder[i] / den[0];
        quotient[i] = coeff;

        // 构造当前除法结果乘以除式的多项式，做减法
        for (size_t j = 0; j <= deg_den; ++j) {
            remainder[i + j] -= coeff * den[j];
        }
    }

    // 截取余数部分（从 quotient 之后的项）
    size_t remainder_start = deg_num - deg_den + 1;
    valarray<double> real_remainder = remainder[remainder_start != remainder.size() ? slice(remainder_start, remainder.size() - remainder_start, 1) : slice(0, 0, 1)];

    return { quotient, real_remainder };
}

void TransferFunction::tf2StateSpace() {
    size_t n = denominator.size() - 1; // 系统的阶数
    
    pair<valarray<double>, valarray<double>> result = polynomialDivision(numerator, denominator);
    valarray<double> q = result.first;
    valarray<double> r = result.second;
    size_t m = r.size() - 1;   // 分子的阶数

    // 初始化状态空间矩阵
    A.resize(n, valarray<double>(n));
    B.resize(n);
    C.resize(n);
    // 计算 D
	D = q[0];

    // 填充矩阵 A
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

    // 填充矩阵 B
    for (size_t i = 0; i < n; ++i) {
        if (i == n - 1) {
            B[i] = 1.0 / denominator[0];
        }
        else {
            B[i] = 0.0;
        }
    }

    // 填充矩阵 C
	
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

valarray<double> SimpleODE::f(const valarray<double>& x, double t) {
   double input = u(t);
   valarray<double> result(x.size());
   result[0] = x[0] + input;
   return result;
}
