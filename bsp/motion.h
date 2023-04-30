#define kPbase 0.1
#define STOPPING_PWM 50

void forward_motion(void *args);
void backward_motion(void *args);
void stop_motion(void);
void spot_right(void *arg);
void spot_left(void *arg);
