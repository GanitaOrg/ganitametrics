#ifndef _GANITAMETRICSTOPDETECTION_
#define _GANITAMETRICSTOPDETECTION_

// This class implements algorithms that 
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

class GanitaMetricsTopDetection
{
private:
  int64_t frame_number;  //frame
  int64_t id;            //person number
  int headValid;
  int bodyValid;
  double headLeft;
  double headTop;
  double headRight;
  double headBottom;
  double bodyLeft;
  double bodyTop;
  double bodyRight;
  double bodyBottom;
  double confidence;
  std::vector<double> aux;
  int auxValid;
  int verbosity;
  double major_width;
  double major_height;
public:
  GanitaMetricsTopDetection(void);
  GanitaMetricsTopDetection(int vv);                   // set verbosity=vv
  int setValues(int64_t new_frame_number,
		int64_t new_id,
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
  int64_t returnId(void);
  int64_t returnFrameNumber(void);
  double returnX_Anchor(void);
  double returnY_Anchor(void);
  double returnWidth(void);
  double returnHeight(void);
  double returnBodyRight(void);
  double returnBodyBottom(void);
  double setBodyRight(double br);
  double setBodyBottom(double bb);
  double returnConfidence(void);
  int returnAuxValid(void);
  int setAuxValid(void);
  double returnAux(int ii);
  double pushAux(double val);
  double setMajorWidth(double ww);
  double setMajorHeight(double hh);
  double returnMajorWidth(void);
  double returnMajorHeight(void);
};

#endif
