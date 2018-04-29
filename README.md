Software that implements a new tracking performance metric based on KL-divergence. 

Project: GanitaMetrics Group: Ganita Author: T. Adams, Ph.D.

Requires GanitaGraph which is sold separately. Or, you may obtain it for free: git clone http://ganita.org:8083/ganita/ganitagraph.git

Compile: cd to root directory containing this README.md file. 
mkdir build 
cd build/ 
cmake -DCMAKE_BUILD_TYPE=Release ../config 
make 
sudo make install

Executables: gmetrics

Library: libGanitaMetrics.so

----------------------------------------------------------
| Usage: ./gmetrics [options] -i file1 [-i file2]        |
| Version: 0.3                                           |
| Options:                                               |
| -h,--help                                              |
| -v,--verbose               0-9                         |
| -r,--resolution            wxh                         |
| -c,--compute-resolution                                |
| -s,--visualize             video-file                  |
| -i,--input-file            file1                       |
----------------------------------------------------------
| file1 should be in the csv top format.                 |
| video-file should be in a format consumable by ffmpeg. |
| Default resolution is 1920x1080.                       |
----------------------------------------------------------

If gmetrics is run with the -s flag, then it will look 
for the annotation file following -i and build a script 
that uses ffmpeg to overlay these annotation boxes 
onto the video file following -s. 
Note, it assumes a frame rate of 24.97. 
The default value should be changed to handle a different 
frame rate. 

To produce the visualization video:
./gmetric -s video-file -i annotation.top 

will output a file called ganita_metrics_vis.sh. 
mkdir ganita_tmp
mv ganita_metrics_vis.sh ganita_tmp/
cd ganita_tmp/
sh ganita_metrics_vis.sh
ffplay ganita_vis.avi

The visualization option uses default values. 
To change these values, modify config/setup.sh 
to appropriate values and then type:
source setup.sh

gmetrics outputs an error summary to stdout. 
Sample command-line and output:
time ./gmetrics -i ../data/duke_tracker_output/TownCentre/TownCentre-groundtruth.top -i ../data/duke_tracker_output/TownCentre/TownCentre-output-BenfoldReidCVPR2011.top -r 1920x1080 -s /home/data/duke_tracker_output/TownCentre/TownCentreXVID.avi
File ../data/duke_tracker_output/TownCentre/TownCentre-groundtruth.top size = 5553300
File ../data/duke_tracker_output/TownCentre/TownCentre-output-BenfoldReidCVPR2011.top size = 5728155
***************************************************
*                    Summary                      *
***************************************************
* Inner div relative to reference 	 0.757959 *
* Inner div relative to system    	 0.192347 *
* Total inner div error           	+0.950307 *
* Missed detection error          	+1.038188 *
* Missed detection proportion     	 0.322062 *
* Density error rel to reference  	+0.103510 *
* False alarm error               	+0.764577 *
* False alarm proportion          	 0.252416 *
* Density error rel to system     	+0.091421 *
*-------------------------------------------------*
* Total KL-track error            	=2.948003 *
***************************************************

real	1m21.236s
user	1m21.001s
sys	0m0.132s

