// g++ -O3 ../src/drivers/gmGenerate.cc -o gmGenerate
// Sample program used to generate simple test tracks.
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
  int ii, jj;
  
  for(ii=0; ii<9; ii++){
    for(jj=0; jj<10; jj++){
      std::cout<<jj<<","<<ii<<",0,1,0,0,0,0,"<<192*ii<<","<<108*jj
	       <<","<<192*ii+192<<","<<108*jj+108<<std::endl;
    }
  }

  return(0);
}

