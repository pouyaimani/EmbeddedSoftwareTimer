#include "TicToc.h"
#include "Isr/Isr.h"

int main()
{
	handleTimer();
}

Error_t handleTimer()
{
	if (isTimerListEmpty())
		return E_OOPS;
	if (getTimeMs() - (*timerList)->setPoint > (*timerList)->interval) {
	    (*timerList)->isr((*timerList), pedestal);
	    if((*timerList)->singleShot) {
	        deleteTimer((*timerList));
	        timerList = getTimer();
	    } else {
	        (*timerList)->setPoint = getTimeMs();
	        (*timerList) = (*timerList)->next;
	    }
	} else
	    (*timerList) = (*timerList)->next;
	return E_DONE;
}
