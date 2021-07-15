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
	if (getTimeMs() - timersList->setPoint > timersList->interval) {
	    timersList->isr(timersList, pedestal);
	    if(timersList->singleShot) {
	        deleteTimer(timersList);
	    } else {
	        timersList->setPoint = getTimeMs();
	        timersList = timersList->next;
	    }
	} else
	    timersList = timersList->next;
	return E_DONE;
}
