#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "functions.h"


using namespace std;

int main(){
    vector <double> E;
    vector <double> x;
    vector <double> y;
    int counter = 0;

    double E_storage;
    double x_storage;
    double y_storage;
    ifstream file;  // Datei-Handle
    string line;
    file.open("E_x_y_MCdata_GeV_mm.dat", ios::in); // Öffne Datei aus Parameter
    while (!file.eof())          // Solange noch Daten vorliegen
    {
        getline(file, line);        // Lese eine Zeile
        //cout << line << endl;    // Zeige sie auf dem Bildschirm
        if(line.compare("----") != 0)
        {
          istringstream parser(line);
          parser >> E_storage >> x_storage >> y_storage ;
          E.push_back(E_storage);
          x.push_back(x_storage);
          y.push_back(y_storage);
          //cout << E[counter] << x[counter] << y[counter] << '\n';
          counter++;
        }
    }
    file.close();                // Datei wieder schließen



    double xy_CoG[2];
    calculateCentreOfGrav(E,x,y,xy_CoG);

    cout << xy_CoG[0] << " " << xy_CoG[1] << '\n';



    double args[4] = {1.09,10.52,-0.09,97.1233};
    cout << energyDeposition(xy_CoG[0],xy_CoG[1], xy_CoG ,args) << '\n';
    cout << cdf(200,200,args) << '\n';

    cout << chisquare(E,x,y, xy_CoG, args) << '\n';








  return 0;
}
