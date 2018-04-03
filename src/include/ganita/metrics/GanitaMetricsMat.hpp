#ifndef _GANITAMETRICSMAT_
#define _GANITAMETRICSMAT_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <cmath>
#include <memory>
//#include <boost/shared_ptr.hpp>

#ifdef __APPLE__
#include <bsd/stdlib.h>
#endif

using namespace std;

class GanitaMetricsMat
{
private:
  unsigned long ncols;
  unsigned long nrows;
  double **gmmat;
public:
  GanitaMetricsMat(void);
  GanitaMetricsMat(unsigned long nn, unsigned long mm);
  int close(void);
  unsigned long returnNCols(void);
  unsigned long returnNRows(void);
  int set(unsigned long row, unsigned long column, double value);
  double get(unsigned long row, unsigned long column);
  int leftMultiply(GanitaMetricsMat mymat);
  int leftMultiply(GanitaMetricsMat mymat, GanitaMetricsMat result);
  int square(void);
  int dumpMat(void);
  int setEx1(void);
  int init(unsigned long nn, unsigned long mm);
  int setUnit(unsigned long ii);
};

#endif
