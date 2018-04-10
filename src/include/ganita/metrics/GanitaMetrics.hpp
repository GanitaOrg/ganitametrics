// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "ganita/graph/GanitaBuffer.hpp"
#include "ganita/metrics/GanitaMetricsVersion.hpp"
#include "ganita/metrics/GanitaMetricsMat.hpp"
#include "ganita/metrics/GanitaMetricsTrackSet.hpp"
#include "ganita/metrics/GanitaMetricsVisualize.hpp"

//std::array<string,6> gm_colors[] = 
//	{"red", "blue", "yellow", "purple", "orange", "green"};

class GanitaMetrics
{
private:
  std::string name;
  std::string type;
  int verbosity;
  GanitaBuffer *gmr, *gms;
  GanitaMetricsTrackSet gmts[2];
  std::vector< std::shared_ptr<GanitaMetricsTrack> > gmrTracks;
  std::vector< std::shared_ptr<GanitaMetricsTrack> > gmsTracks;
  std::vector< std::shared_ptr<GanitaMetricsVisualize> > gmvis;
  vector<string> gm_colors;
public:
  GanitaMetrics(void);
  GanitaMetrics(int vv);                   // set verbosity=vv
  int init(char *ref_input, char *sys_input);
  int init_2(char *ref_input, char *sys_input);
  int readMotReference(void);
  int readTopReference(void);
  int64_t addRefTrack(void);
  int readTopSystem(void);
  int64_t addSysTrack(void);
  int64_t addVis(void);
  int readT(char *input_tran);
  int dumpTHeader(void);
  int visTracks(void);
  int visTracks(int tset, int tnum);
  int printDetDenFrame(void);
  int printDetDenFrame(int ss);
  int computeKL_DensityDistance(uint64_t fr_num);
  int computeKL_DensityDistance_2(uint64_t fr_num);
  int computeKL_DensityDistance
  (uint64_t fr_num,uint64_t ref_nn, uint64_t sys_nn);
  int64_t printTrackStats(void);
  int64_t printTrackStats(int ss);
  int64_t printTrackStats(int tset, uint64_t tnum);
  int readTop(int tt);
  int readTopAsOne(int tt);
  int closeBuffers(void);
  int computeFrameStats(void);
  int64_t printStartStop(int tset);
  int computeTrackPairKL_aux(int64_t ref_nn, int64_t sys_nn);
  int computeTrackPairKL(int64_t ref_nn, int64_t sys_nn, int flip,
			 double& score);
  int computeTrackPairOuterDiv
  (int64_t ref_nn, int64_t sys_nn, int flip, double& score);
  int computeTrackKL(int64_t ref_nn, double iKL, int flip, double& scoreKL);
  int computeTrackOuterDiv
  (int64_t ref_nn, double iKL, int flip, double& scoreKL);
  int purifyTrackKL(int64_t ref_nn, int ref_or_sys, double& crossKL);
  int purifyTrackPairKL(int64_t ref_nn, int64_t sys_nn, 
			int ref_or_sys, double& score);
  int computeMeanTrackKL(int ref_or_sys, double& track_error);
};

