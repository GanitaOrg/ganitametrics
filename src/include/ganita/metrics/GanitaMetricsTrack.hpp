#ifndef _GANITAMETRICSTRACK_
#define _GANITAMETRICSTRACK_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <boost/shared_ptr.hpp>

#include "ganita/metrics/GanitaMetricsDetection.hpp"

class GanitaMetricsTrack
{
private:
  int64_t id;
  int64_t track_start;
  int64_t track_end;
  double confidence;
  int verbosity;
  std::vector< std::shared_ptr<GanitaMetricsDetection> > gmdetections;

public:
  GanitaMetricsTrack(void);
  GanitaMetricsTrack(int vv);                   // set verbosity=vv
  int64_t addDetection(void);
  int64_t addDetection(int64_t new_frame_number,
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
  int returnGMD(uint64_t nn, GanitaMetricsDetection& gmd);
};

#endif
