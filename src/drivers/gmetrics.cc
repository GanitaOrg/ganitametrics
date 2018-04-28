#include "ganita/metrics/GanitaMetrics.hpp"
  
// Driver program for GanitaMetrics. 

int main(int argc, char *argv[])
{
  GanitaMetrics gmetrics(0);
  GanitaMetricsOptions gmoptions;
  double ttee;                     // ttee is tracking error
  double tide;                     // tide is total inner divergence error

  if(gmoptions.getOptions(argc, argv) < 2){
    exit(1);
  }

  if(gmetrics.init(gmoptions) < 1){
    std::cerr<<"Unable to open one of the input files"<<std::endl;
    std::cerr<<"Rerun in verbose mode for more details"<<std::endl;
    exit(1);
  }
//   gmetrics.readTopAsOne(0);
//   gmetrics.readTopAsOne(1);
//   gmetrics.computeFrameStats();
  //gmetrics.closeBuffers();
  gmetrics.readTop(0);
  gmetrics.readTop(1);

  gmetrics.computeMeanTrackKL(0, ttee);
  tide = ttee;
  gmetrics.gmScores.push_back(ttee);
  gmetrics.gmScoreType.push_back("Inner div relative to reference");
  gmetrics.computeMeanTrackKL(1, ttee);
  tide += ttee;
  gmetrics.gmScores.push_back(ttee);
  gmetrics.gmScoreType.push_back("Inner div relative to system   ");
  gmetrics.gmScores.push_back(tide);
  gmetrics.gmScoreType.push_back("Total inner div error          ");

  gmetrics.processOuterDiv_3(0);
  gmetrics.processOuterDiv_3(1);

  gmetrics.printSummary();

  return(0);
}

