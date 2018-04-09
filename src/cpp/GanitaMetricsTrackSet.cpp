// This file contains the methods for GanitaMetricsTrackSet. 

#include "ganita/metrics/GanitaMetricsTrackSet.hpp"

// Constructors
GanitaMetricsTrackSet::GanitaMetricsTrackSet(void)
{
  id = 0;
  track_set_start = 0;
  track_set_end = 0;
  verbosity = 0;
}

int64_t GanitaMetricsTrackSet::setStart(int64_t ss)
{
  track_set_start = ss;

  return(track_set_start);
}

int64_t GanitaMetricsTrackSet::setEnd(int64_t ee)
{
  track_set_end = ee;

  return(track_set_end);
}

int GanitaMetricsTrackSet::init(char *buf_input)
{
  gmb = new GanitaBuffer();
  gmb->open(buf_input);
  cout<<"File "<<buf_input<<" size = "<<gmb->size()<<endl;

  return(1);
}

int GanitaMetricsTrackSet::readTop(void)
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
  int64_t min_frame, max_frame;

  min_frame = 1<<31;
  max_frame = -1;
  
  addTrack();
  
  string line("");
  while(gmb->getLine(line) >= 0){
    //cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d",  
	   &new_id, &new_frame_number, &new_headValid, &new_bodyValid, 
	   &new_headLeft, &new_headTop, &new_headRight, &new_headBottom,
	   &new_bodyLeft, &new_bodyTop, &new_bodyRight, &new_bodyBottom, 
	   &new_confidence, &new_verbosity);

    if(new_frame_number < min_frame){
      min_frame = new_frame_number;
    }
    if(new_frame_number > max_frame){
      max_frame = new_frame_number;
    }
 
    num = gmTracks[0]->addTopDetection(new_id, new_frame_number, new_headValid, new_bodyValid, 
				       new_headLeft, new_headTop, new_headRight, new_headBottom,
				       new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom, 
				       new_confidence, new_verbosity);
    gmTracks[0]->returnTopGMD(num - 1, gmd);
    //cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    line = "";
  }

  gmTracks[0]->setStart(min_frame);
  gmTracks[0]->setEnd(max_frame);

  return(1);
}

int64_t GanitaMetricsTrackSet::addTrack(void)
{
  GanitaMetricsTrack *newtrack = new GanitaMetricsTrack();
  gmTracks.push_back(std::make_shared<GanitaMetricsTrack>(*newtrack));
  delete newtrack;
  return(gmTracks.size());
}

int64_t GanitaMetricsTrackSet::addVis(void)
{
  GanitaMetricsVisualize *newvis = new GanitaMetricsVisualize();
  gmvis.push_back(std::make_shared<GanitaMetricsVisualize>(*newvis));
  delete newvis;
  return(gmvis.size());
}

int GanitaMetricsTrackSet::visTracks(void)
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
  num = gmTracks[0]->returnNumberOfTopDetections();
  gmTracks[0]->returnTopGMD(num-1, gmd);
  final_frame = gmd.returnFrameNumber();
  addVis();
  if(num > 1600){
    gmTracks[0]->returnTopGMD(1600, gmd);
    nframes[1] = gmd.returnFrameNumber();
  }
  nframes[0] = 0;
  gmvo<<"#!/bin/bash"<<endl;
  gmvo<<"ffmpeg -i /diva/DIVA/DATA/Tracking/TownCentre/TownCentreXVID.avi -vframes "<<nframes[1]<<" -vf \\"<<endl;
  gmvo<<"\"";
  while(ii<num){
    gmTracks[0]->returnTopGMD(ii, gmd);
    //gmvo<<"Frame number = "<<gmd.returnFrameNumber()<<endl;
    //gmvis[0]->ffmpegBox(gmd.returnFrameNumber() - nframes[0] + 1, gmd.returnFrameNumber() - nframes[0] + 1,
    //		(uint64_t) std::max(gmd.returnX_Anchor(), 0), (uint64_t) std::max(gmd.returnY_Anchor(), 0),
    //		(uint64_t) gmd.returnWidth(), (uint64_t) gmd.returnHeight());
    gmvo<<"drawbox=enable=\'between(n,"<<gmd.returnFrameNumber() - nframes[0] + 1<<","<<gmd.returnFrameNumber() - nframes[0] + 1
	<<")\' : x="<<(uint64_t) std::max(gmd.returnX_Anchor(),0.0)<<" : y="<<(uint64_t) std::max(gmd.returnY_Anchor(),0.0)
	<<" : w="<<(uint64_t) std::max(gmd.returnWidth(),0.0)<<" : h="<<(uint64_t) std::max(gmd.returnHeight(),0.0)
	<<" : color="<<gm_colors[gmd.returnId() % gm_colors.size()]<<",\\"
	<<std::endl;
    ii++;
    if(ii % 1600 == 0){
      // end and start another clip with ffmpeg
      sprintf(mystr, "ganita_video_%05ld.avi", jj);
      gmvo<<"drawbox=enable='between(n,1,1)' : x=0 : y=0 : w=0 : h=0 : color=red\" "<<mystr<<endl;
      jj++;
      nframes[0] = nframes[1];
      if(ii + 1600 < num){
	gmTracks[0]->returnTopGMD(ii+1600, gmd);
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

std::shared_ptr<GanitaMetricsTrack> GanitaMetricsTrackSet::returnTrack(uint64_t tt)
{
  return(gmTracks[tt]);
}

