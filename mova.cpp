#include <bits/stdc++.h>
using namespace std;

#define SIZE 385

/*
Unit: 
length: m
intensity: mm/hr
*/

const double ds = 320.512820513;

double firstOrderDifferentiation(double x1, double x2, double dx){
	return (x2-x1)/dx;
}

double secondOrderDifferentiation(double x1, double x2, double dx){
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
	vector<vector<double>> u(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> v(SIZE, vector<double>(SIZE, 0.0));
	
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
	
	//1. Find dIdt, dIdx, dIdy
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			dIdt[i][j] = firstOrderDifferentiation(I_12mins[i][j],I[i][j],ds);
			
			if(i!=0 && i!= SIZE-1){
				dIdx[i][j] = secondOrderDifferentiation(I[i-1][j], I[i+1][j],ds);
			}else{
				if(i==0){
					dIdx[i][j] = firstOrderDifferentiation(I[i][j], I[i+1][j],ds);
				}else{
					dIdx[i][j] = firstOrderDifferentiation(I[i-1][j],I[i][j],ds);
				}
			}
			
			if(j!=0 && j!= SIZE-1){
				dIdy[i][j] = secondOrderDifferentiation(I[i][j-1], I[i][j+1],ds);
			}else{
				if(j==0){
					dIdy[i][j] = firstOrderDifferentiation(I[i][j], I[i][j+1],ds);
				}else{
					dIdy[i][j] = firstOrderDifferentiation(I[i][j-1], I[i][j],ds);
				}
			}
		}
	}
	
	//2. Gradient desecent
	for(int n=0;n<1000;n++){
		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++){
				double du = 0.01;
				double dv = 0.01;
				double a = 1;
				double dJdu = secondOrderDifferentiation((dIdt[i][j]+(u[i][j]-du)*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+(u[i][j]-du)*dIdx[i][j]+v[i][j]*dIdy[i][j]),(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j]),du);
				double dJdv = secondOrderDifferentiation((dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]-dv)*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]-dv)*dIdy[i][j]),(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j]),dv);
				u[i][j] -= a*dJdu;
				v[i][j] -= a*dJdv;
			}
		}
	}
	
	//File output
	ofstream fout("output.txt");
	
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			fout << u[i][j] << " ";
		}
		fout << endl;
	}
	
	fout.close();
	
	//Done!
	cout << "Done!" << endl;
	
	return 0;
}
