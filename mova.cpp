#include <bits/stdc++.h>
using namespace std;

#define SIZE 20

/*
Unit: 
length: pixel
intensity: mm/hr
time: 12 mins
*/

double firstOrderDifferentiation(double x1, double x2, double dx){
	return (x2-x1)/dx;
}

int main(){
	
	//File input
	ifstream fin("rainmapnow.txt"); //12 mins ago
	ifstream fin2("rainmap12mins.txt"); //real now
	
	//Variable
	vector<vector<double>> I(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> I_12mins(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdt(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdx(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdy(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> u(SIZE, vector<double>(SIZE, 10.0));
	vector<vector<double>> v(SIZE, vector<double>(SIZE, 10.0));
	
	//Input from rain map
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			fin >> I_12mins[i][j]; //12 mins ago
			fin2 >> I[i][j]; //real now
		}
	}
	
	fin.close();
	fin2.close();
	
	//Mova here
	
	//1. Find dIdt, dIdx, dIdy
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			dIdt[i][j] = firstOrderDifferentiation(I_12mins[i][j],I[i][j],1.0);
			
			if(i != SIZE-1){
				dIdx[i][j] = firstOrderDifferentiation(I[i][j], I[i+1][j],1.0);
			}else{
				dIdx[i][j] = firstOrderDifferentiation(I[i-1][j],I[i][j],1.0);
			}
			
			if(j != SIZE-1){
				dIdy[i][j] = firstOrderDifferentiation(I[i][j], I[i][j+1],1.0);
			}else{
				dIdy[i][j] = firstOrderDifferentiation(I[i][j-1],I[i][j],1.0);
			}
		}
	}
	
	//2. Gradient desecent
	for(int n=0;n<100000;n++){
		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++){
				double du = 0.001;
				double dv = 0.001;
				double a = 0.001;
				double dJdu = firstOrderDifferentiation((dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]),(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j]),du);
				double dJdv = firstOrderDifferentiation((dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]),(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j]),dv);
				u[i][j] -= a*dJdu;
				v[i][j] -= a*dJdv;
			}
		}
	}
	
	for(int i=3;i<4;i++){
		for(int j=0;j<1;j++){
			double du = 0.001;
			double dv = 0.001;
			double a = 0.001;
			double dJdu = firstOrderDifferentiation((dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]),(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j]),du);
			double dJdv = firstOrderDifferentiation((dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]),(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j]),dv);
			cout << dJdu << endl;
			cout << dJdv << endl;
		}
	}
	
	//File output
	ofstream fout("output.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			fout << (dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]) << " ";
		}
		fout << endl;
	}
	
	fout.close();
	
	ofstream fout2("output2.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			if(abs(u[i][j]-10.0)<0.1){
				u[i][j] = 0.0;
			}
			if(u[i][j]>0.0){
				fout2 << "+" << " ";
			}else if(u[i][j]<0.0){
				fout2 << "-" << " ";
			}else{
				fout2 << "/" << " ";
			}
		}
		fout2 << endl;
	}
	
	fout2.close();
	
	//Done!
	cout << "Done!" << endl;
	
	return 0;
}
