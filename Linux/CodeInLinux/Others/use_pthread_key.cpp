/*
 * pthread_key.cpp
 *
 *  Created on: 2016年12月22日
 *      Author: root
 */
#include "use_pthread_key.h"

const int g_arrayCount = 2;

/* The key used to associate a log file pointer with each thread. */

static pthread_key_t thread_log_key;

TSS<FILE> tls(close_thread_log);

/* Write MESSAGE to the log file for the current thread. */

void write_to_thread_log (const char* message)

{
	FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);

	fprintf (thread_log, "%s\n", message);
}

/* Close the log file pointer THREAD_LOG. */

void close_thread_log (void* thread_log)

{
    printf("run close_thread_log\n");
	fclose ((FILE*) thread_log);
}

void* thread_function (void* argv)
{
    printf("thread_function %d #start\n", argv);

	char thread_log_filename[20];

	FILE* thread_log;

	/* Generate the filename for this thread’s log file. */

	sprintf (thread_log_filename, "thread%d.log", argv);
    printf("thread_function log filename %s\n", thread_log_filename);
	/* Open the log file. */

	thread_log = fopen (thread_log_filename, "w");

	/* Store the file pointer in thread-specific data under thread_log_key. */

	pthread_setspecific (thread_log_key, thread_log);

	write_to_thread_log ("Thread starting.");

	/* Do work here... */

    printf("thread_function %d #end\n", argv);
	return NULL;
}

void use_pthread_key()
{
	int i;

	pthread_t threads[g_arrayCount];


	/* Create a key to associate thread log file pointers in

	thread-specific data. Use close_thread_log to clean up the file

	pointers. */

	pthread_key_create (&thread_log_key, close_thread_log);

	/* Create threads to do the work. */

	for (i = 1; i <= g_arrayCount; ++i)

		pthread_create (&(threads[i - 1]), NULL, thread_function, (void*)i);

	/* Wait for all threads to finish. */

	for (i = 0; i < g_arrayCount; ++i)

		pthread_join (threads[i], NULL);

	pthread_key_delete(thread_log_key);
}


void write_to_thread_log_class (const char* message)
{
	FILE* thread_log = (FILE*) tls.get();

	fprintf (thread_log, "%s\n", message);
}

void* thread_function_class (void* argv)
{
    printf("thread_function_class %d #start\n", argv);

	char thread_log_filename[20];

	/* Generate the filename for this thread’s log file. */

	sprintf (thread_log_filename, "thread%d.log", argv);
    printf("thread_function log filename %s\n", thread_log_filename);

    FILE* thread_log = tls.get();

    if (!thread_log)
    {
    	thread_log = fopen (thread_log_filename, "w");
    	tls.set(thread_log);
    }

	write_to_thread_log ("Thread class starting.");

	/* Do work here... */

    printf("thread_function_class %d #end\n", argv);
	return NULL;
}

void use_pthread_key_class()
{
		int i;

		pthread_t threads[g_arrayCount];

		/* Create threads to do the work. */

		for (i = 1; i <= g_arrayCount; ++i)

			pthread_create (&(threads[i - 1]), NULL, thread_function_class, (void*)i);

		/* Wait for all threads to finish. */

		for (i = 0; i < g_arrayCount; ++i)

			pthread_join (threads[i], NULL);
}
