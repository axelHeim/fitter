#include <math.h>

using namespace std;

double sigmaE(double);
double cdf(double, double, const double *);
double energyDeposition(double, double, double *, const double *);

function<double(const double *)> chisquare_output(function <double(const double *)> chifunc)
{
  function<double(const double *)> out = [chifunc](const double * args) -> double
          {return chifunc(args);};
  return out;
}

function<double(const double *)> chisquare(double totalEnergyDeposit, vector<double> E,
                vector<double> x, vector<double> y, double cog_pos[2])
{
  function<double(const double *)> out = [totalEnergyDeposit, E, x, y, cog_pos](const double * args)
                                                -> double {
  double E_calc_norm;
  double chisquare = 0;


  for(uint i = 0; i < E.size(); i++)
  {
      E_calc_norm = energyDeposition(x[i],y[i],cog_pos,args);
      chisquare += pow((E[i] - E_calc_norm*totalEnergyDeposit),2)/pow(sigmaE(E[i]),2);
  }

  return chisquare;
  };




  return out;
}




void calculateCentreOfGrav(vector<double> E, vector<double> x, vector<double> y, double cog_pos[2])
{
  double gewichteterOrt = 0;
  double totalEnergy = 0;

  for(uint i = 0; i< E.size(); i++)
  {
    totalEnergy += E[i];
  }


  for(uint i = 0; i< E.size(); i++)
  {
    gewichteterOrt += E[i]*x[i];
  }
  cog_pos[0] = gewichteterOrt / totalEnergy;

  gewichteterOrt = 0;

  for(uint i = 0; i< E.size(); i++)
  {
    gewichteterOrt += E[i]*y[i];
  }
  cog_pos[1] = gewichteterOrt / totalEnergy;

}







double sigmaE(double E)//from coral option file
 {
  double c1 = 0.15;
  double c2 = 0.015;
  double c3 = 0.05;


  double sigmaE = sqrt(c1*c1*E + c2*c2*E*E + c3*c3);
  return sigmaE;
}


//nimmt Koordinaten im CoG Frame an
  double cdf(double x, double y, const double args[5])
  {
  double pi = 3.14159265359;
  double a1 = args[0];
  double b1 = args[1];
  double a2 = args[2];
  double b2 = args[3];
  double a3 = 1.0 - a1 - a2;
  double b3 = args[4];




  double result = 1.0 / 4.0 +
              1.0 / (2 * pi) *
              (a1 * (atan(x / b1) + atan(y / b1) +
                     atan(x * y / (b1 * sqrt(b1 * b1 + x * x + y * y)))) +
               (a2 * (atan(x / b2) + atan(y / b2) +
                      atan(x * y / (b2 * sqrt(b2 * b2 + x * x + y * y))))) +
              (a3 * (atan(x / b3) + atan(y / b3) +
                             atan(x * y / (b3 * sqrt(b3 * b3 + x * x + y * y))))));
  return result;
}

//gibt EnergieDeponierung in Zelle zurück, nimmt normale Koordinaten an
double energyDeposition(double x_cell, double y_cell, double cog_pos[2], const double args[5])
{
  double cellsize = 38.3;
  double halfCell = cellsize / 2.0;
  double x_0 = x_cell - cog_pos[0];
  double y_0 = y_cell - cog_pos[1];       //position in CoG frame

  double energyDeposit = cdf(x_0 + halfCell, y_0 + halfCell, args) +
                        cdf(x_0 - halfCell, y_0 - halfCell, args)  -
                        cdf(x_0 - halfCell, y_0 + halfCell, args)  -
                        cdf(x_0 + halfCell, y_0 - halfCell, args);

  return energyDeposit;
}


double chisquare_old(double totalEnergyDeposit, vector<double> E, vector<double> x, vector<double> y,
                double cog_pos[2],const double args[5])
{
  double E_calc_norm;
  double chisquare = 0;


  for(uint i = 0; i < E.size(); i++)
  {
      E_calc_norm = energyDeposition(x[i],y[i],cog_pos,args);
      chisquare += pow((E[i] - E_calc_norm*totalEnergyDeposit),2)/pow(sigmaE(E[i]),2);
  }

  return chisquare;
}

double chisquare_cell(double totalEnergyDeposit, double E, double x, double y,
                double cog_pos[2],const double args[5])
{
  double E_calc_norm;
  double chisquare = 0;


  E_calc_norm = energyDeposition(x,y,cog_pos,args);
  chisquare += pow((E - E_calc_norm*totalEnergyDeposit),2)/pow(sigmaE(E),2);

  return chisquare;
}
