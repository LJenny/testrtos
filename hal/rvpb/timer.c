#include <stdint.h>
#include "armcpu.h"
#include "board.h"
#include "gic_v3.h"
#include "stdio.h"
#include "timer.h"

#define TIMER_DEBUG		0
#define TIMER_WAIT		1	/* Assert Timer IRQ after n secs */

static uint32_t cntfrq;		/* System frequency */

#if TIMER_DEBUG
void timer_handler(void){}
#else
/* Assert Timer IRQ after n secs */
void timer_handler(void)
{
	uint64_t ticks, current_cnt;
	uint32_t val;

    putstr("timer_handler: \n");

	// Disable the timer
	disable_cntv();
    putstr("\tDisable the timer, CNTV_CTL_EL0 = ");
	val = raw_read_cntv_ctl();
	puthex(val);
	gicd_clear_pending(TIMER_IRQ);
    putstr("\n\tSystem Frequency: CNTFRQ_EL0 = ");
	puthex(cntfrq);

	// Next timer IRQ is after n sec.
	ticks = TIMER_WAIT * cntfrq;
	// Get value of the current timer
	current_cnt = raw_read_cntvct_el0();
    putstr("\n\tCurrent counter: CNTVCT_EL0 = ");
	puthex(current_cnt);
	// Set the interrupt in Current Time + TimerTick
	raw_write_cntv_cval_el0(current_cnt + ticks);
    putstr("\n\tAssert Timer IRQ after ");
	puthex(TIMER_WAIT);
    putstr(" sec(s): CNTV_CVAL_EL0 = ");
	val = raw_read_cntv_cval_el0();
	puthex(val);

	// Enable the timer
	enable_cntv();
    putstr("\n\tEnable the timer, CNTV_CTL_EL0 = ");
	val = raw_read_cntv_ctl();
	puthex(val);
    putstr("\n");
}
#endif /* TIMER_DEBUG */

void timer_test(void)
{
	uint32_t val;
	uint64_t ticks, current_cnt;

    putstr("timer_test\n");
	// GIC Init
	gic_v3_initialize();

    // putstr("CurrentEL = ");
	// val = raw_read_current_el();
	// puthex(val);

    // putstr("\nRVBAR_EL1 = ");
	// val = raw_read_rvbar_el1();
	// puthex(val);

    // putstr("\nVBAR_EL1 = ");
	// val = raw_read_vbar_el1();
	// puthex(val);

    // putstr("\nDAIF = ");
	// val = raw_read_daif();
	// puthex(val);

	// Disable the timer
	disable_cntv();
    // putstr("\nDisable the timer, CNTV_CTL_EL0 = ");
	// val = raw_read_cntv_ctl();
	// puthex(val);
    // putstr("\nSystem Frequency: CNTFRQ_EL0 = ");
	// cntfrq = raw_read_cntfrq_el0();
	// puthex(cntfrq);

	// Next timer IRQ is after n sec(s).
	ticks = TIMER_WAIT * cntfrq;
	// Get value of the current timer
	// current_cnt = raw_read_cntvct_el0();
    // putstr("\nCurrent counter: CNTVCT_EL0 = ");
	// puthex(current_cnt);
	// Set the interrupt in Current Time + TimerTick
	raw_write_cntv_cval_el0(current_cnt + ticks);
    // putstr("\nAssert Timer IRQ after 1 sec: CNTV_CVAL_EL0 = ");
	// val = raw_read_cntv_cval_el0();
	// puthex(val);

	// Enable the timer
	// enable_cntv();
    // putstr("\nEnable the timer, CNTV_CTL_EL0 = ");
	// val = raw_read_cntv_ctl();
	// puthex(val);

	// Enable IRQ 
	enable_irq();
    // putstr("\nEnable IRQ, DAIF = ");
	// val = raw_read_daif();
	// puthex(val);
    // putstr("\n");

#if TIMER_DEBUG // Observe CNTP_CTL_EL0[2]: ISTATUS
	while(1){
		current_cnt = raw_read_cntvct_el0();
		val = raw_read_cntv_ctl();
		putstr("\nCNTVCT_EL0 = ");
		puthex(current_cnt);
		putstr(", CNTV_CTL_EL0 = ");
		puthex(val);
		val = raw_read_spsr_el1();
		putstr(", SPSR_EL1 = ");
		puthex(val);
		val = raw_read_isr_el1();
		putstr(", ISR_EL1 = ");
		puthex(val);
		putstr("\n");
	}
#else
	// while(1){
	// 	wfi();	/* Wait for Interrupt */
	// }
#endif
}
