// This file contains the methods for GanitaMetricsDetection. 

#include "ganita/metrics/GanitaMetricsTopDetection.hpp"

// Constructors
GanitaMetricsTopDetection::GanitaMetricsTopDetection(void)
{
  id = 0;
  frame_number = 0;
  headValid = 0;
  bodyValid = 0;
  headLeft = 0;
  headTop = 0;
  headRight = 0;
  headBottom = 0;
  bodyLeft = 0;
  bodyTop = 0;
  bodyRight = 0;
  bodyBottom = 0;
  confidence = 0;
  verbosity = 0;
  auxValid = 0;
}

GanitaMetricsTopDetection::GanitaMetricsTopDetection(int vv)
{
  id = 0;
  frame_number = 0;
  headValid = 0;
  bodyValid = 0;
  headLeft = 0;
  headTop = 0;
  headRight = 0;
  headBottom = 0;
  bodyLeft = 0;
  bodyTop = 0;
  bodyRight = 0;
  bodyBottom = 0;
  confidence = 0;
  verbosity = vv;
  auxValid = 0;
}

int GanitaMetricsTopDetection::setValues(
				  int64_t new_id,
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
  id = new_id;
  frame_number = new_frame_number;
  headValid = new_headValid;
  bodyValid = new_bodyValid;
  headLeft = new_headLeft;
  headTop = new_headTop;
  headRight = new_headRight;
  headBottom = new_headBottom;
  bodyLeft = new_bodyLeft;
  bodyTop = new_bodyTop;
  bodyRight = new_bodyRight;
  bodyBottom = new_bodyBottom;
  confidence = new_confidence;
  verbosity = new_verbosity;

  return(1);
}

int64_t GanitaMetricsTopDetection::returnId(void)
{
  return(id);
}

int64_t GanitaMetricsTopDetection::returnFrameNumber(void)
{
  return(frame_number);
}

double GanitaMetricsTopDetection::returnX_Anchor(void)
{
  return(bodyLeft);
}

double GanitaMetricsTopDetection::returnY_Anchor(void)
{
  return(bodyTop);
}

double GanitaMetricsTopDetection::returnWidth(void)
{
  return(bodyRight - bodyLeft);
}

double GanitaMetricsTopDetection::returnHeight(void)
{
  return(bodyBottom - bodyTop);
}

double GanitaMetricsTopDetection::returnBodyRight(void)
{
  return(bodyRight);
}

double GanitaMetricsTopDetection::returnBodyBottom(void)
{
  return(bodyBottom);
}

double GanitaMetricsTopDetection::setBodyRight(double br)
{
  bodyRight = br;
  return(bodyRight - bodyLeft);
}

double GanitaMetricsTopDetection::setBodyBottom(double bb)
{
  bodyBottom = bb;
  return(bodyBottom - bodyTop);
}

double GanitaMetricsTopDetection::returnConfidence(void)
{
  return(confidence);
}

int GanitaMetricsTopDetection::returnAuxValid(void)
{
  return(auxValid);
}

int GanitaMetricsTopDetection::setAuxValid(void)
{
  auxValid = 1;
  return(auxValid);
}

double GanitaMetricsTopDetection::returnAux(int ii)
{
  return(aux[ii % aux.size()]);
}

double GanitaMetricsTopDetection::pushAux(double val)
{
  aux.push_back(val);

  return(val);
}

double GanitaMetricsTopDetection::setMajorWidth(double ww)
{
  major_width = ww;
  return(major_width);
}

double GanitaMetricsTopDetection::setMajorHeight(double hh)
{
  major_height = hh;
  return(major_height);
}

double GanitaMetricsTopDetection::returnMajorWidth(void)
{;
  return(major_width);
}

double GanitaMetricsTopDetection::returnMajorHeight(void)
{
  return(major_height);
}

