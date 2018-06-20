#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/Minimizer.h"
#include "Math/MinimizerOptions.h"
#include "fit_functions.h"
#include "fitvalidation.h"

int degreesOfFreedom(vector <double> , int);


using namespace std;

int main(){
    vector <double> E; vector <double> E_norm; vector <double> x; vector <double> y;
    int counter = 0;
    int numbEvents = 0;

    double E_storage; double E_norm_storage; double x_storage; double y_storage;
    ifstream file;  // Datei-Handle
    string line;
    file.open("E_x_y_MCdata_GeV_mm.dat", ios::in); // Öffne Datei aus Parameter
    while (!file.eof())          // Solange noch Daten vorliegen
    {
        getline(file, line);        // Lese eine Zeile
        //cout << line << endl;    // Zeige sie auf dem Bildschirm
        if(line.compare("----") == 0)
        {numbEvents++;}
        if(line.compare("----") != 0)
        {
          istringstream parser(line);
          parser >> E_norm_storage >> E_storage >> x_storage >> y_storage ;
          E_norm.push_back(E_norm_storage);
          E.push_back(E_storage);
          x.push_back(x_storage);
          y.push_back(y_storage);
          //cout << E_norm[counter] << E[counter] << x[counter] << y[counter] << '\n';
          counter++;
        }
    }
    file.close();                // Datei wieder schließen



    double totalEnergyDeposit;
    for(int j = 0; j < E.size(); j++)
    {
      totalEnergyDeposit += E[j]/numbEvents;
    }



    double xy_CoG[2];
    calculateCentreOfGrav(E,x,y,xy_CoG);


    function<double(const double *)> chisquare_data =
                      chisquare(totalEnergyDeposit, E, x, y, xy_CoG);

    function<double(const double *)> chisquare_result = chisquare_output(chisquare_data);


    const int numbOfArguments = 5;
    ROOT::Math::Minimizer *min =
        ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
    min->SetMaxFunctionCalls(10000); // if one wants a max calls default=420?
    min->SetTolerance(0.001);
    min->SetPrintLevel(2);
    ROOT::Math::Functor f =
        ROOT::Math::Functor(chisquare_result, numbOfArguments); // function of type double
    min->SetFunction(f);
    min->SetVariable(0, "a1", 1.01, 1e-5);
    min->SetVariable(1, "b1", 3.0, 1e-5);
    min->SetVariable(2, "a2", -0.05, 1e-5);
    min->SetVariable(3, "b2", +2.0, 1e-5);
    min->SetVariable(4, "b3", +200.0, 1e-5);
    min->Minimize();

    const double args[numbOfArguments] = {min->X()[0],min->X()[1],min->X()[2],min->X()[3],min->X()[4]};


    fit_validation(E,x,y,xy_CoG,args, numbEvents);
    cout << xy_CoG[0] << " " << xy_CoG[1] << '\n';

    cout << "number of degrees of freedom (ndf): " << degreesOfFreedom(E, numbOfArguments) << '\n';
    //cout << chisquare_old(totalEnergyDeposit, E,x,y, xy_CoG, args) << '\n';
    cout << "chi2/ndf: " << chisquare_old(totalEnergyDeposit, E,x,y, xy_CoG, args)/degreesOfFreedom(E, numbOfArguments) << '\n';


  return 0;
}


int degreesOfFreedom(vector <double> E, int numbOfArguments)
{
  int degreesOfFreedom = 0;
  for(int i = 0; i < E.size(); i++)
  {
    degreesOfFreedom++;
  }
  degreesOfFreedom -= numbOfArguments;

  return degreesOfFreedom;
}
