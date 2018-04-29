// This file contains the methods for GanitaMetricsTrackSet. 

#include "ganita/metrics/GanitaMetricsTrackSet.hpp"

// Constructors
GanitaMetricsTrackSet::GanitaMetricsTrackSet(void)
{
  id = 0;
  track_set_start = 0;
  track_set_end = 0;
  verbosity = 0;
  numTracks = 0;
  frame_width = 0;
  frame_height = 0;
  gm_colors.push_back("red");
  gm_colors.push_back("blue");
  gm_colors.push_back("yellow");
  gm_colors.push_back("purple");
  gm_colors.push_back("orange");
  gm_colors.push_back("green");
  gm_bgcolors.push_back("white");
  gm_bgcolors.push_back("white");
  gm_bgcolors.push_back("black");
  gm_bgcolors.push_back("white");
  gm_bgcolors.push_back("black");
  gm_bgcolors.push_back("white");
  gm_frame_rate = GM_DEFAULT_FRAME_RATE;
}

uint64_t GanitaMetricsTrackSet::setStart(uint64_t ss)
{
  track_set_start = ss;

  return(track_set_start);
}

uint64_t GanitaMetricsTrackSet::setEnd(uint64_t ee)
{
  track_set_end = ee;

  return(track_set_end);
}

uint64_t GanitaMetricsTrackSet::returnStart(void)
{
  return(track_set_start);
}

uint64_t GanitaMetricsTrackSet::returnEnd(void)
{
  return(track_set_end);
}

int GanitaMetricsTrackSet::init(const char *buf_input)
{
  gmb = new GanitaBuffer();
  if(gmb->open((char *) buf_input) < 1){
    return(0);
  }
  cout<<"File "<<buf_input<<" size = "<<gmb->size()<<endl;

  return(1);
}

int GanitaMetricsTrackSet::readTopAsOne(void)
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

  setNumTracks(1);

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
  int64_t total_tracks;
  int64_t *rev_ids = new int64_t[10000000]();
  int64_t *tru_ids = new int64_t[10000000]();

  total_tracks = 0;
  gmb->rewindFile();
  gmTracks.clear();
  
  string line("");
  while(gmb->getLine(line) >= 0){
    //cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %d, %d, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d", 
	   &new_id, &new_frame_number, &new_headValid, &new_bodyValid, 
	   &new_headLeft, &new_headTop, &new_headRight, &new_headBottom,
	   &new_bodyLeft, &new_bodyTop, &new_bodyRight, &new_bodyBottom, 
	   &new_confidence, &new_verbosity);

    if(tru_ids[new_id] == 0){
      // create a new track
      addTrack();
      tru_ids[new_id] = 1;
      rev_ids[new_id] = total_tracks;
      gmTracks[rev_ids[new_id]]->setStart(new_frame_number);
      gmTracks[rev_ids[new_id]]->setId(new_id);
      total_tracks++;
    }

    num = gmTracks[rev_ids[new_id]]->addTopDetection
      (new_id, new_frame_number, new_headValid, new_bodyValid, 
       new_headLeft, new_headTop, new_headRight, new_headBottom,
       new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom, 
       new_confidence, new_verbosity);
    gmTracks[rev_ids[new_id]]->returnTopGMD(num - 1, gmd);
    if(verbosity > 0){
      cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    }
    line = "";
  }

  delete rev_ids;
  delete tru_ids;

  if(verbosity){
    cout<<"Total number of tracks = "<<total_tracks<<endl;
  }
  setNumTracks(total_tracks);

  //Set the last frame number for each track
  setEndFrames();
  // Compute the min and max frames of entire Track Set.
  computeEndPoints();
  computeResolution();

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

