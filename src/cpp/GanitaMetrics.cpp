// This file contains the methods for GanitaMetrics. 

#include "ganita/metrics/GanitaMetrics.hpp"

// Constructors
GanitaMetrics::GanitaMetrics(void)
{
  verbosity = 0;
  gm_colors.push_back("red");
  gm_colors.push_back("blue");
  gm_colors.push_back("yellow");
  gm_colors.push_back("purple");
  gm_colors.push_back("orange");
  gm_colors.push_back("green");
}

GanitaMetrics::GanitaMetrics(int vv)
{
  verbosity = vv;
  gm_colors.push_back("red");
  gm_colors.push_back("blue");
  gm_colors.push_back("yellow");
  gm_colors.push_back("purple");
  gm_colors.push_back("orange");
  gm_colors.push_back("green");
}

int GanitaMetrics::init(char *ref_input, char *sys_input)
{
  gmr = new GanitaBuffer();
  gmr->open(ref_input);
  cout<<"Reference file size = "<<gmr->size()<<endl;

  gms = new GanitaBuffer();
  gms->open(sys_input);
  cout<<"System file size = "<<gms->size()<<endl;

  return(1);
}

int GanitaMetrics::init_2(char *ref_input, char *sys_input)
{
  gmts[0].init(ref_input);
  gmts[1].init(sys_input);

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

  addRefTrack();
  
  string line("");
  while(gmr->getLine(line) >= 0){
    cout<<line;
    sscanf(line.c_str(), "%ld, %ld, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %d",  
	   &new_id, &new_frame_number, &new_x_anchor, &new_y_anchor, &new_width, &new_height, 
	   &new_confidence, &new_x_world, &new_y_world, &new_z_world, &new_verbosity);
 
    num = gmrTracks[0]->addMotDetection(new_id, new_frame_number, new_x_anchor, new_y_anchor, new_width, new_height, 
			      new_confidence, new_x_world, new_y_world, new_z_world, new_verbosity);
    gmrTracks[0]->returnMotGMD(num - 1, gmd);
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
  int64_t min_frame, max_frame;

  min_frame = 1<<31;
  max_frame = -1;
  
  addRefTrack();
  
  string line("");
  while(gmr->getLine(line) >= 0){
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
 
    num = gmrTracks[0]->addTopDetection(new_id, new_frame_number, new_headValid, new_bodyValid, 
				       new_headLeft, new_headTop, new_headRight, new_headBottom,
				       new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom, 
				       new_confidence, new_verbosity);
    gmrTracks[0]->returnTopGMD(num - 1, gmd);
    //cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    line = "";
  }

  gmrTracks[0]->setStart(min_frame);
  gmrTracks[0]->setEnd(max_frame);

  return(1);
}

int64_t GanitaMetrics::addRefTrack(void)
{
  GanitaMetricsTrack *newtrack = new GanitaMetricsTrack();
  gmrTracks.push_back(std::make_shared<GanitaMetricsTrack>(*newtrack));
  delete newtrack;
  return(gmrTracks.size());
}

int GanitaMetrics::readTopSystem(void)
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
  
  addSysTrack();
  
  string line("");
  while(gms->getLine(line) >= 0){
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
 
    num = gmsTracks[0]->addTopDetection(new_id, new_frame_number, new_headValid, new_bodyValid, 
				       new_headLeft, new_headTop, new_headRight, new_headBottom,
				       new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom, 
				       new_confidence, new_verbosity);
    gmsTracks[0]->returnTopGMD(num - 1, gmd);
    //cout<<"Number of detections = "<<num<<" Frame # = "<<gmd.returnFrameNumber()<<endl;
    line = "";
  }

  gmsTracks[0]->setStart(min_frame);
  gmsTracks[0]->setEnd(max_frame);

  return(1);
}

