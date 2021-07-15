#include "TicToc.h"
#include "DSP28x_Project.h"
#include "stdlib.h"

static volatile Uint64 time = 0;
static Timer_t *head = NULL;
static int timersNum = 0;

Error_t initTicToc()
{
	EALLOW;
    PieVectTable.TINT1 = &tikTok;
    ConfigCpuTimer(&CpuTimer1, 1, 90000);
    CpuTimer1Regs.TCR.all = 0x4001;
    IER |= M_INT13; // Enable timer interrupt
    EDIS;
    return E_DONE;
}

__interrupt void tikTok(void)
{
    DINT; // Disable all interrupts to prevent race condition
	++time;
	EINT; // Enable interrupts
}

Uint64 getTimeMs()
{
	static volatile Uint64 tmp;
	DINT; // Disable all interrupts to prevent race condition
	tmp = time;
	EINT; // Enable interrupts
	return tmp;
}

bool isTimerListEmpty()
{
    return (bool)(head == NULL);
}

static Error_t startTimer(Timer_t *timer, Uint64 t)
{
	timer->interval = t;
	DINT; // Disable all interrupts to prevent race condition
	timer->setPoint = time;
	EINT; // Enable interrupts
	return E_DONE;
}

Timer_t *newTimer(void (*funPtr)(Timer_t *timer, Pedestal_t *), bool singleShot)
{
	Timer_t *timer = malloc(sizeof(Timer_t));
	timer->isr = funPtr;
	timer->singleShot = singleShot;
	timer->start = startTimer;
	++timersNum;
	if (head == NULL) {
		timer->next = timer->previous =
		head = timer;
		return timer;
	}
	Timer_t *last = head->previous;
	timer->next = head;
	timer->previous = last;
	head->previous = timer;
	last->next = timer;
	return timer;
}

Error_t deleteTimer(Timer_t *timer)
{
    if (timer->next == timer)
        head = NULL;
    else {
        if (timer == head)
            head = timer->next;
        timer->previous->next = timer->next;
        timer->next->previous = timer->previous;
    }
	free(timer);
	timersNum--;
	return E_DONE;
}

Timer_t **getTimer()
{
    return &head;
}
