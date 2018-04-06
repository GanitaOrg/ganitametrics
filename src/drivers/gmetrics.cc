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

  gmetrics.init(argv[1], argv[2]);
  gmetrics.readTopReference();
  gmetrics.readTopSystem();
  gmetrics.visTracks();
  //gmetrics.printDetDenFrame();
}

