double getXEnergyFractions(double* , double* , vector<double> ,
                vector<double> , vector<double> , vector<double> , double *);
double getYEnergyFractions(double* , double* , vector<double> ,
                vector<double> , vector<double> , vector<double> , double *);
void getXYPositions(double *, double *, double *);


void fit_validation(vector<double> E_norm, vector<double> E, vector<double> x, vector<double> y,
                double cog_pos[2], const double args[3], int numbEvents)
{
  int numb_xCells = 65; int numb_yCells = 48; double cellsize = 38.3;
  double yPositions[48]; double yEnergyFractions[48]; double yEnergyError[48];
  double xPositions[65]; double xEnergyFractions[65]; double xEnergyError[65];



  double y_row = getXEnergyFractions(xEnergyFractions, xEnergyError, E_norm,E, x, y, cog_pos);
  double x_column = getYEnergyFractions(yEnergyFractions, yEnergyError, E_norm,E, x, y, cog_pos);
  getXYPositions(cog_pos, xPositions, yPositions);

  double totalEnergyDeposit;
  for(int j = 0; j < E.size(); j++)
  {
    totalEnergyDeposit += E[j]/numbEvents;
  }

  cout << totalEnergyDeposit << '\n';

  fstream fileX;
  fileX.open("fit_validationX.dat", ios::out);
  for(int i = 0; i < numb_xCells; i++)
  {
    fileX << xPositions[i] << " " << xEnergyFractions[i] << " "
          << xEnergyError[i]/totalEnergyDeposit << " "
          << endl;
  }
  fileX.close();

  fstream fileXfunc;
  fileXfunc.open("fit_validationXfunc.dat", ios::out);
  for(int i = -20000; i < 20000; i++)
  {
    fileXfunc << i*0.05 - cog_pos[0] << " "
          << energyDeposition((i*0.05), (-900.05 + y_row*cellsize),cog_pos,args)
          << endl;
  }
  fileXfunc.close();

  fstream fileY;
  fileY.open("fit_validationY.dat", ios::out);
  for(int i = 0; i < numb_yCells; i++)
  {
    fileY << yPositions[i] << " " << yEnergyFractions[i] << " "
          << yEnergyError[i]/totalEnergyDeposit << " "
          << endl;
  }
  fileY.close();

  fstream fileYfunc;
  fileYfunc.open("fit_validationYfunc.dat", ios::out);
  for(int i = -20000; i < 20000; i++)
  {
    fileYfunc << i*0.05 - cog_pos[1] << " "
          << energyDeposition((-1206.45 + x_column*cellsize), (i*0.05),cog_pos,args)
          << endl;
  }
  fileYfunc.close();

}


void getXYPositions(double cog_pos[2], double xPositions[65], double yPositions[48])
{
  double cellsize = 38.3;
  int numb_xCells = 65; int numb_yCells = 48;
  for(int i = 0; i < numb_xCells; i++)
  {
    xPositions[i] = (i * cellsize - 1206.45)  - cog_pos[0];
  }
  for(int i = 0; i < numb_yCells; i++)
  {
    yPositions[i] = (i * cellsize - 900.05)  - cog_pos[1];
  }
}

double getYEnergyFractions(double* yEnergyFractions, double* yEnergyError, vector<double> E_norm,
                vector<double> E, vector<double> x, vector<double> y, double cog_pos[2])
{
  double cellsize = 38.3;
  int numb_xCells = 65; int numb_yCells = 48;

  int x_column = (cog_pos[0] - (-1206.45 - 19.15)) / cellsize;
  double x_pos = x_column * cellsize - 1206.45;

  cout << x_column << " " << x_pos << '\n';

  for(int i = 0; i < numb_yCells; i++)
  {yEnergyFractions[i] = 0; yEnergyError[i] = 0;}
  for(int i = 0; i < numb_yCells; i++)
  {
    int zaehler = 0;
    for(int j = 0; j < x.size(); j++)
    {
      if(fabs(x[j] - x_pos) < 2.5 && fabs((- 900.05 + i*cellsize) - y[j]) < 2.5)
      {
        yEnergyFractions[i] += E_norm[j];
        yEnergyError[i] += sigmaE(E[j]);
        zaehler++;
      }
    }
    if(zaehler != 0){
    yEnergyFractions[i] = yEnergyFractions[i] / zaehler;
    yEnergyError[i] = yEnergyError[i] / zaehler;
    }
  }
  return x_column;
}


double getXEnergyFractions(double* xEnergyFractions, double* xEnergyError, vector<double> E_norm,
                vector<double> E, vector<double> x, vector<double> y, double cog_pos[2])
{
  double cellsize = 38.3;
  int numb_xCells = 65; int numb_yCells = 48;


  int y_row = (cog_pos[1] - (-900.05 - 19.15)) / cellsize;
  double y_pos = y_row * cellsize - 900.05;



  for(int i = 0; i < numb_xCells; i++)
  {xEnergyFractions[i] = 0; xEnergyError[i] = 0;}

  for(int i = 0; i < numb_xCells; i++)
  {
    int zaehler = 0;
    for(int j = 0; j < x.size(); j++)
    {
      if(fabs(y[j] - y_pos) < 2.5 && fabs((-1206.45 + i*cellsize) - x[j]) < 2.5)
      {
        //cout << E_norm[j] << '\n';
        xEnergyFractions[i] += E_norm[j];
        xEnergyError[i] += sigmaE(E[j]);
        zaehler++;
      }
    }
    if(zaehler != 0){
    xEnergyFractions[i] = xEnergyFractions[i] / zaehler;
    xEnergyError[i] = xEnergyError[i] / zaehler;
    }
  }
  return y_row;
}
