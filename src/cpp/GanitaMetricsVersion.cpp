// Methods for GanitaMetricsVersion.
// Place to set version numbers. 

#include "ganita/metrics/GanitaMetricsVersion.hpp"

GanitaMetricsVersion::GanitaMetricsVersion(void)
{
  // This is the main required place to set version numbers. 
  version = "1.0.0";
}

std::string GanitaMetricsVersion::returnVersion(void)
{
  return(version);
}