int GanitaMetricsTrackSet::visTracks(string myvideoname)
{
  uint64_t ii, jj, num;
  uint64_t nframes[2];
  GanitaMetricsTopDetection gmd;
  char mystr[101];
  uint64_t final_frame;
  double start_time;
  ofstream gmvo;

  gmvo.open("ganita_metrics_vis.sh");

  if(verbosity > 0){
    std::cout<<"frame rate ("<<gm_frame_rate<<")"<<std::endl;
  }
  
  jj = 0; ii = 0;
  num = gmTracks[0]->returnNumberOfTopDetections();
  gmTracks[0]->returnTopGMD(num-1, gmd);
  final_frame = gmd.returnFrameNumber();
  addVis();
  nframes[1] = 0;
  if(num > 450){
    gmTracks[0]->returnTopGMD(450, gmd);
    nframes[1] = gmd.returnFrameNumber();
  }
  nframes[0] = 0;
  gmvo<<"#!/bin/bash"<<endl;
  gmvo<<"ffmpeg -i "<<myvideoname<<" -vframes "<<nframes[1]<<" -vf \\"<<endl;
  gmvo<<"\"";
  while(ii<num){
    gmTracks[0]->returnTopGMD(ii, gmd);
    //gmvo<<"Frame number = "<<gmd.returnFrameNumber()<<endl;
    //gmvis[0]->ffmpegBox(gmd.returnFrameNumber() - nframes[0] + 1, gmd.returnFrameNumber() - nframes[0] + 1,
    //		(uint64_t) std::max(gmd.returnX_Anchor(), 0), (uint64_t) std::max(gmd.returnY_Anchor(), 0),
    //		(uint64_t) gmd.returnWidth(), (uint64_t) gmd.returnHeight());
//     std::cout<<"drawbox=enable=\'between(n,"<<gmd.returnFrameNumber() - nframes[0] + 1<<","<<gmd.returnFrameNumber() - nframes[0] + 1;
//     fflush(stdout);
//     std::cout<<")\' : x="<<(uint64_t) std::max(gmd.returnX_Anchor(),0.0)<<" : y="<<(uint64_t) std::max(gmd.returnY_Anchor(),0.0);
//     fflush(stdout);
//     std::cout<<" : w="<<(uint64_t) std::max(gmd.returnWidth(),0.0)<<" : h="<<(uint64_t) std::max(gmd.returnHeight(),0.0);
//     fflush(stdout);
//     std::cout<<" : color="<<gm_colors[gmd.returnId() % gm_colors.size()]<<",\\"
// 	<<std::endl;
//     fflush(stdout);
    gmvo<<"drawbox=enable=\'between(n,"<<gmd.returnFrameNumber() - nframes[0]<<","<<gmd.returnFrameNumber() - nframes[0]
	<<")\' : x="<<(uint64_t) std::max(gmd.returnX_Anchor(),0.0)<<" : y="<<(uint64_t) std::max(gmd.returnY_Anchor(),0.0)
	<<" : w="<<(uint64_t) std::max(gmd.returnWidth(),0.0)<<" : h="<<(uint64_t) std::max(gmd.returnHeight(),0.0)
	<<" : color="<<gm_colors[gmd.returnId() % gm_colors.size()]<<",\\"
	<<std::endl;
    gmvo<<"drawtext=enable=\'eq(n,"<<gmd.returnFrameNumber() - nframes[0]
	<<")\' :fontfile=/usr/share/fonts/open-sans/OpenSans-Bold.ttf: text=\'"<<gmd.returnId()<<"\': fontcolor="<<gm_colors[gmd.returnId() % gm_colors.size()]<<": fontsize=32: box=1: boxcolor="<<gm_bgcolors[gmd.returnId() % gm_colors.size()]<<"@0.9: x="<<(uint64_t) std::max(gmd.returnX_Anchor()-8,0.0)<<":y="<<(uint64_t) std::max(gmd.returnY_Anchor()-8,0.0)<<",\\"<<std::endl;
    ii++;
    if(ii % 450 == 0){
      // end and start another clip with ffmpeg
      sprintf(mystr, "ganita_video_%05ld.avi", jj);
      gmvo<<"drawbox=enable='between(n,1,1)' : x=0 : y=0 : w=0 : h=0 : color=red\" "<<mystr<<endl;
      jj++;
      nframes[0] = nframes[1];
      if(ii + 450 < num){
	gmTracks[0]->returnTopGMD(ii+450, gmd);
	nframes[1] = gmd.returnFrameNumber();
      }
      else nframes[1] = final_frame;
      start_time = ((double) nframes[0]) / gm_frame_rate;
      //gmvo<<"ffmpeg -i ../data/duke_tracker_output/TownCentre/TownCentreXVID.avi -vf \\"<<endl;
      gmvo<<"ffmpeg -ss "<<start_time<<" -i "<<myvideoname<<" -vframes "
	  <<nframes[1] - nframes[0]<<" -vf \\"<<endl;
      gmvo<<"\"";
    }
  }
  sprintf(mystr, "ganita_video_%05ld.avi", jj);
  gmvo<<"drawbox=enable='between(n,1,1)' : x=0 : y=0 : w=0 : h=0 : color=red\" "<<mystr<<endl;
  gmvo<<"for f in ./ganita_video_*.avi; do echo \"file '$f'\" >> gv_list.txt; done"<<endl;
  gmvo<<"ffmpeg -f concat -safe 0 -i gv_list.txt -c copy ganita_vis.avi"<<endl;
  gmvo<<"rm -f ./ganita_video_*.avi"<<endl;

  gmvo.close();

  return(1);
}

