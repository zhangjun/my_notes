#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

static long page_size = sysconf(_SC_PAGESIZE);

void get_proc(int pid, int& num_thread, int & rss_size);
