#include <math.h>
#include <fstream>
#include <sstream>
#include <iomanip>

#define PI 3.141592653589793

using namespace std;

class fg{
 public:
  int _n,_nm1, 
    _ntot,//total dimension in p
    _nminus;//number of grid for negative p
  int _nz, _nztot;//total number of grid in z
  double _zMin,_pzMin;//minimum value for z

  //initial condition
  double _E;

  //Output file name
  ostringstream _ostr;

  double **_f;//distribution function
  double **_jp, **_jz;//elastic collision term
  double *_pz,*_z;//momentum and coordinate grids

  double *_xMin,*_xMax;//xmin=p/pMax, xMax=p/(p+p0)
  double *_k;//residues of loss term
  double *_kern;//splitting kernel
  double *_col;//collision term due to branching
  double _pzMax, _dpz, _zMax,_dz;//UV cutoff
  double _t, _dt;//time and time step
  double _trelInv, _tbrTInv;//the inverse of relaxation time and branching time for T.
  double _pztot0, _etot0,_ngtot0;//total pz and gluon number at the initial time
  double _p0;// the lower cutoff for the splitting kernel
  int _n0;//The index for the lower cutoff for the splitting kernel

  fg(int n, double p0, double E, int nz, double dt, double t, double tr, double tb);
  double f(int z, double p);
  double f0(double z, double p){return 10.0*exp(-10.0*(p-_E)*(p-_E) - 10.0*z*z)/PI;}
  double kern(double x);

  double findMaxf(int z);//Find the maximal value of f at given z

  void split(int z);//collision term from 2<->3 process.
  void nextTime();//time integration to next time step
  void output();//output
  void outputtot();//output
  void calcJ();//calculate j;
  double pztot();//total pz
  double etot();//total energy
  double ngtot();//total ng
  double e(int z);//energy density at z
  double ejet();//energy for p>=p0
  double eloss();//energy loss in the medium
  double pz(int z);//pz at z
  double ng(int z);//ng at z
  inline int getzIndex(double z){return floor((z+_zMin)/_dz);}//return the index of the z array for given z

  ~fg();
};

//constructor and destructor
fg::fg(int n, double p0, double E, int nz, double dt, double t, double trel, double tbrT){
  _n=n; _E=E; _dt=dt;_t=t; _p0=p0; _nz=nz;
  _trelInv=_dt*trel;_tbrTInv=_dt*tbrT;

  //_zMin=sqrt(t)+3.0;_zMax=t+3.0;
  _zMin=10.0;_zMax=20.0;_pzMax=_E+5.0;_pzMin=10.0;

  _nm1=_n-1; _dpz=_pzMax/((double)_n);
  _nminus=(int) (_pzMin/_dpz)+1;_ntot=_n+_nminus;
  _dz=_zMax/((double) _nz); _nztot=(int) (_zMin/_dz)+_nz+1; 

  //cout << _nztot << " " << _ntot << endl;

  _f=new double*[_nztot]; _jp=new double*[_nztot+1];_jz=new double*[_nztot+1];
  for(int i=0;i<_nztot;i++){
    _f[i]=new double[_ntot];
  }
  for(int i=0;i<=_nztot;i++){
    _jp[i]=new double[_ntot+1];_jz[i]=new double[_ntot+1];
  }

  _pz=new double[_ntot];_z=new double[_nztot];
  _xMin=new double[_n];_xMax=new double[_n];
  _kern=new double[_nm1];_col=new double[_nm1];_k=new double[_nm1];

  //boundary condition
  for(int i=0;i<=_ntot;i++){
    _jz[0][i]=0.0;_jz[_nztot][i]=0.0;
  }

  for(int i=0;i<=_nztot;i++){
    _jp[i][0]=0.0;_jp[i][_ntot]=0.0;
  }

  //initialize p and z grids
  for(int i=0;i<_nztot;i++){
    _z[i]=-_zMin+i*_dz;
    for(int j=0;j<_ntot;j++){
      _pz[j]=-_pzMin+1.0e-8+j*_dpz;_f[i][j]=f0(_z[i],_pz[j]);
    }
  }

  for(int i=0;i<_n;i++){
    _xMin[i]=_pz[i+_nminus]/_pzMax;
    if(i<_nm1) _kern[i]=kern(_xMin[i]);
  }

  //set the lower cutoff
  _n0=_p0/_dpz;_p0=_n0*_dpz;_n0-=1;

  //cout << _n0 << endl;

  for(int i=0;i<_n;i++){
    int j=_nminus+i;
    _xMax[i]= _pz[j]/(_pz[j]+_p0);
  }
  //input k from k.dat
  ifstream in; in.open("k.dat");
  for(int i=0;i<_nm1;i++) in >> _k[i];
  in.close();

  //get global quantities
  _pztot0=pztot();_ngtot0=ngtot();_etot0=etot();


  //get output filename
  _ostr << "fpMax"  << setfill('0') << int(_pzMax);
  _ostr << "np"  << setfill('0') << _n;
  _ostr << "p0"  << setfill('0') << int(_p0);
  _ostr << "E"  << setfill('0') << int(_E);
  _ostr << "zMax"  << setfill('0') << int (_zMax);
  _ostr << "nz"  << setfill('0') << _nz;
  if(trel==0&&tbrT>0) _ostr << "br";
  if(trel>0&&tbrT==0) _ostr << "diff";
  _ostr << "v4.1.1";
}

