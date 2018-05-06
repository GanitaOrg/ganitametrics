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
  vname = std::string("");
}

int GanitaMetricsOptions::getOptions(int argc, char* argv[])
{
  int ii;
  std::string::size_type sz;
  char mystr[500];

  if(argc < 2){
    outputOptions(argv);
    return(0);
  }

  for (ii = 1; ii < argc; ++ii) {
    if ((std::string(argv[ii]) == "--help") ||(std::string(argv[ii]) == "-h")) {
      outputOptions(argv);
      return(0);
    }
    else if ((std::string(argv[ii]) == "--verbose") ||(std::string(argv[ii]) == "-v")) {
      if (ii + 1 < argc) { // Make sure we aren't at the end of argv!
	verbosity = std::stoi(argv[++ii], &sz);
	//destination = argv[ii++]; // Increment 'ii' so we don't get the argument as the next argv[ii].
      } else { // Uh-oh, there was no argument.
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
      } else { // Uh-oh, there was no argument.
	std::cerr << "--resolution requires an argument wxh" << std::endl;
	return(-1);
      } 
    }
    else if ((std::string(argv[ii]) == "--visualize") ||(std::string(argv[ii]) == "-s")) {
      if (ii + 1 < argc) { // Make sure we aren't at the end of argv!
	sscanf(argv[++ii], "%s", mystr);
	vname = std::string(mystr);
	if(file_flag == 0){
	  vis_flag = 1;
	}
	else{
	  vis_flag = file_flag;
	}
	if(verbosity > 0){
	  std::cout<<"video-file ("<<vname<<")"<<std::endl;
	}
      } else { // Uh-oh, there was no argument.
	std::cerr << "--visualize requires an argument" << std::endl;
	return (-1);
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

  if(file_flag < 1){
    outputOptions(argv);
    return(0);
  }

  if((file_flag < 2) && (vis_flag < 1)){
    outputOptions(argv);
    return(0);
  }

  if(file_flag > 1){
    compare_flag = 1;
  }
  
  return(file_flag + vis_flag);
}

int GanitaMetricsOptions::outputOptions(char* argv[])
{
  GanitaMetricsVersion version;
  uint64_t ii;
  std::string progname;
  progname = std::string(argv[0]);

  std::cout<<"------------------------------------------------------------"<<std::endl;
  std::cout<<"| Usage: "<<argv[0]<<" [options] -i file1 [-i sys_file]";
  for(ii=0; ii<17-progname.size(); ii++){ std::cout<<" ";}
  std::cout<<"|"<<std::endl;
  std::cout<<"| Version: "<<version.returnVersion();
  for(ii=0; ii<48-version.returnVersion().size(); ii++){ std::cout<<" ";}
  std::cout<<"|"<<std::endl;
  std::cout<<"| Options:                                                 |"<<std::endl;
  std::cout<<"| -h,--help                                                |"<<std::endl;
  std::cout<<"| -v,--verbose               0-9                           |"<<std::endl;
  std::cout<<"| -r,--resolution            wxh                           |"<<std::endl;
  std::cout<<"| -c,--compute-resolution                                  |"<<std::endl;
  std::cout<<"| -s,--visualize             video-file                    |"<<std::endl;
  std::cout<<"| -i,--input-file            file1                         |"<<std::endl;
  std::cout<<"------------------------------------------------------------"<<std::endl;
  std::cout<<"| When scoring system output, file1 must be the reference. |"<<std::endl;
  std::cout<<"| For the visualization, video-file must be in a format    |"<<std::endl;
  std::cout<<"| consumable by ffmpeg.                                    |"<<std::endl;
  std::cout<<"| Default resolution is 1920x1080.                         |"<<std::endl;
  std::cout<<"------------------------------------------------------------"<<std::endl;

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
  if(verbosity > 1){
    std::cout<<"File "<<ii<<" "<<fnames[ii]<<std::endl;
  }
  return(fnames[ii]);
}

std::string GanitaMetricsOptions::returnVideoName(void)
{
  return(vname);
}

