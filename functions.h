#include <math.h>


double sigmaE(double energy) {
  double c = 0.02;           // constant Term
  double s = 0.06;           // stochastic term

  double sigmaE = energy * sqrt(pow(c, 2) + s / sqrt(energy));
  return sigmaE;
}



double cdf(double x, double y, double args[4]) {
  double pi = 3.14159265359;
  double a1 = args[0];
  double b1 = args[1];
  double a2 = args[2];
  double b2 = args[3];


  double result = 1.0 / 4.0 +
              1.0 / (2 * pi) *
              (a1 * (atan(x / b1) + atan(y / b1) +
                     atan(x * y / (b1 * sqrt(b1 * b1 + x * x + y * y)))) +
               (a2 * (atan(x / b2) + atan(y / b2) +
                      atan(x * y / (b2 * sqrt(b2 * b2 + x * x + y * y))))));
  return result;
}
