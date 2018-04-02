// This file contains the methods for GanitaMetrics. 

#include "ganita/metrics/GanitaMetrics.hpp"

// Constructors
GanitaMetrics::GanitaMetrics(void)
{
  verbosity = 0;
}

GanitaMetrics::GanitaMetrics(int vv)
{
  verbosity = vv;
}

int GanitaMetrics::init(char *ref_input, char *sys_input)
{
  gmr = new GanitaBuffer();
  gmr->open(ref_input);
  cout<<"Reference file size = "<<gmr->size()<<endl;

  gms = new GanitaBuffer();
  gms->open(sys_input);

  return(1);
}

int GanitaMetrics::readMotReference(void)
{
  int64_t new_id;
  int64_t new_frame_number;
  double new_x_anchor;
  double new_y_anchor;
  double new_width;
  double new_height;
  double new_confidence;
  double new_x_world;
  double new_y_world;
  double new_z_world;
  int new_verbosity;
  uint64_t num;
  GanitaMetricsDetection gmd;

  addTrack();
  
  string line("");
  while(gmr->getLine(line) >= 0){
    cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d",  
	   &new_id, &new_frame_number, &new_x_anchor, &new_y_anchor, &new_width, &new_height, 
	   &new_confidence, &new_x_world, &new_y_world, &new_z_world, &new_verbosity);
 
    num = gmtracks[0]->addDetection(new_id, new_frame_number, new_x_anchor, new_y_anchor, new_width, new_height, 
			      new_confidence, new_x_world, new_y_world, new_z_world, new_verbosity);
    gmtracks[0]->returnGMD(num - 1, gmd);
    cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    line = "";
  }
  return(1);
}

int64_t GanitaMetrics::addTrack(void)
{
  GanitaMetricsTrack *newtrack = new GanitaMetricsTrack();
  gmtracks.push_back(std::make_shared<GanitaMetricsTrack>(*newtrack));
  delete newtrack;
  return(gmtracks.size());
}

int GanitaMetrics::dumpTHeader(void)
{
  GanitaMetricsVersion gzv;
  fprintf(stdout, "GanitaMetrics version \t %s\n", gzv.returnVersion().c_str());

  return (1);
}

int GanitaMetrics::init(char *input_seq)
{
  // Read in the input transformation. 
  unsigned long tmp;
  //std::ifstream sym_file(input_seq,std::ifstream::binary);
  // std::ifstream sym_file(input_seq);
  // if (!sym_file.is_open()){
  //   std::cout<<"Unable to open input file: "<<input_seq<<std::endl;
  //   return(0);
  // }
  
//   tmp = gSym.init(input_seq);
//   if(verbosity){
//     gSym.dumpAlphabet();
//   }

  return(tmp);
}

