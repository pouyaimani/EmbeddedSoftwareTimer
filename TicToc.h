#ifndef TICTOC_H_
#define TICTOC_H_

typedef struct Timer_t{
	struct Timer_t *next;
	struct Timer_t *previous;
	double interval;
	double setPoint;
	void (*isr)(struct Timer_t *, Pedestal_t *);
	Error_t (*start)(struct Timer_t *timer, Uint64 time);
	bool singleShot;
} Timer_t;

__interrupt void tikTok(void); // tic toc isr
Uint64 getTimeMs();
// bellow function istantiate a timer and take isr function as its argument
// also take a bool variale that determine timer is single shot or not
Timer_t *newTimer(void (*funPtr)(Timer_t *timer), bool singleShot);
Error_t initTicToc();
Error_t deleteTimer(Timer_t *timer);
bool isTimerListEmpty();
Timer_t **getTimer();

#endif
