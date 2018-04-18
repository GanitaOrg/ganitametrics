Software that implements a new tracking performance metric based on KL-divergence. 

Project: GanitaMetrics Group: Ganita Author: T. Adams, Ph.D.

Requires GanitaGraph which is sold separately. Or, you may obtain it for free: git clone git@cloud.ganita.org:ganita/ganitagraph.git

Compile: cd to root directory containing this README.md file. 
mkdir build 
cd build/ 
cmake -DCMAKE_BUILD_TYPE=Release ../config 
make 
sudo make install

Executables: gmetrics

Library: libGanitaMetrics.so
