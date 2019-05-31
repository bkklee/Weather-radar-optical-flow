#include <bits/stdc++.h>
using namespace std;

#define SIZE 385

//Unit:

const double dx = 1.0;

double firstOrderDifferentiation(double x1, double x2){
	return (x2-x1)/dx;
}

double secondOrderDifferentiation(double x1, double x2){
	return (x2-x1)/(2.0*dx);
}

int main(){
	
	//File input
	ifstream fin("rainmapnow.txt");
	ifstream fin2("rainmap12mins.txt");
	
	//Variable
	vector<vector<double>> I(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> I_12mins(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdt(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdx(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdy(SIZE, vector<double>(SIZE, 0.0));
	
	//Input from rain map
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			fin >> I[i][j];
			fin2 >> I_12mins[i][j];
		}
	}
	
	fin.close();
	fin2.close();
	
	//Mova here
	
	//1. Cal dIdt, dIdx, dIdy
	
	//2. Multi-variable gradient desecent
	
	//Done!
	
	
	cout << "Great!" << endl;
	
	return 0;
}
