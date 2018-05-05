// Author: T.M. Adams (ganita.org)
// This class implements algorithms that implement a tracking performance 
// metric loosely based on KL-divergence. 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

#include "ganita/metrics/GanitaMetricsOptions.hpp"
#include "ganita/graph/GanitaBuffer.hpp"
#include "ganita/metrics/GanitaMetricsMat.hpp"
#include "ganita/metrics/GanitaMetricsTrackSet.hpp"

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
  vector<string> gm_colors;
  uint64_t major_width;
  uint64_t major_height;
  string vname;
  int gm_kl_mode;
public:
  GanitaMetrics(void);
  GanitaMetrics(int vv);                   // set verbosity=vv
  int init(GanitaMetricsOptions myOpt);
  int readMotReference(void);
  int64_t addRefTrack(void);
  int readTopSystem(void);
  int64_t addSysTrack(void);
  int readT(char *input_tran);
  int dumpTHeader(void);
  int visTracks(void);
  int visTracks(int tset, int tnum);
  int visTracks(int tr_num);
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
  double computeInnerDivCore(double mykule);
  int computeTrackPairOuterDiv
  (int64_t ref_nn, int64_t sys_nn, int flip, double& score);
  int computeTrackKL(int64_t ref_nn, double iKL, int flip, double& scoreKL);
  int computeTrackOuterDiv
  (int64_t ref_nn, double iKL, int flip, double& scoreKL);
  int purifyTrackKL(int64_t ref_nn, int ref_or_sys, double& crossKL);
  int purifyTrackPairKL(int64_t ref_nn, int64_t sys_nn, 
			int ref_or_sys, double& score);
  int purifyTrackPairKL_2(int64_t ref_nn, int64_t sys_nn, 
			  int ref_or_sys, double& score);
  int computeMeanTrackKL(int ref_or_sys, double& track_error);
  int processOuterDiv_1(uint64_t fr_num, uint64_t tr_num, 
		      GanitaMetricsMat& rMat, int set1, int set2);
  int processOuterDiv_2(uint64_t fr_num, int tset);
  int processOuterDiv_3(int tset);
  int updateStats(int tset, uint64_t tr_num, 
		  GanitaMetricsMat rMat1, GanitaMetricsMat rMat2);
  vector<double> gmScores;
  vector<string> gmScoreType;
  int printSummary(void);
  int testSummary(void);
  uint64_t setMajorResolution();
  int clipDetectionBoxes(void);
  uint64_t returnMajorWidth();
  uint64_t returnMajorHeight();
  uint64_t setMajorWidth(uint64_t ww);
  uint64_t setMajorHeight(uint64_t hh);
};

