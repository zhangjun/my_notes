#include "ps.h"

void get_proc(int pid, int &num_thread, int &rss_size) {
  char buf[32] = {0};
  sprintf(buf, "/proc/%d/stat", pid);
  std::ifstream fin(buf);

  std::stringstream buffer;
  buffer << fin.rdbuf();

  std::string content = buffer.str();

  // get pagesize
  // long sz = sysconf(_SC_PAGESIZE);
  // std::cout << sz << std::endl;

  // int page_size = getpagesize();
  // std::cout << page_size << std::endl;

  int count = 0, last = 0;
  char ch = ' ';
  size_t found = content.find_first_of(ch, last);
  while (found != std::string::npos) {
    count++;
    // man proc
    // 20: num_thread; ;24: page_size
    if (count == 20) {
      num_thread = std::atoi(content.substr(last, found - last).c_str());
    }
    if (count == 24) {
      rss_size = std::atoi(content.substr(last, found - last).c_str()) *
                 page_size / 1024;
      break;
    }
    last = found + 1;
    found = content.find_first_of(ch, last);
  }
}