int64_t GanitaMetrics::addSysTrack(void)
{
  GanitaMetricsTrack *newtrack = new GanitaMetricsTrack();
  gmsTracks.push_back(std::make_shared<GanitaMetricsTrack>(*newtrack));
  delete newtrack;
  return(gmsTracks.size());
}

int GanitaMetrics::dumpTHeader(void)
{
  GanitaMetricsVersion gzv;
  fprintf(stdout, "GanitaMetrics version \t %s\n", gzv.returnVersion().c_str());

  return (1);
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
  num = gmrTracks[0]->returnNumberOfTopDetections();
  gmrTracks[0]->returnTopGMD(num-1, gmd);
  final_frame = gmd.returnFrameNumber();
  addVis();
  nframes[1] = 0;
  if(num > 1600){
    gmrTracks[0]->returnTopGMD(1600, gmd);
    nframes[1] = gmd.returnFrameNumber();
  }
  nframes[0] = 0;
  gmvo<<"#!/bin/bash"<<endl;
  gmvo<<"ffmpeg -i /diva/DIVA/DATA/Tracking/TownCentre/TownCentreXVID.avi -vframes "<<nframes[1]<<" -vf \\"<<endl;
  gmvo<<"\"";
  while(ii<num){
    gmrTracks[0]->returnTopGMD(ii, gmd);
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
	gmrTracks[0]->returnTopGMD(ii+1600, gmd);
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

int GanitaMetrics::visTracks(int tset, int tnum)
{
  uint64_t ii, jj, num;
  uint64_t nframes[2];
  GanitaMetricsTopDetection gmd;
  char mystr[101];
  uint64_t final_frame;
  double start_time;
  ofstream gmvo;
  std::shared_ptr< GanitaMetricsTrack> myTrack;

  gmvo.open("ganita_metrics_vis.sh");
  
  jj = 0; ii = 0;
  myTrack = gmts[tset % 2].returnTrack(tnum);
  num = myTrack->returnNumberOfTopDetections();
  myTrack->returnTopGMD(num-1, gmd);
  final_frame = gmd.returnFrameNumber();
  addVis();
  if(num > 1600){
    myTrack->returnTopGMD(1600, gmd);
    nframes[1] = gmd.returnFrameNumber();
  }
  else{ nframes[1] = 1600; }
  nframes[0] = 0;
  gmvo<<"#!/bin/bash"<<endl;
  gmvo<<"ffmpeg -i /diva/DIVA/DATA/Tracking/TownCentre/TownCentreXVID.avi -vframes "<<nframes[1]<<" -vf \\"<<endl;
  gmvo<<"\"";
  while(ii<num){
    myTrack->returnTopGMD(ii, gmd);
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
	myTrack->returnTopGMD(ii+1600, gmd);
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

// This was used to test the detection density on a per frame basis.
int GanitaMetrics::printDetDenFrame(void)
{
  GanitaMetricsMat det_mat(1920,1080);
  uint64_t ii, jj;

  gmrTracks[0]->computeDetectionDensity(0,det_mat);

  for(jj=0; jj<1080; jj++){
    for(ii=0; ii<1920; ii++){
      cout<<det_mat.get(ii,jj)<<" ";
    }
    cout<<endl;
  }

  return(1);
}

// This was used to test the detection density on a per frame basis.
int GanitaMetrics::printDetDenFrame(int ss)
{
  GanitaMetricsMat det_mat(1920,1080);
  uint64_t ii, jj;

  gmts[ss % 2].returnTrack(0)->computeDetectionDensity(0,det_mat);

  for(jj=0; jj<1080; jj++){
    for(ii=0; ii<1920; ii++){
      cout<<det_mat.get(ii,jj)<<" ";
    }
    cout<<endl;
  }

  return(1);
}

// Each set of tracks produces a density or frequency map in the following way. 
// Take the reference set of tracks. 
// Given each frame, and each pixel, calculate the number of reference 
// tracks that contain that pixel location. This is the density map:
// reference_detection_density_map(frame_number, xx, yy). 
// Its values are non-negative integers. 
// We will compute a similar map for system output: 
// system_detection_density_map(frame_number, xx, yy).
// Then we will compute the KL-divergence between the two density maps. 
// KL-divergence = 
// Avg_{frame_number} KL-div(reference_detection_density_map,system_detection_density_map).

// First, we compute the error produced by:
// false detects (This is computed in the KL-divergence framework.)
// missed detections (This is symmetric to false detects with ref and sys tracks roles reversed.)
// difference in ref and sys densities where they are absolutely continuous.
int GanitaMetrics::computeKL_DensityDistance(uint64_t fr_num)
{
  GanitaMetricsMat det_mat_ref(1920,1080);
  GanitaMetricsMat det_mat_sys(1920,1080);
  //GanitaMetricsTopDetection gmsd;
  uint64_t ii, jj;
  uint64_t overlap, no_overlap;
  uint64_t ref_not_sys, sys_not_ref;
  double kL1;

  overlap = 0;
  no_overlap = 0;
  ref_not_sys = 0;
  sys_not_ref = 0;
  kL1 = 0;

  gmrTracks[0]->computeDetectionDensity(fr_num,det_mat_ref);
  gmsTracks[0]->computeDetectionDensity(fr_num,det_mat_sys);

  //gmsTracks[0]->returnTopGMD(fr_num, gmsd);

  for(jj=0; jj<1080; jj++){
    for(ii=0; ii<1920; ii++){
      if(det_mat_ref.get(ii,jj) == 0){
	if(det_mat_sys.get(ii,jj) == 0){
	  // Not part of the support of either density.
	  no_overlap++;
	}
	else{
	  // In support of sys density, but not ref density.
	  sys_not_ref++;
	}
      }
      else{
	if(det_mat_sys.get(ii,jj) == 0){
	  // In support of ref density, but not sys density.
	  ref_not_sys++;
	}
	else{
	  // In support of both densities.
	  overlap++;
	  if(det_mat_ref.get(ii,jj) > det_mat_sys.get(ii,jj)){
	    kL1 += log( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
	  }
	}
      }
    }
  }

  gmsTracks[0]->stats.push_back(6);
  gmsTracks[0]->stats.push_back(fr_num);
  gmsTracks[0]->stats.push_back(no_overlap);
  gmsTracks[0]->stats.push_back(overlap);
  gmsTracks[0]->stats.push_back(ref_not_sys);
  gmsTracks[0]->stats.push_back(sys_not_ref);
  gmsTracks[0]->stats.push_back(kL1);

  det_mat_ref.close();
  det_mat_sys.close();
  
  return(1);
}

int64_t GanitaMetrics::printTrackStats(void)
{
  return(gmsTracks[0]->printStats());
}
  
// Each set of tracks produces a density or frequency map in the following way. 
// Take the reference set of tracks. 
// Given each frame, and each pixel, calculate the number of reference 
// tracks that contain that pixel location. This is the density map:
// reference_detection_density_map(frame_number, xx, yy). 
// Its values are non-negative integers. 
// We will compute a similar map for system output: 
// system_detection_density_map(frame_number, xx, yy).
// Then we will compute the KL-divergence between the two density maps. 
// KL-divergence = 
// Avg_{frame_number} KL-div(reference_detection_density_map,system_detection_density_map).

// First, we compute the error produced by:
// false detects (This is computed in the KL-divergence framework.)
// missed detections (This is symmetric to false detects with ref and sys tracks roles reversed.)
// difference in ref and sys densities where they are absolutely continuous.
int GanitaMetrics::computeKL_DensityDistance_2(uint64_t fr_num)
{
  GanitaMetricsMat det_mat_ref(1920,1080);
  GanitaMetricsMat det_mat_sys(1920,1080);
  uint64_t ii, jj;
  uint64_t overlap, no_overlap;
  uint64_t ref_not_sys, sys_not_ref;
  double kL1;

  overlap = 0;
  no_overlap = 0;
  ref_not_sys = 0;
  sys_not_ref = 0;
  kL1 = 0;

  gmts[0].returnTrack(0)->computeDetectionDensity(fr_num,det_mat_ref);
  gmts[1].returnTrack(0)->computeDetectionDensity(fr_num,det_mat_sys);

  for(jj=0; jj<1080; jj++){
    for(ii=0; ii<1920; ii++){
      if(det_mat_ref.get(ii,jj) == 0){
	if(det_mat_sys.get(ii,jj) == 0){
	  // Not part of the support of either density.
	  no_overlap++;
	}
	else{
	  // In support of sys density, but not ref density.
	  sys_not_ref++;
	}
      }
      else{
	if(det_mat_sys.get(ii,jj) == 0){
	  // In support of ref density, but not sys density.
	  ref_not_sys++;
	}
	else{
	  // In support of both densities.
	  overlap++;
	  if(det_mat_ref.get(ii,jj) > det_mat_sys.get(ii,jj)){
	    kL1 += log( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
	  }
	}
      }
    }
  }

  gmts[1].returnTrack(0)->stats.push_back(6);
  gmts[1].returnTrack(0)->stats.push_back(fr_num);
  gmts[1].returnTrack(0)->stats.push_back(no_overlap);
  gmts[1].returnTrack(0)->stats.push_back(overlap);
  gmts[1].returnTrack(0)->stats.push_back(ref_not_sys);
  gmts[1].returnTrack(0)->stats.push_back(sys_not_ref);
  gmts[1].returnTrack(0)->stats.push_back(kL1);
  
  det_mat_ref.close();
  det_mat_sys.close();
  
  return(1);
}

// Each set of tracks produces a density or frequency map in the following way. 
// Take the reference set of tracks. 
// Given each frame, and each pixel, calculate the number of reference 
// tracks that contain that pixel location. This is the density map:
// reference_detection_density_map(frame_number, xx, yy). 
// Its values are non-negative integers. 
// We will compute a similar map for system output: 
// system_detection_density_map(frame_number, xx, yy).
// Then we will compute the KL-divergence between the two density maps. 
// KL-divergence = 
// Avg_{frame_number} KL-div(reference_detection_density_map,system_detection_density_map).

// First, we compute the error produced by:
// false detects (This is computed in the KL-divergence framework.)
// missed detections (This is symmetric to false detects with ref and sys tracks roles reversed.)
// difference in ref and sys densities where they are absolutely continuous.
int GanitaMetrics::computeKL_DensityDistance(uint64_t fr_num, uint64_t ref_nn, uint64_t sys_nn)
{
  GanitaMetricsMat det_mat_ref(1920,1080);
  GanitaMetricsMat det_mat_sys(1920,1080);
  uint64_t ii, jj;
  uint64_t overlap, no_overlap;
  uint64_t ref_not_sys, sys_not_ref;
  double kL1;

  overlap = 0;
  no_overlap = 0;
  ref_not_sys = 0;
  sys_not_ref = 0;
  kL1 = 0;

  gmts[0].returnTrack(ref_nn)->computeDetectionDensity(fr_num,det_mat_ref);
  gmts[1].returnTrack(sys_nn)->computeDetectionDensity(fr_num,det_mat_sys);

  for(jj=0; jj<1080; jj++){
    for(ii=0; ii<1920; ii++){
      if(det_mat_ref.get(ii,jj) == 0){
	if(det_mat_sys.get(ii,jj) == 0){
	  // Not part of the support of either density.
	  no_overlap++;
	}
	else{
	  // In support of sys density, but not ref density.
	  sys_not_ref++;
	}
      }
      else{
	if(det_mat_sys.get(ii,jj) == 0){
	  // In support of ref density, but not sys density.
	  ref_not_sys++;
	}
	else{
	  // In support of both densities.
	  overlap++;
	  if(det_mat_ref.get(ii,jj) > det_mat_sys.get(ii,jj)){
	    kL1 += log( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
	  }
	}
      }
    }
  }

  gmts[1].returnTrack(sys_nn)->stats.push_back(6);
  gmts[1].returnTrack(sys_nn)->stats.push_back(fr_num);
  gmts[1].returnTrack(sys_nn)->stats.push_back(no_overlap);
  gmts[1].returnTrack(sys_nn)->stats.push_back(overlap);
  gmts[1].returnTrack(sys_nn)->stats.push_back(ref_not_sys);
  gmts[1].returnTrack(sys_nn)->stats.push_back(sys_not_ref);
  gmts[1].returnTrack(sys_nn)->stats.push_back(kL1);
  
  det_mat_ref.close();
  det_mat_sys.close();
  
  return(1);
}

int64_t GanitaMetrics::printTrackStats(int ss)
{
  return(gmts[ss % 2].returnTrack(0)->printStats());
}
  
int64_t GanitaMetrics::printTrackStats(int tset, uint64_t tnum)
{
  return(gmts[tset % 2].returnTrack(tnum)->printStats());
}
  
int GanitaMetrics::readTop(int tt)
{
  return(gmts[tt % 2].readTop());
}

int GanitaMetrics::readTopAsOne(int tt)
{
  return(gmts[tt % 2].readTopAsOne());
}

int GanitaMetrics::computeFrameStats(void)
{
  GanitaMetricsTopDetection gmd;
  std::shared_ptr< GanitaMetricsTrack > refTrack, sysTrack;
  uint64_t ii, jj;
  uint64_t ff1, ff2, ff;
  uint64_t final_frame1, final_frame2, final_frame;
  uint64_t num;
 
  ii = 1; jj = 1;
  ff = 0;

  refTrack = gmts[0].returnTrack(0);
  num = refTrack->returnNumberOfTopDetections();
  refTrack->returnTopGMD(num-1, gmd);
  final_frame1 = gmd.returnFrameNumber();
  sysTrack = gmts[1].returnTrack(0);
  num = sysTrack->returnNumberOfTopDetections();
  sysTrack->returnTopGMD(num-1, gmd);
  final_frame2 = gmd.returnFrameNumber();
  if(final_frame1 > final_frame2){
    final_frame = final_frame1;
    //refTrack->returnTopGMD(0, gmd);
  }
  else{
    final_frame = final_frame2;
    //sysTrack->returnTopGMD(0, gmd);
  }

  refTrack->returnTopGMD(0, gmd);
  ff1 = gmd.returnFrameNumber();
  sysTrack->returnTopGMD(0, gmd);
  ff2 = gmd.returnFrameNumber();
  if(ff1 > ff2){
    ff = ff2;
    sysTrack->returnTopGMD(0, gmd);
  }
  else{
    ff = ff1;
    refTrack->returnTopGMD(0, gmd);
  }
  while(ff<final_frame){
    computeKL_DensityDistance_2(ff);
    cout<<"Computed stats on frame "<<ff<<endl;
    while(ff1 <= ff){
      refTrack->returnTopGMD(ii, gmd);
      ff1 = gmd.returnFrameNumber();
      ii++;
    }
    while(ff2 <= ff){
      sysTrack->returnTopGMD(jj, gmd);
      ff2 = gmd.returnFrameNumber();
      jj++;
    }
    if(ff1 < ff2){
      ff = ff1;
    }
    else{ ff = ff2; }
  }
  computeKL_DensityDistance_2(final_frame);
 
  return(1);	  
}

int64_t GanitaMetrics::printStartStop(int tset)
{
  return(gmts[tset % 2].printStartStop());
}

int GanitaMetrics::computeTrackPairKL_aux(int64_t ref_nn, int64_t sys_nn)
{
  GanitaMetricsTopDetection gmd1, gmd2;
  std::shared_ptr< GanitaMetricsTrack > refTrack, sysTrack;
  uint64_t ii, jj;
  uint64_t ff1, ff2, ff;
  uint64_t final_frame1, final_frame2, final_frame;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0;

  refTrack = gmts[0].returnTrack(ref_nn);
  sysTrack = gmts[1].returnTrack(sys_nn);
  ff1 = refTrack->returnStart();
  ff2 = sysTrack->returnStart();
  final_frame1 = refTrack->returnEnd();
  final_frame2 = sysTrack->returnEnd();

  if(ff1 > final_frame2){
    // no overlap temporally
    return(-1);
  }
  if(ff2 > final_frame1){
    // no overlap temporally
    return(-1);
  }
  // Otherwise there is overlap
  if(ff1 < ff2){
    ii+= ff2 - ff1;
  }
  else{ jj += ff1 - ff2; }
  
  refTrack->returnTopGMD(ii, gmd1);
  ff1 = gmd1.returnFrameNumber();
  sysTrack->returnTopGMD(jj, gmd2);
  ff2 = gmd2.returnFrameNumber();

  cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;

  ff = ff1;
  final_frame = final_frame1;
  while(ff<final_frame){
    computeKL_DensityDistance(ff,ref_nn,sys_nn);
    //cout<<"Computed stats on frame "<<ff<<endl;
    while(ff1 <= ff){
      refTrack->returnTopGMD(ii, gmd1);
      ff1 = gmd1.returnFrameNumber();
      ii++;
    }
    while(ff2 <= ff){
      sysTrack->returnTopGMD(jj, gmd2);
      ff2 = gmd2.returnFrameNumber();
      jj++;
    }
    if(ff1 < ff2){
      ff = ff1;
    }
    else{ ff = ff2; }
  }
  computeKL_DensityDistance(final_frame,ref_nn,sys_nn);
 
  return(1);	  
}

int GanitaMetrics::computeTrackPairKL(int64_t ref_nn, int64_t sys_nn, double& score)
{
  GanitaMetricsTopDetection gmd1, gmd2;
  std::shared_ptr< GanitaMetricsTrack > refTrack, sysTrack;
  uint64_t ii, jj;
  uint64_t ff1, ff2, ff;
  uint64_t final_frame1, final_frame2, final_frame;
  double left_x1, right_x2;
  double lower_y1, upper_y2;
  double ref_x1, sys_x1;
  double ref_x2, sys_x2;
  double ref_y1, sys_y1;
  double ref_y2, sys_y2;
  double fr_overlap;
  uint64_t num1, num2;
  double kule;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0;

  refTrack = gmts[0].returnTrack(ref_nn);
  sysTrack = gmts[1].returnTrack(sys_nn);
  ff1 = refTrack->returnStart();
  ff2 = sysTrack->returnStart();
  final_frame1 = refTrack->returnEnd();
  final_frame2 = sysTrack->returnEnd();

  if(ff1 > final_frame2){
    // no overlap temporally
    return(-1);
  }
  if(ff2 > final_frame1){
    // no overlap temporally
    return(-1);
  }
  // Otherwise there is overlap
  if(ff1 < ff2){
    ii+= ff2 - ff1;
    ff = ff2;
  }
  else{
    jj += ff1 - ff2;
    ff = ff1;
  }

  cout<<"ii = "<<ii<<" "<<"jj = "<<jj<<endl;

  num1 = refTrack->returnNumberOfTopDetections();
  num2 = sysTrack->returnNumberOfTopDetections();

  kule = 0;
  final_frame = final_frame1;
  while(ff<=final_frame){
    refTrack->returnTopGMD(ii, gmd1);
    ff1 = gmd1.returnFrameNumber();
    sysTrack->returnTopGMD(jj, gmd2);
    ff2 = gmd2.returnFrameNumber();
    
    cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;
    cout<<"Values: "<<gmd1.returnX_Anchor()<<":"<<gmd1.returnY_Anchor()<<":"
	<<gmd1.returnWidth()<<":"<<gmd1.returnHeight()<<":"<<endl;
    cout<<"Values: "<<gmd2.returnX_Anchor()<<":"<<gmd2.returnY_Anchor()<<":"
	<<gmd2.returnWidth()<<":"<<gmd2.returnHeight()<<":"<<endl;

    // Find the lower left point of overlap
    ref_x1 = gmd1.returnX_Anchor();
    sys_x1 = gmd2.returnX_Anchor();
    if(ref_x1 > sys_x1){
      left_x1 = ref_x1;
    }
    else{ left_x1 = sys_x1; }

    ref_y1 = gmd1.returnY_Anchor();
    sys_y1 = gmd2.returnY_Anchor();
    if(ref_y1 > sys_y1){
      lower_y1 = ref_y1;
    }
    else{ lower_y1 = sys_y1; }
   
    // Find the upper right point of overlap
    ref_x2 = gmd1.returnX_Anchor() + gmd1.returnWidth();
    sys_x2 = gmd2.returnX_Anchor() + gmd2.returnWidth();
    if(ref_x2 > sys_x2){
      right_x2 = sys_x2;
    }
    else{ right_x2 = ref_x2; }

    ref_y2 = gmd1.returnY_Anchor() + gmd1.returnHeight();
    sys_y2 = gmd2.returnY_Anchor() + gmd2.returnHeight();
    if(ref_y2 > sys_y2){
      upper_y2 = sys_y2;
    }
    else{ upper_y2 = ref_y2; }

    cout<<"Computed stats on frame "<<ff<<endl;
    if((left_x1 >= right_x2) || (lower_y1 >= upper_y2)){
      // There is no overlap
      fr_overlap = 0;
    }
    else{
      fr_overlap = (upper_y2 - lower_y1)*(right_x2 - left_x1);
      cout<<"Overlap ("<<left_x1<<","<<lower_y1<<") "<<"("<<right_x2<<","<<upper_y2<<")"<<endl;
    }
    if((gmd1.returnWidth() <= 0) || (gmd1.returnHeight() <= 0)){
      //skip
    }
    else{
      kule += ((double) fr_overlap) / (gmd1.returnWidth() * gmd1.returnHeight());
    }
    cout<<"Overlap "<<fr_overlap<<endl;

    ff++;

    while(ff1 < ff){
      ii++;
      if(ii >= num1){
	// reached end of track
	break;
      }
      refTrack->returnTopGMD(ii, gmd1);
      ff1 = gmd1.returnFrameNumber();
    }
    while(ff2 < ff){
      jj++;
      if(jj >= num2){
	// reached end of track
	break;
      }
      sysTrack->returnTopGMD(jj, gmd2);
      ff2 = gmd2.returnFrameNumber();
    }
    if((ii >= num1) || (jj >= num2)){
      // a track has finished
      break;
    }
  }
  kule /= num1;
  if(kule > 0){
    score = -1*kule*log(kule) / log(2);
    cout<<"Average overlap per frame "<<kule<<" KL-score "<<score<<endl;
  }
  else{
    score = 0;
    cout<<"Average overlap per frame "<<kule<<", KL-score "<<score<<endl;
  }
  
  return(1);	  
}

int GanitaMetrics::computeTrackKL(int64_t ref_nn)
{
  uint64_t num, ii;
  double scoreKL;
  double score;

  scoreKL = 0;
  num = gmts[1].returnNumTracks();
  for(ii=0; ii<num; ii++){
    computeTrackPairKL(ref_nn, ii, score);
    scoreKL += score;
  }

  cout<<"Final inner divergence "<<scoreKL<<endl;

  return(1);
}

