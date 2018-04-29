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

#ifndef GM_DEFAULT_FRAME_RATE
#define GM_DEFAULT_FRAME_RATE 24.97
#endif

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
  vector<string> gm_bgcolors;
  uint64_t frame_width, frame_height;
  double gm_frame_rate;
public:
  GanitaMetricsTrackSet(void);
  GanitaMetricsTrackSet(int vv);                   // set verbosity=vv
  uint64_t setStart(uint64_t ss);
  uint64_t setEnd(uint64_t ee);
  uint64_t returnStart(void);
  uint64_t returnEnd(void);
  int init(const char *buf_input);
  int readTopAsOne(void);
  int readTop(void);
  int64_t addTrack(void);
  int64_t addVis(void);
  int visTracks(string myvideoname);
  std::shared_ptr<GanitaMetricsTrack> returnTrack(uint64_t tt);
  int64_t setEndFrames(void);
  int64_t printStartStop(void);
  int64_t setNumTracks(uint64_t tnum);
  uint64_t returnNumTracks(void);
  int computeEndPoints(void);
  int computeResolution(void);
  uint64_t returnFrameWidth();
  uint64_t returnFrameHeight();
};

#endif
