#!/bin/bash
# This bash script can be used to set environment variables used by ganitametrics.
# Default values are assigned in ganita metrics, so this may not be needed.
# Otherwise, change to appropriate values and type:
# source setup.sh
export GM_FONT_PATH="/usr/share/fonts/open-sans/OpenSans-Bold.ttf"
export GM_FRAME_RATE=24.97
export GM_KL_MODE=0

# GM_KL_MODE=0  error = -1*kule*log2(kule);
# GM_KL_MODE=1  error = -1*(1 - kule)*log2(1 - kule);
# GM_KL_MODE=2  error = -1*kule*log2(kule) + -1*(1 - kule)*log2(1 - kule);
# GM_KL_MODE=3  error = 2*kule*(1 - kule);

