#ifndef _GANITAMETRICSTRACKSET_
#define _GANITAMETRICSTRACKSET_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ganita/graph/GanitaBuffer.hpp>
#include <ganita/metrics/GanitaMetricsTrack.hpp>

#ifndef GM_DEFAULT_FRAME_RATE
#define GM_DEFAULT_FRAME_RATE 24.97
#endif

#ifndef GM_DEFAULT_FONT
#define GM_DEFAULT_FONT "/usr/share/fonts/open-sans/OpenSans-Bold.ttf"
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
  vector<string> gm_colors;
  vector<string> gm_bgcolors;
  uint64_t frame_width, frame_height;
  double gm_frame_rate;
  string gm_font_path;
public:
  GanitaMetricsTrackSet(void);
  GanitaMetricsTrackSet(int vv);                   // set verbosity=vv
  int setVerbose(int vv);
  uint64_t setStart(uint64_t ss);
  uint64_t setEnd(uint64_t ee);
  uint64_t returnStart(void);
  uint64_t returnEnd(void);
  int init(const char *buf_input);
  int readTopAsOne(void);
  int readTop(void);
  int64_t addTrack(void);
  int64_t addTrack(double ww, double hh);
  int visTracks(string myvideoname);
  std::shared_ptr<GanitaMetricsTrack> returnTrack(uint64_t tt);
  int64_t setEndFrames(void);
  int64_t printStartStop(void);
  int64_t setNumTracks(uint64_t tnum);
  uint64_t returnNumTracks(void);
  int computeEndPoints(void);
  int computeResolution(void);
  uint64_t setFrameWidth(uint64_t ww);
  uint64_t setFrameHeight(uint64_t hh);
  uint64_t returnFrameWidth();
  uint64_t returnFrameHeight();
  int clipDetectionBoxes(void);
};

#endif
