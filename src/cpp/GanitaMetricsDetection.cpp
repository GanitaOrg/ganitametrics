// This file contains the methods for GanitaMetricsDetection. 

#include "ganita/metrics/GanitaMetricsDetection.hpp"

// Constructors
GanitaMetricsDetection::GanitaMetricsDetection(void)
{
  id = 0;
  frame_number = 0;
  x_anchor = 0;
  y_anchor = 0;
  width = 0;
  height = 0;
  confidence = 0;
  x_world = 0;
  y_world = 0;
  z_world = 0;
  verbosity = 0;
}

GanitaMetricsDetection::GanitaMetricsDetection(int vv)
{
  id = 0;
  frame_number = 0;
  x_anchor = 0;
  y_anchor = 0;
  width = 0;
  height = 0;
  confidence = 0;
  x_world = 0;
  y_world = 0;
  z_world = 0;
  verbosity = vv;
}

int GanitaMetricsDetection::setValues(
				  int64_t new_frame_number,
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
  frame_number = new_frame_number;
  id = new_id;
  x_anchor = new_x_anchor;
  y_anchor = new_y_anchor;
  width = new_width;
  height = new_height;
  confidence = new_confidence;
  x_world = new_x_world;
  y_world = new_y_world;
  z_world = new_z_world;
  verbosity = new_verbosity;

  return(1);
}

int64_t GanitaMetricsDetection::returnId(void)
{
  return(id);
}

int64_t GanitaMetricsDetection::returnFrameNumber(void)
{
  return(frame_number);
}

double GanitaMetricsDetection::returnX_Anchor(void)
{
  return(x_anchor);
}

double GanitaMetricsDetection::returnY_Anchor(void)
{
  return(y_anchor);
}

double GanitaMetricsDetection::returnWidth(void)
{
  return(width);
}

double GanitaMetricsDetection::returnHeight(void)
{
  return(height);
}

double GanitaMetricsDetection::returnConfidence(void)
{
  return(confidence);
}

