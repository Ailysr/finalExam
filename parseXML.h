#include <iostream>
#include <valarray>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <stdexcept>
#include <memory>
#include "ODE.h"
#include "OdeSolver.h"
#include "ExpressionParser.h"

using namespace std;

using namespace std;

static void parseXML(const string& xmlFile, string& selectedProblemType, vector<double>& coefficients, vector<double>& numerator,
   vector<double>& denominator, vector<vector<double>>& A, vector<double>& B, vector<double>& C, double& D,
   vector<double>& initialState, double& dt, int& steps, int& solverOrder, string& solverType, string& resultPath , function<double(double)>& inputFunction) {
   ifstream file(xmlFile);
   if (!file.is_open()) {
       throw runtime_error("Failed to open XML file");
   }
   string inputFunctionExpr;
   string line;

   while (getline(file, line)) {
       // ����Ƿ��� <problem> ��ǩ
       if (line.find("<problem") != string::npos) {
           // ��� state ����
           size_t statePos = line.find("state=\"");
           if (statePos != string::npos) {
               char state = line[statePos + 7]; // ��ȡ state ��ֵ
               if (state == '1') {
                   // ��ȡ��������
                   size_t typePos = line.find("type=\"");
                   if (typePos != string::npos) {
                       selectedProblemType = line.substr(typePos + 6, line.find("\"", typePos + 6) - (typePos + 6));

                       // �����������ͽ����������
                       if (selectedProblemType == "SimpleODE") {
                           // ���� coefficients �� initialState
                           while (getline(file, line)) {
                               if (line.find("<coefficients>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   coefficients.clear();
                                   while (ss >> value) {
                                       coefficients.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<initialState>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   initialState.clear();
                                   while (ss >> value) {
                                       initialState.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<inputFunction>") != string::npos) {
                                   size_t start = line.find("<inputFunction>") + 15;
                                   size_t end = line.find("</inputFunction>");
                                   if (end != string::npos) {
                                       inputFunctionExpr = line.substr(start, end - start);
                                   }
                                   else {
                                       inputFunctionExpr = line.substr(start); // �����Ƕ��б��ʽ���ȶ���ǰ��ʣ��
                                       string exprLine;
                                       while (getline(file, exprLine)) {
                                           size_t closing = exprLine.find("</inputFunction>");
                                           if (closing != string::npos) {
                                               inputFunctionExpr += exprLine.substr(0, closing);
                                               break;
                                           }
                                           else {
                                               inputFunctionExpr += exprLine;
                                           }
                                       }
                                   }
                               }
                               if (line.find("</problem>") != string::npos) {
                                   break; // ������ɣ��˳�ѭ��
                               }
                           }
                       }
                       else if (selectedProblemType == "TransferFunction") {
                           // ���� numerator��denominator �� initialState
                           while (getline(file, line)) {
                               if (line.find("<numerator>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   numerator.clear();
                                   while (ss >> value) {
                                       numerator.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<denominator>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   denominator.clear();
                                   while (ss >> value) {
                                       denominator.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<initialState>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   initialState.clear();
                                   while (ss >> value) {
                                       initialState.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<inputFunction>") != string::npos) {
                                   size_t start = line.find("<inputFunction>") + 15;
                                   size_t end = line.find("</inputFunction>");
                                   if (end != string::npos) {
                                       inputFunctionExpr = line.substr(start, end - start);
                                   }
                                   else {
                                       inputFunctionExpr = line.substr(start); // �����Ƕ��б��ʽ���ȶ���ǰ��ʣ��
                                       string exprLine;
                                       while (getline(file, exprLine)) {
                                           size_t closing = exprLine.find("</inputFunction>");
                                           if (closing != string::npos) {
                                               inputFunctionExpr += exprLine.substr(0, closing);
                                               break;
                                           }
                                           else {
                                               inputFunctionExpr += exprLine;
                                           }
                                       }
                                   }
                               }
                               if (line.find("</problem>") != string::npos) {
                                   break; // ������ɣ��˳�ѭ��
                               }
                           }
                       }
                       else if (selectedProblemType == "StateSpace") {
                           // ���� A��B��C��D �� initialState
                           while (getline(file, line)) {
                               if (line.find("<A>") != string::npos) {
                                   A.clear();
                                   while (getline(file, line) && line.find("</A>") == string::npos) {
                                       if (line.find("<row>") != string::npos) {
                                           string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                           stringstream ss(values);
                                           vector<double> row;
                                           double value;
                                           while (ss >> value) {
                                               row.push_back(value);
                                               if (ss.peek() == ',') ss.ignore();
                                           }
                                           A.push_back(row);
                                       }
                                   }
                               }
                               if (line.find("<B>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   B.clear();
                                   while (ss >> value) {
                                       B.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<C>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   C.clear();
                                   while (ss >> value) {
                                       C.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<D>") != string::npos) {
                                   D = stod(line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1));
                               }
                               if (line.find("<initialState>") != string::npos) {
                                   string values = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);
                                   stringstream ss(values);
                                   double value;
                                   initialState.clear();
                                   while (ss >> value) {
                                       initialState.push_back(value);
                                       if (ss.peek() == ',') ss.ignore();
                                   }
                               }
                               if (line.find("<inputFunction>") != string::npos) {
                                   size_t start = line.find("<inputFunction>") + 15;
                                   size_t end = line.find("</inputFunction>");
                                   if (end != string::npos) {
                                       inputFunctionExpr = line.substr(start, end - start);
                                   }
                                   else {
                                       inputFunctionExpr = line.substr(start); // �����Ƕ��б��ʽ���ȶ���ǰ��ʣ��
                                       string exprLine;
                                       while (getline(file, exprLine)) {
                                           size_t closing = exprLine.find("</inputFunction>");
                                           if (closing != string::npos) {
                                               inputFunctionExpr += exprLine.substr(0, closing);
                                               break;
                                           }
                                           else {
                                               inputFunctionExpr += exprLine;
                                           }
                                       }
                                   }
                               }
                               if (line.find("</problem>") != string::npos) {
                                   break; // ������ɣ��˳�ѭ��
                               }
                           }
                       }

                       // �������������ͺ󣬼������� solver ��Ϣ
                       while (getline(file, line)) {
                           if (line.find("<solver") != string::npos) {
                               size_t typePos = line.find("type=\"");
                               if (typePos != string::npos) {
                                   solverType = line.substr(typePos + 6, line.find("\"", typePos + 6) - (typePos + 6));
                               }
                               else {
                                   throw runtime_error("Solver type attribute not found in <solver> tag.");
                               }
                           }
                           if (line.find("<order>") != string::npos) {
                               if (solverType != "Euler") { // ������� Euler������ order ����
                                   size_t start = line.find("<order>") + 7; // �ҵ� <order> ����ʼλ��
                                   size_t end = line.find("</order>"); // �ҵ� </order> �Ľ���λ��
                                   if (end != string::npos && start < end) {
                                       solverOrder = stoi(line.substr(start, end - start)); // ��ȡ��ת��Ϊ����
                                   }
                                   else {
                                       throw runtime_error("Invalid <order> tag format in XML.");
                                   }
                               }
                               else {
                                   solverOrder = 1; // Euler Ĭ�Ͻ���Ϊ 1
                               }
                           }
                           if (line.find("<dt>") != string::npos) {
                               dt = stod(line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1));
                           }

                           if (line.find("<steps>") != string::npos) {
                               steps = stoi(line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1));
                           }

                           // ���� resultPath
                           if (line.find("<resultPath>") != string::npos) {
                               resultPath = line.substr(line.find(">") + 1, line.rfind("<") - line.find(">") - 1);

                               break; // ������ɣ��˳�ѭ��
                           }
                       }
                       cout << "The problemType is " << selectedProblemType << endl;
                       if(selectedProblemType == "StateSpace")
					   {
						   cout << "The A matrix is: " << endl;
						   for (const auto& row : A) {
							   for (const auto& val : row) {
								   cout << val << " ";
							   }
							   cout << endl;
						   }
						   cout << "The B matrix is: ";
						   for (const auto& val : B) {
							   cout << val << " ";
						   }
						   cout << endl;
						   cout << "The C matrix is: ";
						   for (const auto& val : C) {
							   cout << val << " ";
						   }
						   cout << endl;
						   cout << "The D value is: " << D << endl;
					   }
					   else if (selectedProblemType == "TransferFunction")
					   {
						   cout << "The numerator is: ";
						   for (const auto& val : numerator) {
							   cout << val << " ";
						   }
						   cout << endl;
						   cout << "The denominator is: ";
						   for (const auto& val : denominator) {
							   cout << val << " ";
						   }
						   cout << endl;
					   }
                       else if (selectedProblemType == "SimpleODE")
                       {
						   cout << "The coefficients are: ";
						   for (const auto& val : coefficients) {
							   cout << val << " ";
						   }
						   cout << endl;
                       }
                       cout << "The initialState is: ";
                       for (const auto& val : initialState) {
                           cout << val << " ";
                       }
                       inputFunction = [inputFunctionExpr](double t) {
                           ExpressionParser parser(inputFunctionExpr);
                           auto func = parser.compile();
                           return func(t);
                           };
                       cout << endl;
                       cout << "Your Simulation Config:" << endl;
                       cout << "solverType:" << solverType << endl;
                       cout << "solver order:" << solverOrder << endl;
					   cout << "Input Funciton:" << inputFunctionExpr << endl;
                       cout << "steps:" << steps << endl;
                       cout << "dt is:" << dt << endl;
                       file.close();
                       return; // ������ɣ��˳�����
                   }
               }
           }
       }
   }
   throw runtime_error("No problem with state=\"1\" found in the configuration file.");
};