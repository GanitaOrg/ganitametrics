#ifndef _GANITAMETRICSVISUALIZE_
#define _GANITAMETRICSVISUALIZE_

// This class implements algorithms that 
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

class GanitaMetricsVisualize
{
private:
  std::string video_name;
  std::string output_name;
  int verbosity;

public:
  GanitaMetricsVisualize(void);
  int init(void);
  int ffmpegBox(uint64_t ss, uint64_t ee, int xx, int yy, 
		int ww, int hh);
};

#endif
