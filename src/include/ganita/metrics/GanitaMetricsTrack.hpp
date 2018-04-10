#ifndef _GANITAMETRICSTRACK_
#define _GANITAMETRICSTRACK_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <memory>
//#include <boost/shared_ptr.hpp>

#include "ganita/metrics/GanitaMetricsMat.hpp"
#include "ganita/metrics/GanitaMetricsMotDetection.hpp"
#include "ganita/metrics/GanitaMetricsTopDetection.hpp"

class GanitaMetricsTrack
{
private:
  int64_t track_id;
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
  int computeDetectionDensity(int64_t fr_num, 
	GanitaMetricsMat& frame_det_den);
  std::vector<double> stats;
  int64_t printStats(void);
  int64_t setStart(int64_t ss);
  int64_t setEnd(int64_t ee);
  int64_t setId(int64_t dd);
  int64_t returnStart(void);
  int64_t returnEnd(void);
  int64_t returnId(void);
  int returnAuxValid(uint64_t dd);
  int setAuxValid(uint64_t dd);
  double returnAux(uint64_t dd, int ii);
  double pushAux(uint64_t dd, double val);
};

#endif
