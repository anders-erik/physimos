
void timing_init();

void timing_newFrame();

void timing_waitForNextFrame();

void timing_resetWorldEpoch();
void timing_pauseWorldTime();
void timing_startWorldTime();
bool timing_worldIsPaused();

int timing_current_fps();
int timing_currentFrameCount();
double timing_getWorldTime();
double timing_getWorldEpoch();