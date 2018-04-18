#include "ganita/metrics/GanitaMetrics.hpp"
  
// Driver program for GanitaMetrics. 

int main(int argc, char *argv[])
{
  GanitaMetrics gmetrics(0);
  GanitaMetricsVersion version;
  double ttee;                     // ttee is tracking error
  double tide;                     // tide is total inner divergence error

  if(argc < 3){
    std::cout<<"Usage: "<<argv[0]<<" reference-file system-file"<<std::endl;
    std::cout<<"Version: "<<version.returnVersion()<<endl;
    exit(1);
  }

  gmetrics.init_2(argv[1], argv[2]);
//   gmetrics.readTopAsOne(0);
//   gmetrics.readTopAsOne(1);
//   gmetrics.computeFrameStats();
  //gmetrics.closeBuffers();
  gmetrics.readTop(0);
  gmetrics.readTop(1);
  //gmetrics.setMajorResolution();
  //cout<<"Computed resolution ("<<gmetrics.returnMajorWidth()<<","
  //<<gmetrics.returnMajorHeight()<<")"<<endl;

  gmetrics.computeMeanTrackKL(0, ttee);
  tide = ttee;
  gmetrics.gmScores.push_back(ttee);
  gmetrics.gmScoreType.push_back("Inner div relative to reference");
  //cout<<"KL tracking error conditioned on reference ("<<ttee<<")"<<endl;
  gmetrics.computeMeanTrackKL(1, ttee);
  tide += ttee;
  gmetrics.gmScores.push_back(ttee);
  gmetrics.gmScoreType.push_back("Inner div relative to system   ");
  //cout<<"KL tracking error conditioned on system ("<<ttee<<")"<<endl;
  gmetrics.gmScores.push_back(tide);
  gmetrics.gmScoreType.push_back("Total inner div error          ");
  //cout<<"Total inner divergence error ("<<tide<<")"<<endl;

  gmetrics.processOuterDiv_3(0);
  gmetrics.processOuterDiv_3(1);

  gmetrics.printSummary();

  return(0);
}