fg::~fg(){
  for(int i=0;i<_nztot;i++){
    delete [] _f[i];
  }

  for(int i=0;i<=_nztot;i++){
    delete [] _jp[i];delete [] _jz[i];
  }

  delete [] _f; delete [] _jp;delete [] _jz;
  delete [] _pz; delete [] _z;
  delete [] _xMin; delete [] _xMax;
  delete [] _kern;delete [] _col;delete [] _k;
}

//Manipulation of f

double fg::f(int z, double pz){
  double value;
  if(pz<=_dpz) value=_f[z][_nminus];
  else if(pz>_pzMax) value=0;
  else{
    int index=floor(pz/_dpz)+_nminus-1;
    value=_f[z][index]+(_f[z][index+1]-_f[z][index])*(pz-_pz[index])/_dpz;
  }
  return value;
}

double fg::findMaxf(int z){
  double max=0;
  for(int p=_n0;p<_ntot;p++){
    int j=p+_nminus;
    if(max<_f[z][j]) max=_f[z][j];
  }
  return max;
}

//branching

double fg::kern(double x){
  return pow(1-x+x*x,2.5)/pow(x-x*x,1.5);
  //return 1.0/pow(x-x*x,1.5);
}

void fg::split(int z){
  for(int j=_n0;j<_nm1;j++){
    _col[j]=0;int index=floor(_xMax[j]/_xMin[0])-1;
    int pzj=j+_nminus;
    if(j<=index){
      for(int i=j;i<index;i++) _col[j]+=_kern[i]*f(z,_pz[pzj]/_xMin[i])/sqrt(_xMin[i]);
      _col[j]*=_xMin[0];_col[j]+=(_xMax[j]-_xMin[index])*_kern[index]*f(z,_pz[pzj]/_xMin[index])/sqrt(_xMin[index]);
    }
     _col[j]=(_col[j]-_k[j]*_f[z][pzj])/sqrt(_pz[pzj]);
  }
}

//diffusion and drag

void fg::calcJ(){
 for(int i=1;i<_nztot;i++){
    for(int j=1;j<_ntot;j++){
      double fs=0.5*(_f[i][j]+_f[i][j-1]),pzs=0.5*(_pz[j]+_pz[j-1]);
      if(pzs<0){
	_jp[i][j]=(_f[i][j]-_f[i][j-1])/_dpz-fs;
      }else{
	_jp[i][j]=(_f[i][j]-_f[i][j-1])/_dpz+fs;
      }
      _jz[i][j]=_f[i][j];
    }
  }
}

//go to the next time step

