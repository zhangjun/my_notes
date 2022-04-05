#include <chrono>
#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>

#define MAX_PAGE_SIZE (256 * 1024 * 1024)

#define LOG(format, ...)                                                       \
  do {                                                                         \
    time_t now;                                                                \
    now = time(&now);                                                          \
    struct tm t;                                                               \
    localtime_r(&now, &t);                                                     \
    fprintf(stdout, "[%02d%02d%02d %02d:%02d:%02d] %s:%d " format "\n",        \
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min,    \
            t.tm_sec, __FUNCTION__, __LINE__, ##__VA_ARGS__);                  \
  } while (0);

static size_t getCurrentRSS() {
#if defined(__linux__) || defined(__linux) || defined(linux) ||                \
    defined(__gnu_linux__)
  /* Linux ---------------------------------------------------- */
  long rss = 0L;
  FILE *fp = NULL;
  if ((fp = fopen("/proc/self/statm", "r")) == NULL)
    return (size_t)0L; /* Can't open? */
  if (fscanf(fp, "%*s%ld", &rss) != 1) {
    fclose(fp);
    return (size_t)0L; /* Can't read? */
  }
  fclose(fp);
  return (size_t)rss * (size_t)sysconf(_SC_PAGESIZE);

#else
  /*Other OS ------------------------ */
  return (size_t)0L; /* Unsupported. */
#endif
}

void calc(void *data, int size) {
  auto begin = std::chrono::steady_clock::now();
  unsigned char *p = static_cast<unsigned char *>(data);
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += *p++;
    sum %= 1000;
  }
  auto end = std::chrono::steady_clock::now();
  LOG("result: %d, cost: %d ms.\n", sum,
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count());
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    LOG("usage: %s file_name.\n", argv[0]);
    return -1;
  }

  char *file_name = argv[1];
  int fd = open(file_name, O_RDONLY);
  struct stat st;
  fstat(fd, &st);
  LOG("file size: %d\n", st.st_size);

  LOG("rss: %d.\n", getCurrentRSS());

  void *map = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

  LOG("rss: %d.\n", getCurrentRSS());
  calc(map, st.st_size);
  LOG("rss: %d.\n", getCurrentRSS());

  madvise(map, st.st_size, MADV_WILLNEED);
  madvise(map, st.st_size, MADV_SEQUENTIAL);
  LOG("rss: %d.\n", getCurrentRSS());
  madvise(map, st.st_size, MADV_DONTNEED);
  LOG("rss: %d.\n", getCurrentRSS());
  munmap(map, st.st_size);
  return 0;
}
