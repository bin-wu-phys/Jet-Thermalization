#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

double f0(double z,double pz){
  return exp(-10.0*(pz-5.0)*(pz-5.0)-10.0*z*z);
}

int main(){
  int np=400,Ntot=2*np+1,nz=200,Nztot=2*nz+1;
  double _ng0;
  double f[Nztot][Ntot],jp[Nztot+1][Ntot+1],jz[Nztot+1][Ntot+1],pz[Ntot],z[Nztot],pzMax=10.0,zMax=10.0,dpz=pzMax/((double)np),dz=zMax/((double)nz);
  double t = 0, dt = 1.0e-4, tMax=10.0;	//setup time variables
  double tSave = 0.1;	//tCheck is used to determine the time interval to check thermalization. 
  unsigned int nSave = (unsigned int)(tSave/dt), ns = 0;//time to save the data

  cout.precision(15);
  cout << "# t = " << t << endl;
  for(int i=0;i<Nztot;i++){
    z[i]=-zMax+i*dz;
    for(int j=0;j<Ntot;j++){
      pz[j]=-pzMax+0.1*dpz+j*dpz;f[i][j]=f0(z[i],pz[j]);
      cout << z[i] << " " << pz[j] << " " << f[i][j] << endl;
    }
    cout << endl;
  }

  //boundary condition
  for(int i=0;i<Ntot;i++){
    jz[0][i]=0.0;jz[Nztot][i]=0.0;
  }

  for(int i=0;i<Nztot;i++){
    jp[i][0]=0.0;jp[i][Ntot]=0.0;
  }
  
  _ng0=0;
  for(int i=0;i<Nztot;i++){
    for(int j=0;j<Ntot;j++){
      _ng0+=f[i][j];
    }
  }
  
  do{
  for(int i=1;i<Nztot;i++){
    for(int j=1;j<Ntot;j++){
      double fs=0.5*(f[i][j]+f[i][j-1]),pzs=0.5*(pz[j]+pz[j-1]);
      if(pzs<0){
	jp[i][j]=(f[i][j]-f[i][j-1])/dpz-fs;
      }else{
	jp[i][j]=(f[i][j]-f[i][j-1])/dpz+fs;
      }
      jz[i][j]=f[i][j];
    }
  }
    
    for(int i=0;i<Nztot;i++){
      for(int j=0;j<Ntot;j++){
	if(pz[j]>0&&i>0)
	  f[i][j]+=dt*( (jp[i][j+1]-jp[i][j])/dpz-(jz[i][j]-jz[i-1][j])/dz);
	else
	  f[i][j]+=dt*( (jp[i][j+1]-jp[i][j])/dpz+(jz[i+1][j]-jz[i][j])/dz);
      }
    }
    
    t+=dt;ns++;

    if(ns==nSave){
      ns=0;cout << "\n\n# t = " << t << endl;    
      for(int i=0;i<Nztot;i++){
	for(int j=0;j<Ntot;j++){
	  cout << z[i] << " " << pz[j] << " " << f[i][j] << endl;
	  }
	cout << endl;
      }
      /*
      double _ng=0;
      for(int i=0;i<Nztot;i++){
	for(int j=0;j<Ntot;j++){
	  _ng+=f[i][j];
	}
      }
      cout << t << " " << 1.0-_ng/_ng0 << endl;
      */ 
   }
  
  }while(t<=tMax);
  
  return 0;
}
