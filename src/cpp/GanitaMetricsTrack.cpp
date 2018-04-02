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

int64_t GanitaMetricsTrack::addDetection(void)
{
  GanitaMetricsDetection *newdetection = new GanitaMetricsDetection();
  gmdetections.push_back(std::make_shared<GanitaMetricsDetection>(*newdetection));
  delete newdetection;
  return(gmdetections.size());
}

int64_t GanitaMetricsTrack::addDetection(int64_t new_frame_number,
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
  GanitaMetricsDetection *newdetection = new GanitaMetricsDetection();
  newdetection->setValues(new_frame_number, new_id, new_x_anchor, new_y_anchor, 
			  new_width, new_height, new_confidence, 
			  new_x_world, new_y_world, new_z_world, 
			  new_verbosity);
  gmdetections.push_back(std::make_shared<GanitaMetricsDetection>(*newdetection));
  delete newdetection;
  return(gmdetections.size());
}

int GanitaMetricsTrack::returnGMD(uint64_t nn, GanitaMetricsDetection& gmd)
{
  if(nn >= gmdetections.size()){
    return(-1);
  }
  gmd = *gmdetections[nn];

  return(gmdetections.size());
}

