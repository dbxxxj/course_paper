#include "log.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static FILE* current_log_file;
void log(const char* message) {
	if (current_log_file == NULL)
		return;

	time_t rawtime = time(NULL);
	struct tm* timeinfo = localtime(&rawtime);

	fprintf(current_log_file, "[%02d:%02d:%02d] LOG: %s\n",
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, message);
}

void init_logger(FILE* log_file) {
	_putenv("TZ=Europe/Moscow");
	_tzset();

	if (log_file == NULL)
		return;

	current_log_file = log_file;
}