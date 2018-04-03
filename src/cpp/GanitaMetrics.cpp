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
  GanitaMetricsMotDetection gmd;

  addTrack();
  
  string line("");
  while(gmr->getLine(line) >= 0){
    cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d",  
	   &new_id, &new_frame_number, &new_x_anchor, &new_y_anchor, &new_width, &new_height, 
	   &new_confidence, &new_x_world, &new_y_world, &new_z_world, &new_verbosity);
 
    num = gmtracks[0]->addMotDetection(new_id, new_frame_number, new_x_anchor, new_y_anchor, new_width, new_height, 
			      new_confidence, new_x_world, new_y_world, new_z_world, new_verbosity);
    gmtracks[0]->returnMotGMD(num - 1, gmd);
    cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    line = "";
  }
  return(1);
}

int GanitaMetrics::readTopReference(void)
{
  int64_t new_id;
  int64_t new_frame_number;
  int new_headValid;
  int new_bodyValid;
  double new_headLeft;
  double new_headTop;
  double new_headRight;
  double new_headBottom;
  double new_bodyLeft;
  double new_bodyTop;
  double new_bodyRight;
  double new_bodyBottom;
  double new_confidence;
  int new_verbosity;
  uint64_t num;
  GanitaMetricsTopDetection gmd;
  
  addTrack();
  
  string line("");
  while(gmr->getLine(line) >= 0){
    //cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d",  
	   &new_id, &new_frame_number, &new_headValid, &new_bodyValid, 
	   &new_headLeft, &new_headTop, &new_headRight, &new_headBottom,
	   &new_bodyLeft, &new_bodyTop, &new_bodyRight, &new_bodyBottom, 
	   &new_confidence, &new_verbosity);
 
    num = gmtracks[0]->addTopDetection(new_id, new_frame_number, new_headValid, new_bodyValid, 
				       new_headLeft, new_headTop, new_headRight, new_headBottom,
				       new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom, 
				       new_confidence, new_verbosity);
    gmtracks[0]->returnTopGMD(num - 1, gmd);
    //cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
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
  tmp = 0;
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

int64_t GanitaMetrics::addVis(void)
{
  GanitaMetricsVisualize *newvis = new GanitaMetricsVisualize();
  gmvis.push_back(std::make_shared<GanitaMetricsVisualize>(*newvis));
  delete newvis;
  return(gmvis.size());
}

int GanitaMetrics::visTracks(void)
{
  uint64_t ii, jj, num;
  uint64_t nframes[2];
  GanitaMetricsTopDetection gmd;
  char mystr[101];
  uint64_t final_frame;
  double start_time;
  ofstream gmvo;

  gmvo.open("ganita_metrics_vis.sh");
  
  jj = 0; ii = 0;
  num = gmtracks[0]->returnNumberOfTopDetections();
  gmtracks[0]->returnTopGMD(num-1, gmd);
  final_frame = gmd.returnFrameNumber();
  addVis();
  if(num > 1600){
    gmtracks[0]->returnTopGMD(1600, gmd);
    nframes[1] = gmd.returnFrameNumber();
  }
  nframes[0] = 0;
  gmvo<<"#!/bin/bash"<<endl;
  gmvo<<"ffmpeg -i /diva/DIVA/DATA/Tracking/TownCentre/TownCentreXVID.avi -vframes "<<nframes[1]<<" -vf \\"<<endl;
  gmvo<<"\"";
  while(ii<num){
    gmtracks[0]->returnTopGMD(ii, gmd);
    //gmvo<<"Frame number = "<<gmd.returnFrameNumber()<<endl;
    //gmvis[0]->ffmpegBox(gmd.returnFrameNumber() - nframes[0] + 1, gmd.returnFrameNumber() - nframes[0] + 1,
    //		(uint64_t) gmd.returnX_Anchor(), (uint64_t) gmd.returnY_Anchor(),
    //		(uint64_t) gmd.returnWidth(), (uint64_t) gmd.returnHeight());
    gmvo<<"drawbox=enable=\'between(n,"<<gmd.returnFrameNumber() - nframes[0] + 1<<","<<gmd.returnFrameNumber() - nframes[0] + 1
	<<")\' : x="<<(uint64_t) gmd.returnX_Anchor()<<" : y="<<(uint64_t) gmd.returnY_Anchor()
	<<" : w="<<(uint64_t) gmd.returnWidth()<<" : h="<<(uint64_t) gmd.returnHeight()<<" : color=red,\\"
	<<std::endl;
    ii++;
    if(ii % 1600 == 0){
      // end and start another clip with ffmpeg
      sprintf(mystr, "ganita_video_%05ld.avi", jj);
      gmvo<<"drawbox=enable='between(n,1,1)' : x=0 : y=0 : w=0 : h=0 : color=red\" "<<mystr<<endl;
      jj++;
      nframes[0] = nframes[1];
      if(ii + 1600 < num){
	gmtracks[0]->returnTopGMD(ii+1600, gmd);
	nframes[1] = gmd.returnFrameNumber();
      }
      else nframes[1] = final_frame;
      start_time = ((double) nframes[0]) / 24.97;
      //gmvo<<"ffmpeg -i ../data/duke_tracker_output/TownCentre/TownCentreXVID.avi -vf \\"<<endl;
      gmvo<<"ffmpeg -ss "<<start_time<<" -i /diva/DIVA/DATA/Tracking/TownCentre/TownCentreXVID.avi -vframes "
	  <<nframes[1] - nframes[0]<<" -vf \\"<<endl;
      gmvo<<"\"";
    }
  }
  sprintf(mystr, "ganita_video_%05ld.avi", jj);
  gmvo<<"drawbox=enable='between(n,1,1)' : x=0 : y=0 : w=0 : h=0 : color=red\" "<<mystr<<endl;
  gmvo<<"for f in ./ganita_video_*.avi; do echo \"file '$f'\" >> gv_list.txt; done"<<endl;
  gmvo<<"ffmpeg -f concat -safe 0 -i gv_list.txt -c copy ganita_vis.avi"<<endl;

  gmvo.close();

  return(1);
}

