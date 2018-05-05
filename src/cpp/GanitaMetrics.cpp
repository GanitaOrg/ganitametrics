// Author: T.M. Adams (ganita.org)
// This class implements algorithms that implement a tracking performance 
// metric loosely based on KL-divergence. 
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
  gm_kl_mode = 0;
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
  gm_kl_mode = 0;
}

int GanitaMetrics::init(GanitaMetricsOptions myOpt)
{
  verbosity = myOpt.returnVerbosity();
  gmts[0].setVerbose(verbosity);
  gmts[1].setVerbose(verbosity);

  // check for environment variables
  if(const char* env_gm_kl_mode = std::getenv("GM_KL_MODE")){
    gm_kl_mode = std::stoi(env_gm_kl_mode);
    if(verbosity > 0){
      std::cout << "env var GM_KL_MODE: " <<gm_kl_mode<< '\n';
    }
  }

  if(gmts[0].init(myOpt.returnFileName(0).c_str()) < 1){
    if(verbosity > 0){
      std::cerr<<"Unable to open input file ("<<myOpt.returnFileName(0)<<")"<<std::endl;
    }
    return(0);
  }
  if(gmts[1].init(myOpt.returnFileName(1).c_str()) < 1){
    if(verbosity > 0){
      std::cerr<<"Unable to open input file ("<<myOpt.returnFileName(1)<<")"<<std::endl;
    }
    return(0);
  }

  vname = myOpt.returnVideoName();

  return(2);
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

int GanitaMetrics::visTracks(int tr_num)
{
  gmts[tr_num % 2].visTracks(vname);

  return(1);
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
  GanitaMetricsMat det_mat(major_width,major_height);
  uint64_t ii, jj;

  gmrTracks[0]->computeDetectionDensity(0,det_mat);

  for(jj=0; jj<major_height; jj++){
    for(ii=0; ii<major_width; ii++){
      cout<<det_mat.get(ii,jj)<<" ";
    }
    cout<<endl;
  }

  return(1);
}

// This was used to test the detection density on a per frame basis.
int GanitaMetrics::printDetDenFrame(int ss)
{
  GanitaMetricsMat det_mat(major_width,major_height);
  uint64_t ii, jj;

  gmts[ss % 2].returnTrack(0)->computeDetectionDensity(0,det_mat);

  for(jj=0; jj<major_height; jj++){
    for(ii=0; ii<major_width; ii++){
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
  GanitaMetricsMat det_mat_ref(major_width,major_height);
  GanitaMetricsMat det_mat_sys(major_width,major_height);
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

  for(jj=0; jj<major_height; jj++){
    for(ii=0; ii<major_width; ii++){
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
	    kL1 += log2( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log2( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
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
  GanitaMetricsMat det_mat_ref(major_width,major_height);
  GanitaMetricsMat det_mat_sys(major_width,major_height);
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

  for(jj=0; jj<major_height; jj++){
    for(ii=0; ii<major_width; ii++){
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
	    kL1 += log2( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log2( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
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
  GanitaMetricsMat det_mat_ref(major_width,major_height);
  GanitaMetricsMat det_mat_sys(major_width,major_height);
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

  for(jj=0; jj<major_height; jj++){
    for(ii=0; ii<major_width; ii++){
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
	    kL1 += log2( ((double) det_mat_ref.get(ii,jj)) / ((double) det_mat_sys.get(ii,jj)) );
	  }
	  else{
	    kL1 += log2( ((double) det_mat_sys.get(ii,jj)) / ((double) det_mat_ref.get(ii,jj)) );
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

int GanitaMetrics::closeBuffers(void)
{
  gmr->close();
  gms->close();

  return(2);
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

int GanitaMetrics::computeTrackPairKL(int64_t ref_nn, int64_t sys_nn, int flip, double& score)
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
  double vol1, vol2;
  uint64_t pnf;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0; pnf = 0;
  score = 0;

  refTrack = gmts[flip].returnTrack(ref_nn);
  sysTrack = gmts[1-flip].returnTrack(sys_nn);
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

  if(verbosity > 1){
    cout<<"ii = "<<ii<<" "<<"jj = "<<jj<<endl;
  }

  num1 = refTrack->returnNumberOfTopDetections();
  num2 = sysTrack->returnNumberOfTopDetections();

  vol1 = 0;
  vol2 = 0;
  kule = 0;
  final_frame = final_frame1;
  while(ff<=final_frame){
    refTrack->returnTopGMD(ii, gmd1);
    ff1 = gmd1.returnFrameNumber();
    sysTrack->returnTopGMD(jj, gmd2);
    ff2 = gmd2.returnFrameNumber();
    
    if(verbosity > 0){
      if((gmd1.returnX_Anchor() + gmd1.returnWidth() > 1920) || 
	 (gmd1.returnY_Anchor() + gmd1.returnHeight() > 1080)){
	cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;
	cout<<"Values: "<<gmd1.returnX_Anchor()<<":"<<gmd1.returnY_Anchor()<<":"
	    <<gmd1.returnWidth()<<":"<<gmd1.returnHeight()<<":"<<endl;
	cout<<"Values: "<<gmd2.returnX_Anchor()<<":"<<gmd2.returnY_Anchor()<<":"
	    <<gmd2.returnWidth()<<":"<<gmd2.returnHeight()<<":"<<endl;
      }
    }

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

    //cout<<"Computed stats on frame "<<ff<<endl;
    if((left_x1 >= right_x2) || (lower_y1 >= upper_y2)){
      // There is no overlap
      fr_overlap = 0;
    }
    else{
      fr_overlap = (upper_y2 - lower_y1)*(right_x2 - left_x1);
      if(verbosity > 1){
	cout<<"Overlap ("<<left_x1<<","<<lower_y1<<") "<<"("<<right_x2<<","<<upper_y2<<")"<<endl;
      }
//       if(gmd1.returnAuxValid() == 1){
// 	cout<<"Expected overlap"<<endl;
//       }
    }
    if((gmd1.returnWidth() <= 0) || (gmd1.returnHeight() <= 0)){
      //skip
    }
    else{
      pnf++;
      vol1 += ((double) fr_overlap) / 10000.0;
      vol2 += ((double) (gmd1.returnWidth() * gmd1.returnHeight())) / 10000.0;
      //kule += ((double) fr_overlap) / (gmd1.returnWidth() * gmd1.returnHeight());
    }
    //cout<<"Overlap "<<fr_overlap<<endl;

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
  if((vol2 > 0) && (num1 > 0)){
    //kule = (pnf * vol1) / (num1 * vol2);
    kule = vol1 / vol2;
  }
  else kule = 0;

// inner divergence formula
  score = computeInnerDivCore(kule);
  
  return(1);	  
}

double GanitaMetrics::computeInnerDivCore(double mykule)
{
  double score2;
  score2 = 0;

// inner divergence formula
// Using an environment variable to set the different modes.
  if((gm_kl_mode == 0) || (gm_kl_mode == 2)){
    if(mykule > 0){
      score2 = -1*mykule*log2(mykule);
      if(verbosity > 1){
	cout<<"Average overlap per frame "<<mykule<<" KL-score "<<score2<<endl;
      }
    }
    else{
      score2 = 0;
      //cout<<"Average overlap per frame "<<mykule<<", KL-score "<<score2<<endl;
    }
  }
  else if(gm_kl_mode == 1){
    // Try alternative inner divergence score formula.
    if(mykule < 1){
      score2 = -1*(1 - mykule)*log2(1 - mykule);
      if(verbosity > 1){
	cout<<"Average overlap per frame "<<mykule<<" KL-score1 "<<score2<<endl;
      }
    }
    else{
      score2 = 0;
      //cout<<"Average overlap per frame "<<mykule<<", KL-score "<<score2<<endl;
    }
  }
  if(gm_kl_mode == 2){
       // Try alternative inner divergence score formula.
    if(mykule < 1){
      score2 += -1*(1 - mykule)*log2(1 - mykule);
      if(verbosity > 1){
	cout<<"Average overlap per frame "<<mykule<<" KL-score2 "<<-1*(1 - mykule)*log2(1 - mykule)
	    <<endl;
	cout<<"Average overlap per frame "<<mykule<<" KL-score3 "<<score2<<endl;
      }
    }
  }
  else if(gm_kl_mode > 2){
    // Another alternative inner divergence without log2.
    if(mykule < 1){
      score2 = 2*mykule*(1 - mykule);
      if(verbosity > 1){
	cout<<"Average overlap per frame "<<mykule<<" KL-score "<<score2<<endl;
      }
    }
    else{
      score2 = 0;
      //cout<<"Average overlap per frame "<<mykule<<", KL-score "<<score<<endl;
    }
  }

  return(score2);
}

int GanitaMetrics::computeTrackPairOuterDiv
(int64_t ref_nn, int64_t sys_nn, int flip, double& score)
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
  double vol1, vol2;
  uint64_t pnf;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0; pnf = 0;
  score = 0;

  refTrack = gmts[flip].returnTrack(ref_nn);
  sysTrack = gmts[1-flip].returnTrack(sys_nn);
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

  if(verbosity > 1){
    cout<<"ii = "<<ii<<" "<<"jj = "<<jj<<endl;
  }

  num1 = refTrack->returnNumberOfTopDetections();
  num2 = sysTrack->returnNumberOfTopDetections();

  vol1 = 0;
  vol2 = 0;
  kule = 0;
  final_frame = final_frame1;
  while(ff<=final_frame){
    refTrack->returnTopGMD(ii, gmd1);
    ff1 = gmd1.returnFrameNumber();
    sysTrack->returnTopGMD(jj, gmd2);
    ff2 = gmd2.returnFrameNumber();
    
    if(verbosity > 2){
      cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;
      cout<<"Values: "<<gmd1.returnX_Anchor()<<":"<<gmd1.returnY_Anchor()<<":"
	  <<gmd1.returnWidth()<<":"<<gmd1.returnHeight()<<":"<<endl;
      cout<<"Values: "<<gmd2.returnX_Anchor()<<":"<<gmd2.returnY_Anchor()<<":"
	  <<gmd2.returnWidth()<<":"<<gmd2.returnHeight()<<":"<<endl;
    }

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

    //cout<<"Computed stats on frame "<<ff<<endl;
    if((left_x1 >= right_x2) || (lower_y1 >= upper_y2)){
      // There is no overlap
      fr_overlap = 0;
    }
    else{
      fr_overlap = (upper_y2 - lower_y1)*(right_x2 - left_x1);
      if(verbosity > 1){
	cout<<"Overlap ("<<left_x1<<","<<lower_y1<<") "<<"("<<right_x2<<","<<upper_y2<<")"<<endl;
      }
//       if(gmd1.returnAuxValid() == 1){
// 	cout<<"Expected overlap"<<endl;
//       }
    }
    if((gmd1.returnWidth() <= 0) || (gmd1.returnHeight() <= 0)){
      //skip
    }
    else{
      pnf++;
      vol1 += ((double) fr_overlap) / 10000.0;
      vol2 += ((double) (gmd1.returnWidth() * gmd1.returnHeight())) / 10000.0;
      //kule += ((double) fr_overlap) / (gmd1.returnWidth() * gmd1.returnHeight());
    }
    //cout<<"Overlap "<<fr_overlap<<endl;

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
  if((vol2 > 0) && (num1 > 0)){
    kule = (pnf * vol1) / (num1 * vol2);
  }
  else kule = 0;

  if(kule > 0){
    score = -1*kule*log2(kule);
    if(verbosity > 1){
      cout<<"Average overlap per frame "<<kule<<" KL-score "<<score<<endl;
    }
  }
  else{
    score = 0;
    //cout<<"Average overlap per frame "<<kule<<", KL-score "<<score<<endl;
  }
  
  return(1);	  
}

int GanitaMetrics::computeTrackKL(int64_t ref_nn, double iKL, int flip, double& scoreKL)
{
  uint64_t num, ii;
  double score;

  scoreKL = 0;
  num = gmts[1-flip].returnNumTracks();
  for(ii=0; ii<num; ii++){
    computeTrackPairKL(ref_nn, ii, flip, score);
    scoreKL += score;
  }

  score = scoreKL;
  if(scoreKL > iKL){
    scoreKL -= iKL;
  }
  else scoreKL = 0;
  if(verbosity > 0){
    cout<<"Raw KL-score ("<<score<<") Inner entropy ("<<iKL<<") Final KL-score ("
      <<scoreKL<<")"<<endl;
  }
  return(1);
}

int GanitaMetrics::computeTrackOuterDiv(int64_t ref_nn, double iKL, int flip, double& scoreKL)
{
  uint64_t num, ii;
  double score;

  scoreKL = 0;
  num = gmts[1-flip].returnNumTracks();
  for(ii=0; ii<num; ii++){
    computeTrackPairKL(ref_nn, ii, flip, score);
    scoreKL += score;
  }

  score = scoreKL;
  if(scoreKL > iKL){
    scoreKL -= iKL;
  }
  else scoreKL = 0;
  if(verbosity > 0){
    cout<<"Raw KL-score ("<<score<<") Inner entropy ("<<iKL<<") Final KL-score ("
      <<scoreKL<<")"<<endl;
  }
  return(1);
}

int GanitaMetrics::purifyTrackKL(int64_t ref_nn, int ref_or_sys, double& crossKL)
{
  int64_t ii, num;
  double score;

  crossKL = 0;
  num = gmts[ref_or_sys].returnNumTracks();
  for(ii=0; ii<num; ii++){
    if(ii != ref_nn){
      purifyTrackPairKL(ref_nn, ii, ref_or_sys, score);
      crossKL += score;
    }
  }

  return(1); 
}

int GanitaMetrics::purifyTrackPairKL(int64_t ref_nn, int64_t sys_nn, int ref_or_sys, double& score)
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
  double vol1, vol2;
  uint64_t pnf;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0; pnf = 0;
  score = 0;

  refTrack = gmts[ref_or_sys].returnTrack(ref_nn);
  sysTrack = gmts[ref_or_sys].returnTrack(sys_nn);
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

  //  cout<<"ii = "<<ii<<" "<<"jj = "<<jj<<endl;

  num1 = refTrack->returnNumberOfTopDetections();
  num2 = sysTrack->returnNumberOfTopDetections();

  vol1 = 0;
  vol2 = 0;
  kule = 0;
  final_frame = final_frame1;
  while(ff<=final_frame){
    refTrack->returnTopGMD(ii, gmd1);
    ff1 = gmd1.returnFrameNumber();
    sysTrack->returnTopGMD(jj, gmd2);
    ff2 = gmd2.returnFrameNumber();
    
    if(verbosity > 2){
      cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;
      cout<<"Values: "<<gmd1.returnX_Anchor()<<":"<<gmd1.returnY_Anchor()<<":"
	  <<gmd1.returnWidth()<<":"<<gmd1.returnHeight()<<":"<<endl;
      cout<<"Values: "<<gmd2.returnX_Anchor()<<":"<<gmd2.returnY_Anchor()<<":"
	  <<gmd2.returnWidth()<<":"<<gmd2.returnHeight()<<":"<<endl;
    }
      
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

//     cout<<"Computed stats on frame "<<ff<<endl;
    if((left_x1 >= right_x2) || (lower_y1 >= upper_y2)){
      // There is no overlap
      fr_overlap = 0;
    }
    else{
      fr_overlap = (upper_y2 - lower_y1)*(right_x2 - left_x1);
      if(verbosity > 1){
	cout<<"Overlap* ("<<left_x1<<","<<lower_y1<<") "<<"("<<right_x2<<","<<upper_y2<<")"<<endl;
      }
//       gmd1.setAuxValid();
//       gmd1.pushAux(left_x1);
//       gmd1.pushAux(lower_y1);
//       gmd1.pushAux(right_x2);
//       gmd1.pushAux(upper_y2);
    }
    if((gmd1.returnWidth() <= 0) || (gmd1.returnHeight() <= 0)){
      //skip
    }
    else{
      pnf++;
      vol1 += ((double) fr_overlap) / 10000.0;
      vol2 += ((double) (gmd1.returnWidth() * gmd1.returnHeight())) / 10000.0;
      //kule += ((double) fr_overlap) / (gmd1.returnWidth() * gmd1.returnHeight());
    }
    //cout<<"Overlap "<<fr_overlap<<endl;

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
  if((vol2 > 0) && (num1 > 0)){
    //kule = (pnf * vol1) / (num1 * vol2);
    kule = vol1 / vol2;
 }
  else kule = 0;

  score = computeInnerDivCore(kule);
  
  return(1);	  
}

int GanitaMetrics::purifyTrackPairKL_2
(int64_t ref_nn, int64_t sys_nn, int ref_or_sys, double& score)
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
  double vol1, vol2;
  uint64_t pnf;
  //uint64_t num;
 
  ii = 0; jj = 0;
  ff = 0; pnf = 0;
  score = 0;

  refTrack = gmts[ref_or_sys].returnTrack(ref_nn);
  sysTrack = gmts[ref_or_sys].returnTrack(sys_nn);
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

  //  cout<<"ii = "<<ii<<" "<<"jj = "<<jj<<endl;

  num1 = refTrack->returnNumberOfTopDetections();
  num2 = sysTrack->returnNumberOfTopDetections();

  vol1 = 0;
  vol2 = 0;
  kule = 0;
  final_frame = final_frame1;
  while(ff<=final_frame){
    refTrack->returnTopGMD(ii, gmd1);
    ff1 = gmd1.returnFrameNumber();
    sysTrack->returnTopGMD(jj, gmd2);
    ff2 = gmd2.returnFrameNumber();
    
    if(verbosity > 2){
      cout<<"Frame numbers "<<ff1<<" "<<ff2<<endl;
      cout<<"Values: "<<gmd1.returnX_Anchor()<<":"<<gmd1.returnY_Anchor()<<":"
	  <<gmd1.returnWidth()<<":"<<gmd1.returnHeight()<<":"<<endl;
      cout<<"Values: "<<gmd2.returnX_Anchor()<<":"<<gmd2.returnY_Anchor()<<":"
	  <<gmd2.returnWidth()<<":"<<gmd2.returnHeight()<<":"<<endl;
    }
      
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

//     cout<<"Computed stats on frame "<<ff<<endl;
    if((left_x1 >= right_x2) || (lower_y1 >= upper_y2)){
      // There is no overlap
      fr_overlap = 0;
    }
    else{
      fr_overlap = (upper_y2 - lower_y1)*(right_x2 - left_x1);
      if(verbosity > 8){
	cout<<"Overlap* ("<<left_x1<<","<<lower_y1<<") "<<"("<<right_x2<<","<<upper_y2<<")"<<endl;
      }
//       gmd1.setAuxValid();
//       gmd1.pushAux(left_x1);
//       gmd1.pushAux(lower_y1);
//       gmd1.pushAux(right_x2);
//       gmd1.pushAux(upper_y2);
    }
    if((gmd1.returnWidth() <= 0) || (gmd1.returnHeight() <= 0)){
      //skip
    }
    else{
      pnf++;
      vol1 += ((double) fr_overlap) / 10000.0;
      vol2 += ((double) (gmd1.returnWidth() * gmd1.returnHeight())) / 10000.0;
      //kule += ((double) fr_overlap) / (gmd1.returnWidth() * gmd1.returnHeight());
    }
    //cout<<"Overlap "<<fr_overlap<<endl;

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
  if((vol2 > 0) && (num1 > 0)){
    kule = (pnf * vol1) / (num1 * vol2);
  }
  else kule = 0;
  if(kule > 0){
    score = -1*kule*log2(kule);
    if(verbosity > 1){
      cout<<"Average overlap per frame "<<kule<<" KL-score "<<score<<endl;
    }
  }
  else{
    score = 0;
    //cout<<"Average overlap per frame "<<kule<<", KL-score "<<score<<endl;
  }
  
  return(1);	  
}

// Input 0 for conditioning on reference and 1 for system
int GanitaMetrics::computeMeanTrackKL(int ref_or_sys, double& track_error)
{
  double innerKL, scoreKL, totalKL;
  int64_t num, ii;
  
  totalKL = 0;
  num = gmts[ref_or_sys].returnNumTracks();
  if(num <= 0) return(-1);
  for(ii=0; ii<num; ii++){
    if(verbosity > 0){
      cout<<"Processing ref track ("<<ii<<")"<<endl;
    }
    purifyTrackKL(ii, ref_or_sys, innerKL);
    //fprintf(stdout, "Computing KL-divergence score."); fflush(stdout);
    computeTrackKL(ii, innerKL, ref_or_sys, scoreKL);
    totalKL += scoreKL;
  }
  totalKL /= num;
  //cout<<"Total tracking error ("<<totalKL<<")"<<endl;
  track_error = totalKL;

  return(1);
}

int GanitaMetrics::processOuterDiv_1(uint64_t fr_num, uint64_t tr_num, GanitaMetricsMat& rMat, 
				     int set1, int set2)
{
  uint64_t kk, uu, vv;
  uint64_t ww, hh;
  int64_t iref, isys;
  GanitaMetricsTopDetection gmd1, gmd2;
  std::shared_ptr< GanitaMetricsTrack > refTrack, sysTrack;
  double ref_x1, sys_x1;
  double ref_x2, sys_x2;
  double ref_y1, sys_y1;
  double ref_y2, sys_y2;
  //double no_overlap;

  //no_overlap = 0;
  refTrack = gmts[set1].returnTrack(tr_num);
  iref = refTrack->returnFrameIndex(fr_num);
  if(iref >= 0){
    refTrack->returnTopGMD(iref, gmd1);
    ref_x1 = gmd1.returnX_Anchor();
    ref_y1 = gmd1.returnY_Anchor();
    ww = gmd1.returnWidth(); hh = gmd1.returnHeight();
    ref_x2 = ref_x1 + ww;
    ref_y2 = ref_y1 + hh;
    for(kk=0; kk<gmts[set2].returnNumTracks(); kk++){
      sysTrack = gmts[set2].returnTrack(kk);
      isys = sysTrack->returnFrameIndex(fr_num);
      if(verbosity > 1){
	if((fr_num==5) && (iref == 6)){
	  std::cout<<"(Frame,rTrack,sTrack,rId,sId)=("<<fr_num<<","<<tr_num<<","
		   <<kk<<","<<iref<<","<<isys<<")"<<std::endl;
	}
      }
      if(isys >= 0){
	// Both refTrack and sysTrack have boxes on this frame
	sysTrack->returnTopGMD(isys, gmd2);
	sys_x1 = gmd2.returnX_Anchor();
	sys_y1 = gmd2.returnY_Anchor();
	sys_x2 = sys_x1 + gmd2.returnWidth();
	sys_y2 = sys_y1 + gmd2.returnHeight();
      if(verbosity > 1){
	if((fr_num==5) && (iref == 6)){
	  std::cout<<"(ref x1,ref x2, ref y1, ref y2)=("<<ref_x1<<","<<ref_x2<<","
		   <<ref_y1<<","<<ref_y2<<")"<<std::endl;
	  std::cout<<"(sys x1, sys x2, sys y1, sys y2)=("<<sys_x1<<","<<sys_x2<<","
		   <<sys_y1<<","<<sys_y2<<")"<<std::endl;
	  std::cout<<"(fr_id, fr_num, sys x1, sys y1)=("<<gmd2.returnId()<<","<<gmd2.returnFrameNumber()
		   <<","<<gmd2.returnX_Anchor()<<","<<gmd2.returnY_Anchor()<<")"<<std::endl;
	}
      }
	// Find any overlap between refTrack and sysTrack	    
	if((sys_x1 < ref_x2) && (sys_x2 > ref_x1) && 
	   (sys_y1 < ref_y2) && (sys_y2 > ref_y1)){
	  // there is an overlap
	  if(verbosity > 1){
	    //if(iref == 8){
	      std::cout<<"(Frame,rTrack,sTrack,rId,sId)=("<<fr_num<<","<<tr_num<<","
		       <<kk<<","<<iref<<","<<isys<<")"<<std::endl;
	      //}
	  }
 	  for(uu=0; uu<ww; uu++){
	    for(vv=0; vv<hh; vv++){
	      if((ref_x1 + uu >= sys_x1) && (ref_x1 + uu < sys_x2) && 
		 (ref_y1 + vv >= sys_y1) && (ref_y1 + vv < sys_y2)){
		rMat.set(uu,vv,rMat.get(uu,vv)+1);
	      }
	    }
	  }
	}
      }
    }
  }
  
  return(1);
}

int GanitaMetrics::processOuterDiv_2(uint64_t fr_num, int tset)
{
  uint64_t kk;
  uint64_t ww, hh;
  int64_t iref;
  GanitaMetricsTopDetection gmd1;
  std::shared_ptr< GanitaMetricsTrack > refTrack;

  if(verbosity > 1){
    cout<<"Processing frame ... "<<fr_num<<" ... Tracks ..."<<endl;
  }
  for(kk=0; kk<gmts[tset].returnNumTracks(); kk++){
    refTrack = gmts[tset].returnTrack(kk);
    iref = refTrack->returnFrameIndex(fr_num);
    if(verbosity > 1){
      std::cout<<"Frame ("<<fr_num<<") Matching frame index ("<<iref<<")"<<std::endl;
    }
    if(iref >= 0){
      if(verbosity > 1){
	cout<<kk<<":";
      }
      refTrack->returnTopGMD(iref, gmd1);
      ww = gmd1.returnWidth(); hh = gmd1.returnHeight();
      GanitaMetricsMat rMat1(ww, hh);
      GanitaMetricsMat rMat2(ww, hh);
      processOuterDiv_1(fr_num, kk, rMat1, tset, 1 - tset);
      processOuterDiv_1(fr_num, kk, rMat2, tset, tset);
      updateStats(tset, kk, rMat1, rMat2);
      rMat1.close();
      rMat2.close();
//       cout<<"Track("<<kk<<") "<<"Frame("<<fr_num<<") "<<"("<<refTrack->stats[0]<<") "
// 	  <<"("<<refTrack->stats[1]<<") "<<"("<<refTrack->stats[2]<<")"<<endl;
    }
  }
  if(verbosity > 1){
    cout<<endl;
  }

  return(1);
}

int GanitaMetrics::processOuterDiv_3(int tset)
{
  uint64_t ii;
  uint64_t min_frame, max_frame;
  std::shared_ptr< GanitaMetricsTrack > refTrack;
  double score, alpha;
  double tscore1, tscore2, tscore3;
  double densityKL;

  tscore1 = 0; tscore2 = 0; tscore3 = 0;
  for(ii=0; ii<gmts[tset].returnNumTracks(); ii++){
    refTrack = gmts[tset].returnTrack(ii);
    refTrack->initStats(3);
  }

  min_frame = gmts[tset].returnStart();
  max_frame = gmts[tset].returnEnd();
  for(ii=min_frame; ii<=max_frame; ii++){
    processOuterDiv_2(ii, tset);
  }

  for(ii=0; ii<gmts[tset].returnNumTracks(); ii++){
    refTrack = gmts[tset].returnTrack(ii);
    if(verbosity > 0){
      cout<<"Track("<<ii<<") Stats "<<"("<<refTrack->stats[0]<<") "
	  <<"("<<refTrack->stats[1]<<") "<<"("<<refTrack->stats[2]<<")"<<endl;
    }
    if(refTrack->stats[0] > 0){
      alpha = (refTrack->stats[0] - refTrack->stats[1]) / refTrack->stats[0];
      densityKL = refTrack->stats[2] / refTrack->stats[0];
      
    }
    else{
      // This should only happen for tracks with no volumes.
      alpha = 0; densityKL = 0;
    }
    // Process outer divergence score formula.
    score = log2( (2.0 + gmts[1-tset].returnNumTracks()) 
    		  / (1.0 + alpha * (1 + gmts[1-tset].returnNumTracks())) );
    // Modified formula to use the number of tracks for tset rather than (1 - tset). 
//     score = log2( ((double) 1 + gmts[tset].returnNumTracks()) 
// 		 / ((double) 1 + alpha * gmts[tset].returnNumTracks()) );
    if(verbosity > 0){
      cout<<"Outer divergence ("<<score<<") Missed detection ("<<1 - alpha<<")"<<endl;
    }
    tscore1 += score / (1 + gmts[1-tset].returnNumTracks());
    tscore2 += (1 - alpha) / gmts[tset].returnNumTracks();
    tscore3 += densityKL / gmts[tset].returnNumTracks();
  }

  if(tset == 0){
    gmScores.push_back(tscore1);
    gmScoreType.push_back("Missed detection error         ");
    gmScores.push_back(tscore2);
    gmScoreType.push_back("Missed detection proportion    ");
    gmScores.push_back(tscore3);
    gmScoreType.push_back("Density error rel to reference ");
  }
  else{
    gmScores.push_back(tscore1);
    gmScoreType.push_back("False alarm error              ");
    gmScores.push_back(tscore2);
    gmScoreType.push_back("False alarm proportion         ");
    gmScores.push_back(tscore3);
    gmScoreType.push_back("Density error rel to system    ");
  }
  //cout<<"Final missed detection or false alarm score ("<<tscore1<<")"<<endl;
  //cout<<"Average missed detection or false alarm proportion ("<<tscore2<<")"<<endl;
  //cout<<"Density difference score ("<<tscore3<<")"<<endl;

  return(1);
}

int GanitaMetrics::updateStats(int tset, uint64_t tr_num, 
			       GanitaMetricsMat rMat1, GanitaMetricsMat rMat2)
{
  uint64_t ii, jj;
  double no_overlap, kL;
  std::shared_ptr< GanitaMetricsTrack > refTrack;
  double myratio;

  no_overlap = 0;
  kL = 0;

  refTrack = gmts[tset].returnTrack(tr_num);
  if(refTrack->stats.size() <= 0){
    // need to initialize stats first
    return(-1);
  }
  if(rMat1.returnNCols() * rMat1.returnNRows() <= 0){ return(-1); }
  refTrack->stats[0] += rMat1.returnNCols() * rMat1.returnNRows();
  for(ii=0; ii<rMat1.returnNCols(); ii++){
    for(jj=0; jj<rMat1.returnNRows(); jj++){
      if(rMat1.get(ii, jj) == 0){
	// no overlap
	no_overlap++;
      }
      else{
	// there is overlap
	// compute KL-divergence difference between distributions
	// then add this to the stats
	if(rMat2.get(ii, jj) == 0){
	  // this probably shouldn't happen
	  // possibly we excluded the refTrack
	  //kL += log2((double) rMat1.get(ii, jj));
	  myratio = log2((double) rMat1.get(ii, jj));
	  kL += myratio * log2(myratio);
	  if(verbosity > 2){
	    cout<<tr_num<<":"<<ii<<":"<<jj<<":"<<kL<<endl;
	  }
	}
	else{
	  if(rMat2.get(ii, jj) < rMat1.get(ii, jj)){
	    //kL += log2(((double) rMat1.get(ii, jj)) / ((double) rMat2.get(ii, jj)));
	    myratio = ((double) rMat1.get(ii, jj)) / ((double) rMat2.get(ii, jj));
	    kL += myratio * log2(myratio);
	    if(verbosity > 2){
	      if(kL > 0){
		cout<<"kL:"<<tr_num<<":"<<ii<<":"<<jj<<":"<<kL<<endl;
	      }
	    }
	  }
	  else{
	    // Next lines may be commented out.
	    // If not commented out, may lead to double counting errors.
// 	    kL += log2(((double) rMat2.get(ii, jj)) / ((double) rMat1.get(ii, jj)));
// 	    if(kL > 0){
// 	      cout<<"kL:"<<tr_num<<":"<<ii<<":"<<jj<<":"<<kL<<endl;
// 	    }
	  }
	}
      }
    }
  }
  refTrack->stats[1] += no_overlap;
  if(verbosity > 1){
    std::cout<<"No overlap ("<<((double) no_overlap) / ((double) rMat1.returnNCols() * rMat1.returnNRows())
	     <<")"<<std::endl;
    std::cout<<"Total no overlap ("
	     <<((double) refTrack->stats[1]) / ((double) refTrack->stats[1])
	     <<")"<<std::endl;
  }
  //refTrack->stats[2] += (kL / (rMat1.returnNCols() * rMat1.returnNRows()));
  refTrack->stats[2] += kL;
  //cout<<"Added kL:"<<tr_num<<":"<<kL<<endl;

  return(1);
}

int GanitaMetrics::printSummary(void)
{
  if(gmScores.size() < 9){ return(-1); }
  gmScores.push_back(gmScores[2] + gmScores[3]+gmScores[5]+gmScores[6]+gmScores[8]);
  gmScoreType.push_back("Total KL-track error           ");
  fprintf(stdout, "***************************************************\n");
  fprintf(stdout, "*                    Summary                      *\n");
  fprintf(stdout, "***************************************************\n");
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[0].c_str(), gmScores[0]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[1].c_str(), gmScores[1]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[2].c_str(), gmScores[2]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[3].c_str(), gmScores[3]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[4].c_str(), gmScores[4]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[5].c_str(), gmScores[5]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[6].c_str(), gmScores[6]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[7].c_str(), gmScores[7]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[8].c_str(), gmScores[8]);
  fprintf(stdout, "*-------------------------------------------------*\n");
  fprintf(stdout, "* %s \t=%lf *\n", gmScoreType[9].c_str(), gmScores[9]);
  fprintf(stdout, "***************************************************\n");

  return(1);
}

int GanitaMetrics::testSummary(void)
{
  gmScores.push_back(0.286446);
  gmScoreType.push_back("Inner div relative to reference");
  gmScores.push_back(0.44522);
  gmScoreType.push_back("Inner div relative to system   ");
  gmScores.push_back(0.731667);
  gmScoreType.push_back("Total inner div error          ");
  gmScores.push_back(0.96975);
  gmScoreType.push_back("Missed detection score         ");
  gmScores.push_back(0.438332);
  gmScoreType.push_back("Missed detection proportion    ");
  gmScores.push_back(0.0821141);
  gmScoreType.push_back("Density score rel to reference ");
  gmScores.push_back(0.244259);
  gmScoreType.push_back("False alarm score              ");
  gmScores.push_back(0.198179);
  gmScoreType.push_back("False alarm proportion         ");
  gmScores.push_back(0.0896467);
  gmScoreType.push_back("Density score rel to system    ");
  gmScores.push_back(gmScores[2] + gmScores[3]+gmScores[5]+gmScores[6]+gmScores[8]);
  gmScoreType.push_back("Total KL-track error           ");

  if(gmScores.size() < 9){ return(-1); }
  fprintf(stdout, "***************************************************\n");
  fprintf(stdout, "*                    Summary                      *\n");
  fprintf(stdout, "***************************************************\n");
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[0].c_str(), gmScores[0]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[1].c_str(), gmScores[1]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[2].c_str(), gmScores[2]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[3].c_str(), gmScores[3]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[4].c_str(), gmScores[4]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[5].c_str(), gmScores[5]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[6].c_str(), gmScores[6]);
  fprintf(stdout, "* %s \t %lf *\n", gmScoreType[7].c_str(), gmScores[7]);
  fprintf(stdout, "* %s \t+%lf *\n", gmScoreType[8].c_str(), gmScores[8]);
  fprintf(stdout, "*-------------------------------------------------*\n");
  fprintf(stdout, "* %s \t=%lf *\n", gmScoreType[9].c_str(), gmScores[9]);
  fprintf(stdout, "***************************************************\n");

  return(1);
}

uint64_t GanitaMetrics::setMajorResolution()
{
  // try computing resolution
  gmts[0].computeResolution();
  gmts[1].computeResolution();
  
  if((gmts[0].returnFrameWidth() == 0) ||(gmts[1].returnFrameWidth() == 0) || 
     (gmts[0].returnFrameHeight() == 0) ||(gmts[1].returnFrameHeight() == 0)){
    // something went wrong
    return(0);
  }

  if(gmts[0].returnFrameWidth() > gmts[1].returnFrameWidth()){
    major_width = gmts[0].returnFrameWidth();
  }
  else{
    major_width = gmts[1].returnFrameWidth();
  }

  if(gmts[0].returnFrameHeight() > gmts[1].returnFrameHeight()){
    major_height = gmts[0].returnFrameHeight();
  }
  else{
    major_height = gmts[1].returnFrameHeight();
  }

  return(major_height);
}

int GanitaMetrics::clipDetectionBoxes(void)
{
  // fixed resolution is stored internally for truth and system track sets
  // now clip the width and height of each detection box
  gmts[0].clipDetectionBoxes();
  gmts[1].clipDetectionBoxes();
  
  return(1);
}

uint64_t GanitaMetrics::returnMajorWidth()
{
  return(major_width);
}

uint64_t GanitaMetrics::returnMajorHeight()
{
  return(major_height);
}

uint64_t GanitaMetrics::setMajorWidth(uint64_t ww)
{
  major_width = ww;
  gmts[0].setFrameWidth(major_width);
  gmts[1].setFrameWidth(major_width);
  return(major_width);
}

uint64_t GanitaMetrics::setMajorHeight(uint64_t hh)
{
  major_height = hh;
  gmts[0].setFrameHeight(major_height);
  gmts[1].setFrameHeight(major_height);
  return(major_height);
}

