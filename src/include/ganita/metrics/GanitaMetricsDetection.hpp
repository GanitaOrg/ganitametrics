#ifndef _GANITAMETRICSDETECTION_
#define _GANITAMETRICSDETECTION_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

class GanitaMetricsDetection
{
private:
  int64_t frame_number;
  int64_t id;
  double x_anchor;
  double y_anchor;
  double width;
  double height;
  double confidence;
  double x_world;
  double y_world;
  double z_world;
  int verbosity;
public:
  GanitaMetricsDetection(void);
  GanitaMetricsDetection(int vv);                   // set verbosity=vv
  int setValues(int64_t new_frame_number,
		int64_t new_id,
		double new_x_anchor,
		double new_y_anchor,
		double new_width,
		double new_height,
		double new_confidence,
		double new_x_world,
		double new_y_world,
		double new_z_world,
		int new_verbosity);
  int64_t returnId(void);
  int64_t returnFrameNumber(void);
  double returnX_Anchor(void);
  double returnY_Anchor(void);
  double returnWidth(void);
  double returnHeight(void);
  double returnConfidence(void);
};

#endif
