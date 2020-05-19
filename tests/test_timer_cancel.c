/*
 * test_timer_cancel.c - tests timers
 */

#include <stdio.h>

#include <base/stddef.h>
#include <base/log.h>
#include <base/time.h>
#include <runtime/thread.h>
#include <runtime/sync.h>
#include <runtime/timer.h>

#define WORKERS		1000

void timeout_handler(unsigned long _c) {
	log_info("timeout expired");
}

static void work_handler(void *args)
{
    struct timer_entry te;

    timer_init(&te, &timeout_handler, 0);
    timer_start(&te, microtime() + 1000);
    timer_sleep(1001);
    timer_cancel(&te);
}

static void main_handler(void *arg)
{
    int i;
	log_info("started main_handler() thread");
    for (i = 0; i < WORKERS; i++) {
        thread_spawn(&work_handler, NULL);
    }

	log_info("done");
}

int main(int argc, char *argv[])
{
	int ret;

	if (argc < 2) {
		printf("arg must be config file\n");
		return -EINVAL;
	}

	ret = runtime_init(argv[1], main_handler, NULL);
	if (ret) {
		printf("failed to start runtime\n");
		return ret;
	}

	return 0;
}
