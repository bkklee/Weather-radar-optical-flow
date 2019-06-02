#include <bits/stdc++.h>
using namespace std;

#define SIZE 20

/*
Unit: 
length: grid
intensity: mm/hr
time: 12 mins
*/

double firstOrderDifferentiation(double x1, double x2, double dx){
	return (x2-x1)/dx;
}

int main(){
	
	//File input
	ifstream fin("test_rainmap_12mins"); //12 mins ago
	ifstream fin2("test_rainmap_now"); //real now
	
	//Variable
	vector<vector<double>> I(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> I_12mins(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdt(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdx(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> dIdy(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> u(SIZE, vector<double>(SIZE, 0.0));
	vector<vector<double>> v(SIZE, vector<double>(SIZE, 0.0));
	
	//Input from rain map
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
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
	
	double (*fOD)(double,double,double) = firstOrderDifferentiation;
	double alpha = 0.00; //Smoothing constant
	double a = 0.0001; //Descent rate
	double N = 100000; //No. of descent
	
	for(int n=0;n<N;n++){
		for(int i=0;i<SIZE;i++){
			for(int j=0;j<SIZE;j++){
				
				double du = 0.001;
				double dv = 0.001;
				
				double J0 = (dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]);
				double tmpi = i;
				double tmpj = j;
				if(i==SIZE-1){
					tmpi -= 1;
				}
				if(j==SIZE-1){
					tmpj -= 1;
				}
				
				double JHS = fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)*fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)+
							 fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)*fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)+
							 fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)*fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)+
							 fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0)*fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0);
							 
				if(J0+alpha*JHS > 0.001){
					double J0du = (dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+(u[i][j]+du)*dIdx[i][j]+v[i][j]*dIdy[i][j]);
					double J0dv = (dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+(v[i][j]+dv)*dIdy[i][j]);
					
					double JHSdu = fOD(u[tmpi][tmpj]+du,u[tmpi+1][tmpj],1.0)*fOD(u[tmpi][tmpj]+du,u[tmpi+1][tmpj],1.0)+
								   fOD(u[tmpi][tmpj]+du,u[tmpi][tmpj+1],1.0)*fOD(u[tmpi][tmpj]+du,u[tmpi][tmpj+1],1.0)+
								   fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)*fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)+
								   fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0)*fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0);
					double JHSdv = fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)*fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)+
								   fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)*fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)+
								   fOD(v[tmpi][tmpj]+dv,v[tmpi+1][tmpj],1.0)*fOD(v[tmpi][tmpj]+dv,v[tmpi+1][tmpj],1.0)+
								   fOD(v[tmpi][tmpj]+dv,v[tmpi][tmpj+1],1.0)*fOD(v[tmpi][tmpj]+dv,v[tmpi][tmpj+1],1.0);
					
					double dJdu = firstOrderDifferentiation(J0+alpha*JHS,J0du+alpha*JHSdu,du);
					double dJdv = firstOrderDifferentiation(J0+alpha*JHS,J0dv+alpha*JHSdv,dv);
					
					u[i][j] -= a*dJdu;
					v[i][j] -= a*dJdv;
				}
			}
		}
	}
	
	//3. Advection
	vector<vector<double>> newI(SIZE, vector<double>(SIZE, 0.0));
	
	for(int i=0;i<SIZE;i++){
		for(int j=0;j<SIZE;j++){
			newI[i][j] = I[i][j] - v[i][j]*dIdy[i][j] - u[i][j]*dIdx[i][j];
		}
	}
	
	
	//File output
	ofstream fout("costfunction.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			double J0 = (dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j])*(dIdt[i][j]+u[i][j]*dIdx[i][j]+v[i][j]*dIdy[i][j]);
			double tmpi = i;
			double tmpj = j;
			if(i==SIZE-1){
				tmpi -= 1;
			}
			if(j==SIZE-1){
				tmpj -= 1;
			}
			
			double JHS = fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)*fOD(u[tmpi][tmpj],u[tmpi+1][tmpj],1.0)+
						 fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)*fOD(u[tmpi][tmpj],u[tmpi][tmpj+1],1.0)+
						 fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)*fOD(v[tmpi][tmpj],v[tmpi+1][tmpj],1.0)+
						 fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0)*fOD(v[tmpi][tmpj],v[tmpi][tmpj+1],1.0);
			
			fout << J0+alpha*JHS << " ";
		}
		fout << endl;
	}
	
	fout.close();
	
	ofstream fout2("direction.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			if(abs(u[i][j])<0.01){
				u[i][j] = 0.0;
			}
			if(abs(v[i][j]<0.01)){
				v[i][j] = 0.0;
			}
			if(u[i][j]>0.0 && v[i][j]>0.0){
				fout2 << "¡ù" << " ";
			}else if(u[i][j]>0.0 && v[i][j]<0.0){
				fout2 << "¡û" << " ";
			}else if(u[i][j]<0.0 && v[i][j]>0.0){
				fout2 << "¡ø" << " ";
			}else if(u[i][j]<0.0 && v[i][j]<0.0){
				fout2 << "¡ú" << " ";
			}else if(u[i][j]==0.0 && v[i][j]==0.0){
				fout2 << "//" << " ";
			}else if(u[i][j]==0.0 && v[i][j]>0.0){
				fout2 << "¡ô" << " ";
			}else if(u[i][j]==0.0 && v[i][j]<0.0){
				fout2 << "¡õ" << " ";
			}else if(u[i][j]>0.0 && v[i][j]==0.0){
				fout2 << "¡÷" << " ";
			}else{
				fout2 << "¡ö" << " ";
			}
		}
		fout2 << endl;
	}
	
	fout2.close();
	
	ofstream fout3("velocity.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			fout3 << "(" << u[i][j] << "," << v[i][j] << ")" << " ";
		}
		fout3 << endl;
	}
	
	fout3.close();
	
	ofstream fout4("rainmapnext12mins.txt");
	
	for(int j=0;j<SIZE;j++){
		for(int i=0;i<SIZE;i++){
			fout4 << newI[i][j] << " ";
		}
		fout4 << endl;
	}
	
	fout4.close();
	
	//Done!
	cout << "Done!" << endl;
	
	return 0;
}
