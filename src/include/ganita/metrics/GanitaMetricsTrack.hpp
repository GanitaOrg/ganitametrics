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

#include "ganita/metrics/GanitaMetricsMotDetection.hpp"
#include "ganita/metrics/GanitaMetricsTopDetection.hpp"

class GanitaMetricsTrack
{
private:
  int64_t id;
  int64_t track_start;
  int64_t track_end;
  double confidence;
  int verbosity;
  std::vector< std::shared_ptr<GanitaMetricsMotDetection> > gmdetections;
  std::vector< std::shared_ptr<GanitaMetricsTopDetection> > gtdetections;

public:
  GanitaMetricsTrack(void);
  GanitaMetricsTrack(int vv);                   // set verbosity=vv
  int64_t addTopDetection(void);
  int64_t addTopDetection(int64_t new_id,
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
			  int new_verbosity);
  int returnTopGMD(uint64_t nn, GanitaMetricsTopDetection& gmd);
  int64_t addMotDetection(void);
  int64_t addMotDetection(int64_t new_frame_number,
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
  int returnMotGMD(uint64_t nn, GanitaMetricsMotDetection& gmd);
  uint64_t returnNumberOfTopDetections(void);
};

#endif
