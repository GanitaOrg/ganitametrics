// This file contains the methods for GanitaMetricsTrack. 

#include "ganita/metrics/GanitaMetricsTrack.hpp"

// Constructors
GanitaMetricsTrack::GanitaMetricsTrack(void)
{
  id = 0;
  track_start = 0;
  track_end = 0;
  confidence = 0;
  verbosity = 0;
}

int64_t GanitaMetricsTrack::addTopDetection(void)
{
  GanitaMetricsTopDetection *newdetection = new GanitaMetricsTopDetection();
  gtdetections.push_back(std::make_shared<GanitaMetricsTopDetection>(*newdetection));
  delete newdetection;
  return(gtdetections.size());
}

int64_t GanitaMetricsTrack::addTopDetection(int64_t new_id,
					    int64_t new_frame_number,
					    int new_headValid,
					    int new_bodyValid,
					    double new_headLeft,
					    double new_headTop,
					    double new_headRight,
					    double new_headBottom,
					    double new_bodyLeft,
					    double new_bodyTop,
					    double new_bodyRight,
					    double new_bodyBottom,
					    double new_confidence,
					    int new_verbosity)
{
  GanitaMetricsTopDetection *newdetection = new GanitaMetricsTopDetection();
  newdetection->setValues(new_id, new_frame_number, new_headValid, new_bodyValid, 
			  new_headLeft, new_headTop, new_headRight, new_headBottom,
			  new_bodyLeft, new_bodyTop, new_bodyRight, new_bodyBottom,
			  new_confidence, new_verbosity);
  gtdetections.push_back(std::make_shared<GanitaMetricsTopDetection>(*newdetection));
  delete newdetection;
  return(gtdetections.size());
}

int GanitaMetricsTrack::returnTopGMD(uint64_t nn, GanitaMetricsTopDetection& gmd)
{
  if(nn >= gtdetections.size()){
    return(-1);
  }
  gmd = *gtdetections[nn];

  return(gtdetections.size());
}

int64_t GanitaMetricsTrack::addMotDetection(void)
{
  GanitaMetricsMotDetection *newdetection = new GanitaMetricsMotDetection();
  gmdetections.push_back(std::make_shared<GanitaMetricsMotDetection>(*newdetection));
  delete newdetection;
  return(gmdetections.size());
}

int64_t GanitaMetricsTrack::addMotDetection(int64_t new_frame_number,
					 int64_t new_id,
				  double new_x_anchor,
				  double new_y_anchor,
				  double new_width,
				  double new_height,
				  double new_confidence,
				  double new_x_world,
				  double new_y_world,
				  double new_z_world,
				  int new_verbosity)
{
  GanitaMetricsMotDetection *newdetection = new GanitaMetricsMotDetection();
  newdetection->setValues(new_frame_number, new_id, new_x_anchor, new_y_anchor, 
			  new_width, new_height, new_confidence, 
			  new_x_world, new_y_world, new_z_world, 
			  new_verbosity);
  gmdetections.push_back(std::make_shared<GanitaMetricsMotDetection>(*newdetection));
  delete newdetection;
  return(gmdetections.size());
}

int GanitaMetricsTrack::returnMotGMD(uint64_t nn, GanitaMetricsMotDetection& gmd)
{
  if(nn >= gmdetections.size()){
    return(-1);
  }
  gmd = *gmdetections[nn];

  return(gmdetections.size());
}

uint64_t GanitaMetricsTrack::returnNumberOfTopDetections(void)
{
  return(gtdetections.size());
}

