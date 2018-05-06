Software that implements a new tracking performance metric based on KL-divergence. 

Project: GanitaMetrics Group: Ganita Author: T. Adams, Ph.D.

Requires GanitaGraph which is sold separately. Or, you may obtain it for free: git clone https://github.com/GanitaOrg/ganitagraph.git

Compile: cd to root directory containing this README.md file. 
mkdir build 
cd build/ 
cmake -DCMAKE_BUILD_TYPE=Release ../config 
make 
sudo make install

Executables: gmetrics

Library: libGanitaMetrics.so

------------------------------------------------------------
| Usage: ./gmetrics [options] -i file1 [-i sys_file]       |
| Version: 0.10.0                                          |
| Options:                                                 |
| -h,--help                                                |
| -v,--verbose               0-9                           |
| -r,--resolution            wxh                           |
| -c,--compute-resolution                                  |
| -s,--visualize             video-file                    |
| -i,--input-file            file1                         |
------------------------------------------------------------
| When scoring system output, file1 must be the reference. |
| For the visualization, video-file must be in a format    |
| consumable by ffmpeg.                                    |
| Default resolution is 1920x1080.                         |
------------------------------------------------------------

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
time ./gmetrics -i /home/terry/projects/src/ganita/ganitametrics/data/duke_tracker_output/TownCentre/TownCentre-groundtruth.top -i /home/terry/projects/src/ganita/ganitametrics/data/duke_tracker_output/TownCentre/TownCentre-output-BenfoldReidBMVC2009.top -r 1920x1080
File /home/terry/projects/src/ganita/ganitametrics/data/duke_tracker_output/TownCentre/TownCentre-groundtruth.top size = 5553300
File /home/terry/projects/src/ganita/ganitametrics/data/duke_tracker_output/TownCentre/TownCentre-output-BenfoldReidBMVC2009.top size = 4390814
***************************************************
*                    Summary                      *
***************************************************
* Reference # of Tracks   230                     *
* System    # of Tracks   244                     *
***************************************************
* Inner div relative to reference 	 0.285220 *
* Inner div relative to system    	 0.444053 *
* Total inner div error           	+0.729273 *
* Missed detection error          	+1.275770 *
* Missed detection proportion     	 0.426925 *
* Density error rel to reference  	+0.033175 *
* False alarm error               	+0.327670 *
* False alarm proportion          	 0.176251 *
* Density error rel to system     	+0.142520 *
*-------------------------------------------------*
* Total KL-track error            	=2.508408 *
***************************************************

real	1m6.142s
user	1m2.240s
sys	0m3.883s


Originally, reads annotation files in the "top" format.  This is a csv files in the following format:
TrackId,FrameNumber,HeadValid,BodyValid,HeadLeft,HeadTop,HeadRight,HeadBottom,BodyLeft,BodyTop,BodyRight,BodyBottom

