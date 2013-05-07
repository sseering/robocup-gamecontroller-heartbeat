#ifndef PTHREAD_HELPERS_H
#define PTHREAD_HELPERS_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <pthread.h>

inline void pthread_start_detach(const char* thread_name, pthread_t& thread_var, void* (*thread_main)(void*), void* thread_arg) {
	if(pthread_create(&(thread_var), NULL, (thread_main), (thread_arg))) {
		fprintf(stderr, "Error: thread creation failed in file %s line %d.\nExiting.\n", __FILE__, __LINE__);
		perror("pthread");
		exit(EXIT_FAILURE);
	}

	char short_name[15];
	strncpy(short_name,thread_name,sizeof(short_name));
	short_name[sizeof(short_name)-1]='\0';
	if(int err=pthread_setname_np(thread_var, short_name)) {
        fprintf(stderr, "Error: thread naming failed in file %s line %d: %d, %s\nExiting.\n", __FILE__, __LINE__, err, short_name);
        perror("pthread");
        exit(EXIT_FAILURE);
	}

	if(pthread_detach(thread_var)) {
        fprintf(stderr, "Error: thread detach failed in file %s line %d.\nExiting.\n", __FILE__, __LINE__);
        perror("pthread");
        exit(EXIT_FAILURE);
	}
}

#endif
