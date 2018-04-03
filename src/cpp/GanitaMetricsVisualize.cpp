#include "ganita/metrics/GanitaMetricsVisualize.hpp"

// Constructors
GanitaMetricsVisualize::GanitaMetricsVisualize(void)
{
  video_name = "";
  output_name = "";
  verbosity = 0;
}

int GanitaMetricsVisualize::init(void)
{
  //gmvo = new GanitaBuffer();
  //gmvo.openOut(output_name.c_str());

  return(1);
}

int GanitaMetricsVisualize::ffmpegBox(uint64_t ss, uint64_t ee, int xx, int yy, int ww, int hh)
{
  std::cout<<"drawbox=enable=\'between(n,"<<ss<<","<<ee<<")\' : x="<<xx<<" : y="<<yy
	   <<" : w="<<ww<<" : h="<<hh<<" : color=red,\\"
	   <<std::endl;

  return(1);
}