void fg::nextTime(){
  
  calcJ();
  //cout << "calcJ\n";

  if(_trelInv>0){
    for(int i=0;i<_nztot;i++){
      for(int j=0;j<_ntot;j++){
	if(_pz[j]>0&&i>0)
	  _f[i][j]+=_dt*( (_jp[i][j+1]-_jp[i][j])/_dpz-(_jz[i][j]-_jz[i-1][j])/_dz);
	else
	  _f[i][j]+=_dt*( (_jp[i][j+1]-_jp[i][j])/_dpz+(_jz[i+1][j]-_jz[i][j])/_dz);
      }
    }
  }
  //cout << "diffusion\n";  
  if(_tbrTInv>0){
    for(int z=0;z<_nztot;z++){
      if(findMaxf(z)>1.0e-5){
	//cout << "before split at " << z << endl;
	split(z);
	//cout << "z = " << z << "split\n";
	for(int i=_n0;i<_nm1;i++){
	  _f[z][i+_nminus]+=_tbrTInv*_col[i];
	}
      }
    }
  }
  //cout << "branch\n";
  _t+=_dt;
}

void fg::output(){
  ofstream out;out.precision(15);

  //output f
  out.open(string(_ostr.str()+".dat").c_str(),ios::app);
  out << "# t = " << _t << endl;    
  for(int i=0;i<_nztot;i++){
    for(int j=0;j<_ntot;j++){
      out << _z[i] << " " << _pz[j] << " " << _f[i][j] << endl;
    }
    out << endl;
  }
  out << "\n\n";
  out.close();
	
  //output density
  out.open(string(_ostr.str()+"den.dat").c_str(),ios::app);
  out << "# t = " << _t << endl;    
  for(int i=0;i<_nztot;i++){
    out << _z[i] << " " << ng(i)/_ngtot0 << " " << e(i)/_etot0 << " " << pz(i)/_etot0 << endl;
  }
  out << "\n\n";
  out.close();

}


void fg::outputtot(){
  ofstream out;out.precision(15);

  //output f
  //output total
  out.open(string(_ostr.str()+"tot.dat").c_str(),ios::app);
  out << _t << " " << ngtot()/_ngtot0 << " " << etot()/_etot0 << " " << pztot()/_etot0  << " " << ejet()/_etot0 << " " << eloss() << endl;
  out.close();

}

//Macroscopic quantities

double fg::pz(int z){
  double pz=0;
  for(int j=0;j<_ntot;j++){
    pz+=_pz[j]*_f[z][j];
  }
  return pz*_dpz;
}

double fg::e(int z){
  double pz=0;
  for(int j=0;j<_ntot;j++){
    pz+=fabs(_pz[j])*_f[z][j];
  }
  return pz*_dpz;
}

double fg::ng(int z){
  double ng=0;
  for(int j=0;j<_ntot;j++){
      ng+=_f[z][j];
  }
  return ng*_dpz;
}

double fg::etot(){
  double pz=0;
  for(int j=0;j<_ntot;j++){
    double pzx=0;
    for(int i=0;i<_nztot;i++) pzx+=_f[i][j];
    pz+=fabs(_pz[j])*pzx;
  }
  return pz*_dpz*_dz;
}

double fg::ejet(){
  double pz=0;
  for(int j=_nminus+_n0;j<_ntot;j++){
    double pzx=0;
    for(int i=0;i<_nztot;i++) pzx+=_f[i][j];
    pz+=_pz[j]*pzx;
  }
  return pz*_dpz*_dz;
}


double fg::eloss(){
  double pz=0;
  int z0=getzIndex(_t-1.0);
  //cout << "t = " << _t << ", z0 = " << -_zMin+z0*_dz << endl;
  //cout << z0 <<endl;
  for(int j=_nminus+_n0;j<_ntot;j++){
    double pzx=0;
    for(int i=z0;i<_nztot;i++) pzx+=_f[i][j];
    pz+=_pz[j]*pzx;
  }
  //cout << "done\n";
  return _E-pz*_dpz*_dz;
}


double fg::pztot(){
  double pz=0;
  for(int j=0;j<_ntot;j++){
    double pzx=0;
    for(int i=0;i<_nztot;i++) pzx+=_f[i][j];
    pz+=_pz[j]*pzx;
  }
  return pz*_dpz*_dz;
}

double fg::ngtot(){
  double ng=0;
  for(int i=0;i<_nztot;i++){
    for(int j=0;j<_ntot;j++){
      ng+=_f[i][j];
    }
  }
  return ng*_dpz*_dz;
}

