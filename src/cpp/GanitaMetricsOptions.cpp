// This file contains the methods for GanitaMetricsOptions. 

#include "ganita/metrics/GanitaMetricsOptions.hpp"

// Constructors
GanitaMetricsOptions::GanitaMetricsOptions(void)
{
  verbosity = 0;
  vis_flag = 0;
  file_flag = 0;
  compare_flag = 0;
  help_flag = 0;
  res_flag = 0; // use default value
  res_x = 1920; // default value
  res_y = 1080; // default value
}

int GanitaMetricsOptions::getOptions(int argc, char* argv[])
{
  int ii;
  std::string::size_type sz;
  char mystr[500];

  std::vector <std::string> sources;
  std::string destination;
  for (ii = 1; ii < argc; ++ii) {
    if ((std::string(argv[ii]) == "--help") ||(std::string(argv[ii]) == "-h")) {
      outputOptions(argv);
      return(0);
    }
    else if ((std::string(argv[ii]) == "--verbose") ||(std::string(argv[ii]) == "-v")) {
      if (ii + 1 < argc) { // Make sure we aren't at the end of argv!
	verbosity = std::stoi(argv[++ii], &sz);
	//destination = argv[ii++]; // Increment 'ii' so we don't get the argument as the next argv[ii].
      } else { // Uh-oh, there was no argument to the destination option.
	std::cerr << "--verbose requires a level (0 - 9)" << std::endl;
	return(-1);
      }
    }
    else if ((std::string(argv[ii]) == "--compute-resolution") ||(std::string(argv[ii]) == "-c")) {
      res_flag = 2;
    }
    else if ((std::string(argv[ii]) == "--resolution") ||(std::string(argv[ii]) == "-r")) {
      if (ii + 1 < argc) { // Make sure we aren't at the end of argv!
	if(sscanf(argv[++ii], "%ldx%ld", &res_x, &res_y) == 2){
	  res_flag = 1;
	  if(verbosity > 0){
	    std::cout<<"resolution ("<<res_x<<","<<res_y<<")"<<std::endl;
	  }
	}
      } else { // Uh-oh, there was no argument to the destination option.
	std::cerr << "--resolution requires an argument wxh" << std::endl;
	return(-1);
      }  
    }
    else if ((std::string(argv[ii]) == "--visualize") ||(std::string(argv[ii]) == "-s")) {
      if(file_flag == 0){
	vis_flag = 1;
      }
      else{
	vis_flag = file_flag;
      }
    }
    else if ((std::string(argv[ii]) == "--input-file") ||(std::string(argv[ii]) == "-i")) {
      if (ii + 1 < argc) { // Make sure we aren't at the end of argv!
	sscanf(argv[++ii], "%s", mystr);
	fnames.push_back(mystr);
	if(verbosity > 0){
	  std::cout<<"input-file ("<<fnames[file_flag]<<")"<<std::endl;
	}
	file_flag++;
      } else { // Uh-oh, there was no argument to the destination option.
	std::cerr << "--input-file requires an argument" << std::endl;
	return (-1);
      }  
    }
  }

  if(file_flag > 1){
    compare_flag = 1;
  }
  
  return(file_flag + vis_flag);
}

int GanitaMetricsOptions::outputOptions(char* argv[])
{
  GanitaMetricsVersion version;

  std::cout<<"Usage: "<<argv[0]<<" [options] -i file1 [-i file2]"<<std::endl;
  std::cout<<"Version: "<<version.returnVersion()<<std::endl;
  std::cout<<"Options:"<<std::endl;
  std::cout<<"-h,--help\t"<<std::endl;
  std::cout<<"-v,--verbose\t 0-9"<<std::endl;
  std::cout<<"-c,--compute-resolution"<<std::endl;
  std::cout<<"-r,--resolution\t wxh"<<std::endl;
  std::cout<<"-s,--visualize\t"<<std::endl;
  std::cout<<"-i,--input-file\t file1"<<std::endl;

  return(1);
}

int GanitaMetricsOptions::returnVerbosity(void)
{
  return(verbosity);
}

int GanitaMetricsOptions::returnVisFlag(void)
{
  return(vis_flag);
}

int GanitaMetricsOptions::returnFileFlag(void)
{
  return(file_flag);
}

int GanitaMetricsOptions::returnCompareFlag(void)
{
  return(compare_flag);
}

int GanitaMetricsOptions::returnHelpFlag(void)
{
  return(help_flag);
}

int GanitaMetricsOptions::returnResFlag(void)
{
  return(res_flag);
}

uint64_t GanitaMetricsOptions::returnResX(void)
{
  return(res_x);
}

uint64_t GanitaMetricsOptions::returnResY(void)
{
  return(res_y);
}

std::string GanitaMetricsOptions::returnFileName(uint32_t ii)
{
  if(fnames.size() <= ii){
    // index too large
    return("");
  }
  return(fnames[ii]);
}

