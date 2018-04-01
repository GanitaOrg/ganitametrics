// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "ganita/metrics/GanitaMetricsVersion.hpp"
#include "ganita/metrics/GanitaMetricsMat.hpp"

class GanitaMetrics
{
private:
  std::string name;
  std::string type;
  int verbosity;
public:
  GanitaMetrics(void);
  GanitaMetrics(int vv);                   // set verbosity=vv
  int readTHeader(ifstream &gzt_file);
  int readT(char *input_tran);
  int dumpTHeader(void);
  int init(char *input_seq);
};

