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

function<double(const double *)> chisquare(vector<double> E_norm, vector<double> E,
                vector<double> x, vector<double> y, double cog_pos[2])
{
  function<double(const double *)> out = [E_norm, E, x, y, cog_pos](const double * args)
                                                -> double {
    double E_calc_norm;
    double chisquare = 0;


    for(uint i = 0; i < E.size(); i++)
    {
        E_calc_norm = energyDeposition(x[i],y[i],cog_pos,args);
        chisquare += pow((E_norm[i] - E_calc_norm),2)/pow(sigmaE(E[i]),2);
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







double sigmaE(double energy)//aus Fig.49 compassSetupForPhysicsWithHadronBeams, ohne 2ten term
 {
  double c1 = 0.055;
  double c2 = 0.015;

  double sigmaE = c1 * sqrt(energy) + c2 * energy;
  return sigmaE;
}


//nimmt Koordinaten im CoG Frame an
  double cdf(double x, double y, const double args[3])
  {
  double pi = 3.14159265359;
  double a1 = args[0];
  double b1 = args[1];
  double a2 = 1.0 - a1;
  double b2 = args[2];


  double result = 1.0 / 4.0 +
              1.0 / (2 * pi) *
              (a1 * (atan(x / b1) + atan(y / b1) +
                     atan(x * y / (b1 * sqrt(b1 * b1 + x * x + y * y)))) +
               (a2 * (atan(x / b2) + atan(y / b2) +
                      atan(x * y / (b2 * sqrt(b2 * b2 + x * x + y * y))))));
  return result;
}

//gibt EnergieDeponierung in Zelle zurück, nimmt normale Koordinaten an
double energyDeposition(double x_cell, double y_cell, double cog_pos[2], const double args[3])
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


double chisquare_old(vector<double> E_norm, vector<double> E, vector<double> x, vector<double> y,
                double cog_pos[2], double args[3])
{
  double E_calc_norm;
  double chisquare = 0;


  for(uint i = 0; i < E.size(); i++)
  {
      E_calc_norm = energyDeposition(x[i],y[i],cog_pos,args);
      chisquare += pow((E_norm[i] - E_calc_norm),2)/pow(sigmaE(E[i]),2);
  }

  return chisquare;
}