std::shared_ptr<GanitaMetricsTrack> GanitaMetricsTrackSet::returnTrack(uint64_t tt)
{
  return(gmTracks[tt]);
}

int64_t GanitaMetricsTrackSet::setEndFrames(void)
{
  uint64_t ii;
  int64_t num;
  int64_t max_frame;
  GanitaMetricsTopDetection gmd;

  max_frame = 0;
  for(ii=0; ii<gmTracks.size(); ii++){
    num = gmTracks[ii]->returnNumberOfTopDetections();
    gmTracks[ii]->returnTopGMD(num-1, gmd);
    gmTracks[ii]->setEnd(gmd.returnFrameNumber());
    if(gmd.returnFrameNumber() > max_frame){
      max_frame = gmd.returnFrameNumber();
    }
  }

  return(max_frame);
}

int64_t GanitaMetricsTrackSet::printStartStop(void)
{
  uint64_t ii;

  for(ii=0; ii<gmTracks.size(); ii++){
    cout<<"Track Id ("<<gmTracks[ii]->returnId()<<") First frame ("<<gmTracks[ii]->returnStart()<<") Last frame ("<<gmTracks[ii]->returnEnd()<<")"<<endl;
  }

  return(gmTracks.size());
}

int64_t GanitaMetricsTrackSet::setNumTracks(uint64_t tnum)
{
  numTracks = tnum;

  return((int64_t) numTracks);
}

uint64_t GanitaMetricsTrackSet::returnNumTracks(void)
{
  return(numTracks);
}

int GanitaMetricsTrackSet::computeEndPoints(void)
{
  uint64_t ii;
  uint64_t min_frame, max_frame;

  min_frame = 1<<31;
  max_frame = 0;
  
  for(ii=0; ii<gmTracks.size(); ii++){
    if(min_frame > (uint64_t) gmTracks[ii]->returnStart()){
      min_frame = gmTracks[ii]->returnStart();
    }
    if(max_frame < (uint64_t) gmTracks[ii]->returnEnd()){
      max_frame = gmTracks[ii]->returnEnd();
    }
  }

  setStart(min_frame);
  setEnd(max_frame);

  return(1);
}

int GanitaMetricsTrackSet::computeResolution(void)
{
  uint64_t ii, jj, num;
  uint64_t ww, hh;
  std::shared_ptr<GanitaMetricsTrack> refTrack;
  GanitaMetricsTopDetection gmd;

  ww = 0;
  hh = 0;
  
  for(ii=0; ii<gmTracks.size(); ii++){
    refTrack = gmTracks[ii];
    num = refTrack->returnNumberOfTopDetections();
    for(jj=0; jj < num; jj++){
      refTrack->returnTopGMD(jj, gmd);
      if(gmd.returnX_Anchor() + gmd.returnWidth() > ww){
	ww = gmd.returnX_Anchor()+gmd.returnWidth();
	if(verbosity > 0){
	  cout<<"New max width ("<<ww<<") x("<<gmd.returnX_Anchor()<<") w("
	      <<gmd.returnWidth()<<") t("<<ii<<") f("<<jj<<")"<<endl;
	}
      }
      if(gmd.returnY_Anchor() + gmd.returnHeight() > hh){
	hh = gmd.returnY_Anchor()+gmd.returnHeight();
	if(verbosity > 0){
	  cout<<"New max height ("<<hh<<") x("<<gmd.returnX_Anchor()
	      <<") y("<<gmd.returnY_Anchor()<<") h("
	      <<gmd.returnHeight()<<") t("<<ii<<") f("<<jj<<")"<<endl;
	}
      }
    }
  }

  frame_width = ww;
  frame_height = hh;

  return(1);
}

uint64_t GanitaMetricsTrackSet::returnFrameWidth()
{
  return(frame_width);
}

uint64_t GanitaMetricsTrackSet::returnFrameHeight()
{
  return(frame_height);
}

