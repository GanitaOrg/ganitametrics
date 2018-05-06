#!/bin/bash

echo "Test1: Identical truth and system output. Error should be 0."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_1.top
echo ""
echo "Test2: One track is correct. The other goes wrong half way through."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_2.top
echo ""
echo "Test3: Tracks get switched after intersecting in the middle."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_3.top
echo ""
echo "Test4: Instead of 2 tracks, system splits into 4 tracks."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_4.top
echo ""
echo "Test5: System gets 40% and 60% of tracks."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_5.top
echo ""
echo "Test6: One system track is missing 40% of track."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_6.top
echo ""
echo "Test7: Missing one of the tracks."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_7.top
echo ""
echo "Test8: Outputs one of the tracks twice."
../build/gmetrics -i sample_test_truth_1.top -i sample_test_system_8.top
echo ""
echo "Test9: Error should be 0. Truth has 2 disjoint tracks."
../build/gmetrics -i sample_test_truth_2.top -i sample_test_system_9.top
echo ""
echo "Test10: One track is generated twice.  Truth has 2 disjoint tracks."
../build/gmetrics -i sample_test_truth_2.top -i sample_test_system_10.top
echo ""
echo "Test11: Truth has 10 disjoint tracks. System outputs half of each frame."
../build/gmetrics -i sample_test_truth_3.top -i sample_test_system_11.top
echo ""
echo "Test12: System outputs half of the frames (first half of frames temporally)."
echo "Test12 should give the same error as Test11."
../build/gmetrics -i sample_test_truth_3.top -i sample_test_system_12.top
echo ""
echo "Test13: System misses half of the tracks."
../build/gmetrics -i sample_test_truth_3.top -i sample_test_system_13.top
echo ""
echo "Test14: System gets 7 of 10 tracks; misses others completely."
../build/gmetrics -i sample_test_truth_3.top -i sample_test_system_14.top
echo ""
echo "Test15: System gets 90% of each track."
../build/gmetrics -i sample_test_truth_3.top -i sample_test_system_15.top
