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
#include "ganita/metrics/GanitaMetricsTrack.hpp"
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
  std::vector< std::shared_ptr<GanitaMetricsTrack> > gmtracks;
  std::vector< std::shared_ptr<GanitaMetricsVisualize> > gmvis;
  vector<string> gm_colors;
public:
  GanitaMetrics(void);
  GanitaMetrics(int vv);                   // set verbosity=vv
  int init(char *ref_input, char *sys_input);
  int readMotReference(void);
  int readTopReference(void);
  int64_t addTrack(void);
  int64_t addVis(void);
  int readT(char *input_tran);
  int dumpTHeader(void);
  int init(char *input_seq);
  int visTracks(void);
  int printDetDenFrame(void);
};

