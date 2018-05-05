// Methods for GanitaMetricsVersion.
// Place to set version numbers. 

#include "ganita/metrics/GanitaMetricsVersion.hpp"

GanitaMetricsVersion::GanitaMetricsVersion(void)
{
  // This is the main required place to set version numbers. 
  version = "0.9.2";
}

std::string GanitaMetricsVersion::returnVersion(void)
{
  return(version);
}

