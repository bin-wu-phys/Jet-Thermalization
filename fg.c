#include <iostream>
#include <iomanip>
using namespace std;

#include "fg.h"

int main(){
  fg fg01(600,1.0, 25.0,400, 1.0e-3,0.0,1.0,1.0);
  cout.precision(15);
  double tMax=50,t=0;
  double tSave = 0.1;//0.005039302255187421;		// time for save: corresponds to Francois' Delta t = 2000. 
  unsigned int nSave = (unsigned int)(tSave/fg01._dt), ns = nSave;//time to save the data

  do{
    //check whether to save or not
    if(ns==nSave){
      fg01.output();
      //cout << fg01._t << "   " << 1.0-fg01.pztot()/fg01._pztot0 << endl;
      ns=0;
    }
    fg01.nextTime();
    ns++;fg01.outputtot();
    //cout << fg01._t << endl;
  }while(fg01._t<=tMax);
							
  return 0;
}
