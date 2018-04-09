#include "ganita/metrics/GanitaMetrics.hpp"
  
// Driver program for GanitaMetrics. 

int main(int argc, char *argv[])
{
  GanitaMetrics gmetrics(1);
  GanitaMetricsVersion version;
  if(argc < 3){
    std::cout<<"Usage: "<<argv[0]<<" reference-file system-file"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  gmetrics.init_2(argv[1], argv[2]);
//   gmetrics.readTopReference();
//   gmetrics.readTopSystem();
//   gmetrics.visTracks();
  gmetrics.readTopAsOne(0);
  gmetrics.readTopAsOne(1);
  gmetrics.visTracks(1,0);
  //gmetrics.computeTrackKL(3);
  //gmetrics.printStartStop(1);
  //gmetrics.computeKL_DensityDistance_2(3);
  //gmetrics.printTrackStats(1);
//   gmetrics.computeFrameStats();
  //gmetrics.printTrackStats(1,2);
  //gmetrics.printDetDenFrame();
  //gmetrics.computeKL_DensityDistance(3);
  //gmetrics.printTrackStats();
}

