// This file contains the methods for GanitaMetrics. 

#include "ganita/metrics/GanitaMetrics.hpp"

// Constructors
GanitaMetrics::GanitaMetrics(void)
{
  verbosity = 0;
}

GanitaMetrics::GanitaMetrics(int vv)
{
  verbosity = vv;
}

int GanitaMetrics::readTHeader(ifstream &gzt_file)
{
//   GanitaMetricsSchema gzs;
//   std::string line;

//   if(!std::getline(gzt_file,line)){
//     return(-1);
//   }
//   if(line != gzs.returnSchema(0)){
//     std::cout<<"Header error!"<<std::endl;
//     return(-1);
//   }
//   if(!std::getline(gzt_file,line)){
//     return(-1);
//   }
//   if(line != gzs.returnSchema(1)){
//     std::cout<<"Header error!"<<std::endl;
//     return(-1);
//   }
//   // Read in transformation name.
//   if(!std::getline(gzt_file,name)){
//     return(-1);
//   }
//   // Read in the representation. 
//   if(!std::getline(gzt_file,representation)){
//     return(-1);
//   }
//   // Read in secondary name. Not stored currently. 
//   if(!std::getline(gzt_file,line)){
//     return(-1);
//   }
//   // Read in type. 
//   if(!std::getline(gzt_file,type)){
//     return(-1);
//   }

  return(1);
}

int GanitaMetrics::readT(char *input_tran)
{
  // Read in the input transformation. 
//   std::ifstream gzt_file(input_tran);
//   if (!gzt_file.is_open()){
//     std::cout<<"Unable to open input file: "<<input_tran<<std::endl;
//     return(-1);
//   }

//   if(readTHeader(gzt_file) < 0){
//     gzt_file.close();
//     return(-1);
//   }

//   if(representation != "adic"){
//     cout<<"Only able to read adic representation."<<endl;
//     gzt_file.close();
//     return(-1);
//   }

//   readAdic(gzt_file);

//   if(verbosity > 0){
//     dumpTHeader();
//   }

//   gzt_file.close();
  return(1);
}

int GanitaMetrics::dumpTHeader(void)
{
  GanitaMetricsVersion gzv;
  fprintf(stdout, "GanitaMetrics version \t %s\n", gzv.returnVersion().c_str());

  return (1);
}

int GanitaMetrics::init(char *input_seq)
{
  // Read in the input transformation. 
  unsigned long tmp;
  //std::ifstream sym_file(input_seq,std::ifstream::binary);
  // std::ifstream sym_file(input_seq);
  // if (!sym_file.is_open()){
  //   std::cout<<"Unable to open input file: "<<input_seq<<std::endl;
  //   return(0);
  // }
  
//   tmp = gSym.init(input_seq);
//   if(verbosity){
//     gSym.dumpAlphabet();
//   }

  return(tmp);
}

