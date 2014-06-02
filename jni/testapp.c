#include <jni.h>
#include <testlib.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include <rpcmem.h>
#include <adspmsgd.h>

#define USEC_PER_SEC 1000000

void elapsed_start(struct timeval* start) {
	gettimeofday(start, NULL);
}

struct timeval elapsed_stop(struct timeval* start) {
	struct timeval current, result;
	gettimeofday(&current, NULL);


	long int diff = (current.tv_usec + USEC_PER_SEC * current.tv_sec) - (start->tv_usec + USEC_PER_SEC * start->tv_sec);
	result.tv_sec = diff / USEC_PER_SEC;
	result.tv_usec = diff % USEC_PER_SEC;
	return result;
}
struct params {
	int datasize;
	int heapid;
	boolean logging;
};

struct params parseparams(int argc, char * const * argv) {

	struct params ret;
	ret.logging = 0;
	ret.datasize = 100;
	ret.heapid = 22;
	int c;
	int option_index = 0;

	static struct option long_options[] = {
			{ "data", 1, 0, 'd' },
			{ "heap", 1, 0, 'h' },
			{ "log", 0, 0, 'l' },
			{ NULL, 0, NULL, 0 }
	};

	printf("starting parsing\n");

	while ((c = getopt_long(argc, argv, "d:lh:", long_options, &option_index)) != -1) {
		int this_option_optind = optind ? optind : 1;
		switch (c) {
		case 0:
			printf("option %s", long_options[option_index].name);
			if (optarg)
				printf(" with arg %s", optarg);
			printf("\n");
			break;
		case 'd':
			ret.datasize = atoi(optarg);
			break;
		case 'h':
			ret.heapid = atoi(optarg);
			break;
		case 'l':
			ret.logging = 1;
			break;
		case '?':
			break;
		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}
	}
	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	printf("testing for:\n", ret.datasize);
	printf("\t datasize=%d\n", ret.datasize);
	printf("\t logging=%d\n", ret.logging);
	printf("\t heapid=%d\n", ret.heapid);
	return ret;
}

int main(int argc, char * const * argv) {
	struct params par = parseparams(argc, argv);

	// init and alloc memory
	if (par.logging) {
		adspmsgd_start(par.heapid, 0, 4096);
	}
	rpcmem_init();
	int* mem = (int*)rpcmem_alloc(par.heapid, 0, par.datasize * sizeof(int));
	if (!mem) {
		printf("problem with memory!\n");
	}

	int64 result = 777;
	int i;
	for (i = 0; i < par.datasize; ++i) {
		mem[i] = 1;
	}

	struct timeval start, elapsed;
	elapsed_start(&start);
	int ret = testlib_function(mem, par.datasize, &result);
	elapsed = elapsed_stop(&start);
	long int elapsed_us = elapsed.tv_usec + USEC_PER_SEC * elapsed.tv_sec;

	printf("elapsed=%ds %dusec (%dus per 1000 numbers)\n", elapsed.tv_sec, elapsed.tv_usec, elapsed_us/(par.datasize/1000));
	printf("ret=%d\n", ret);
	printf("result=%lld\n", result);

	// free and fini memory
	rpcmem_free(mem);
	rpcmem_deinit();

	if (par.logging) {
		adspmsgd_stop();
	}
	return 0;
}
