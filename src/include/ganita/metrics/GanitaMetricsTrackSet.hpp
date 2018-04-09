#ifndef _GANITAMETRICSTRACKSET_
#define _GANITAMETRICSTRACKSET_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <memory>
#include <ganita/graph/GanitaBuffer.hpp>
#include <ganita/metrics/GanitaMetricsTrack.hpp>
#include <ganita/metrics/GanitaMetricsVisualize.hpp>

class GanitaMetricsTrackSet
{
private:
  int64_t id;
  int64_t track_set_start;
  int64_t track_set_end;
  int verbosity;
  GanitaBuffer *gmb;
  uint64_t numTracks;
  std::vector< std::shared_ptr<GanitaMetricsTrack> > gmTracks;
  std::vector< std::shared_ptr<GanitaMetricsVisualize> > gmvis;
  vector<string> gm_colors;
public:
  GanitaMetricsTrackSet(void);
  GanitaMetricsTrackSet(int vv);                   // set verbosity=vv
  int64_t setStart(int64_t ss);
  int64_t setEnd(int64_t ee);
  int init(char *buf_input);
  //int readMot(void);
  int readTopAsOne(void);
  int readTop(void);
  int64_t addTrack(void);
  int64_t addVis(void);
  int visTracks(void);
  std::shared_ptr<GanitaMetricsTrack> returnTrack(uint64_t tt);
  int64_t setEndFrames(void);
  int64_t printStartStop(void);
  int64_t setNumTracks(uint64_t tnum);
  uint64_t returnNumTracks(void);
  //int computeTrackPairKL(int64_t ref_nn, int64_t sys_nn);
//   int printDetDenFrame(void);
//   int computeKL_DensityDistance(uint64_t fr_num);
//   int64_t printTrackStats(void);
};

#endif
