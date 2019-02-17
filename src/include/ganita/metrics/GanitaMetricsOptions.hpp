#ifndef _GANITAMETRICSOPTIONS_
#define _GANITAMETRICSOPTIONS_

// T. M. Adams (ganita.org)
// This class is used to parse command-line arguments. 
// It is fairly simple to implement this directly rather than 
// rely on boost or getOpt.
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>

#include "ganita/metrics/GanitaMetricsVersion.hpp"

class GanitaMetricsOptions
{
private:
  int verbosity;     // set to 0, 1, or 2
  int vis_flag;      // set to 0, 1, or 2
  int file_flag;     // set to 0, 1, or 2
  int compare_flag;  // set to 0 or 1
  int help_flag;     // set to 0 or 1
  int res_flag;      // set to 0, 1 or 2
  int detailed_flag; // set to 0 or 1
  uint64_t res_x;    // width
  uint64_t res_y;    // height
  std::vector<std::string> fnames;
  std::string vname;
public:
  GanitaMetricsOptions(void);
  int getOptions(int argc, char* argv[]);
  int outputOptions(char* argv[]);
  int returnVerbosity(void);
  int returnVisFlag(void);
  int returnFileFlag(void);
  int returnCompareFlag(void);
  int returnHelpFlag(void);
  int returnResFlag(void);
  int returnDetailedFlag(void);
  uint64_t returnResX(void);
  uint64_t returnResY(void);
  std::string returnFileName(uint32_t ii);
  std::string returnVideoName(void);
};

#endif
