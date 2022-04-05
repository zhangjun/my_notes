// g++ -o client_para_multiport client_para_multiport.cpp -lpthread -lrt

#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

using std::cout;
using std::string;

#define NUM_THREADS 40
#define MAX_SIZE 10000

#define PORT_NUMS 3

void *conn_handler(void *threadid);
timespec diff(timespec start, timespec end);
string curr_time();

// const int ports[PORT_NUMS] = {50000, 50001, 50002, 50003, 50004, 50005,
// 50006, 50007};
const int ports[PORT_NUMS] = {50000, 50001, 50002};
// const int ports[PORT_NUMS] = {50000};

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  struct timespec ts, te;
  clock_gettime(CLOCK_REALTIME, &ts);
  printf("CLOCK_REALTIME: %d, %d\n", ts.tv_sec, ts.tv_nsec);
  for (long i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, conn_handler, (void *)i) < 0) {
      perror("could not create thread");
      return 1;
    }
  }

  for (long i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  clock_gettime(CLOCK_REALTIME, &te);
  printf("CLOCK_REALTIME: %d, %d\n", te.tv_sec, te.tv_nsec);

  timespec usetime = diff(ts, te);
  printf("time taken: %d sec, %d nsec\n", usetime.tv_sec, usetime.tv_nsec);

  pthread_exit(NULL);
  return 0;
}

void *conn_handler(void *threadid) {
  long threadnum = (long)threadid;
  int sock;
  struct sockaddr_in serv_addr;
  char buff[MAX_SIZE];
  //	string databody = "{log_type\":1,\"status\":1000,\"date\":\"2017-01-10
  //05:59:59\",\"uid\":\"1892354604\",\"search\":\"\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c\",\"ip\":\"220.249.162.120\",\"Mozilla\":\"106A093010\",\"module\":[{\"cate\":1,\"info\":[{\"uid\":2057592134,\"cate\":1}]},{\"cate\":158,\"info\":[{\"mid\":\"4062103712952002\",\"adid\":\"133073\",\"social\":1}]},{\"cate\":26,\"info\":[{\"mid\":\"4062043466821593\",\"social\":1,\"cluster\":6},{\"mid\":\"4062045743496137\",\"social\":1,\"cluster\":2}]},{\"cate\":26,\"info\":[{\"mid\":\"4062048205577663\",\"social\":1,\"cluster\":2}]},{\"cate\":79,\"info\":[{\"doc_id\":\"12882780539554645228\",\"is_headimg\":1},{\"is_manual\":\"0\",\"doc_id\":\"17379660607775038273\"},{\"is_manual\":\"0\",\"doc_id\":\"15117295549520192103\"}]},{\"cate\":31,\"info\":[{\"mid\":\"4062073324842127\",\"social\":1,\"cluster\":2},{\"mid\":\"4062087669953303\",\"social\":1},{\"mid\":\"4062135149948651\",\"social\":1,\"cluster\":2},{\"mid\":\"4062090169920472\",\"social\":1},{\"mid\":\"4062155466294034\",\"social\":1}]},{\"cate\":125,\"info\":[]},{\"cate\":113,\"info\":[{\"mid\":\"4062097669740119\",\"social\":1},{\"mid\":\"4062148558342833\",\"social\":1,\"cluster\":2},{\"mid\":\"4062059602135903\",\"social\":1,\"cluster\":2},{\"mid\":\"4062058272722918\",\"social\":1,\"cluster\":2},{\"mid\":\"4062173116920411\"},{\"mid\":\"4062170440937955\"},{\"mid\":\"4062159820642934\"},{\"mid\":\"4062155869766278\"},{\"mid\":\"4062153722124677\"},{\"mid\":\"4062153437178331\"},{\"mid\":\"4062147778518547\",\"cluster\":2},{\"mid\":\"4062147577567605\"},{\"mid\":\"4062146168196801\"},{\"mid\":\"4062141054458693\"}]},{\"cate\":19,\"info\":[]}],\"v_p\":\"36\",\"cid\":\"100103type=1&q=\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c&t=3\",\"sid\":\"t_wap_ios\",\"type\":\"1\",\"t\":\"32\",\"page\":\"1\",\"seqid\":930173021,\"nettype\":\"4g\",\"gis\":\"119.493949,25.954432\",\"ua\":\"iPhone8,1__weibo__6.10.0__iphone__os9.3.4\",\"refer\":\"c_type=32&filter_type=friends&pos=3\",\"uicode\":\"10000003\",\"luicode\":\"10000011\",\"lfid\":\"106003type%3D25%26filter_type%3Dfriends\",\"ext\":[]}\n";

  // if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  //	printf("faild to create socket\n");

  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  //	serv_addr.sin_addr.s_addr = inet_addr("172.16.42.23");
  serv_addr.sin_addr.s_addr = inet_addr("10.73.12.69");
  //	serv_addr.sin_addr.s_addr = inet_addr("123.126.54.32");
  //	serv_addr.sin_addr.s_addr = inet_addr("10.21.131.15");
  int port = ports[threadnum % PORT_NUMS];
  serv_addr.sin_port = htons(port);

  // if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
  //	printf("fail to connnect to server\n");
  //}

  // printf("connected successfully client: %d\n", threadnum);
  int count = 0;
  struct timespec ts, te;
  struct timespec usetime;
  long long secdiff = 0, nsecdiff = 0;
  // char* databody =
  // "<mobile_query>{\"log_type\":1,\"status\":1000,\"date\":\"2017-01-10
  // 05:59:59\",\"uid\":\"1892354604\",\"search\":\"\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c\",\"ip\":\"220.249.162.120\",\"Mozilla\":\"106A093010\",\"module\":[{\"cate\":1,\"info\":[{\"uid\":2057592134,\"cate\":1}]},{\"cate\":158,\"info\":[{\"mid\":\"4062103712952002\",\"adid\":\"133073\",\"social\":1}]},{\"cate\":26,\"info\":[{\"mid\":\"4062043466821593\",\"social\":1,\"cluster\":6},{\"mid\":\"4062045743496137\",\"social\":1,\"cluster\":2}]},{\"cate\":26,\"info\":[{\"mid\":\"4062048205577663\",\"social\":1,\"cluster\":2}]},{\"cate\":79,\"info\":[{\"doc_id\":\"12882780539554645228\",\"is_headimg\":1},{\"is_manual\":\"0\",\"doc_id\":\"17379660607775038273\"},{\"is_manual\":\"0\",\"doc_id\":\"15117295549520192103\"}]},{\"cate\":31,\"info\":[{\"mid\":\"4062073324842127\",\"social\":1,\"cluster\":2},{\"mid\":\"4062087669953303\",\"social\":1},{\"mid\":\"4062135149948651\",\"social\":1,\"cluster\":2},{\"mid\":\"4062090169920472\",\"social\":1},{\"mid\":\"4062155466294034\",\"social\":1}]},{\"cate\":125,\"info\":[]},{\"cate\":113,\"info\":[{\"mid\":\"4062097669740119\",\"social\":1},{\"mid\":\"4062148558342833\",\"social\":1,\"cluster\":2},{\"mid\":\"4062059602135903\",\"social\":1,\"cluster\":2},{\"mid\":\"4062058272722918\",\"social\":1,\"cluster\":2},{\"mid\":\"4062173116920411\"},{\"mid\":\"4062170440937955\"},{\"mid\":\"4062159820642934\"},{\"mid\":\"4062155869766278\"},{\"mid\":\"4062153722124677\"},{\"mid\":\"4062153437178331\"},{\"mid\":\"4062147778518547\",\"cluster\":2},{\"mid\":\"4062147577567605\"},{\"mid\":\"4062146168196801\"},{\"mid\":\"4062141054458693\"}]},{\"cate\":19,\"info\":[]}],\"v_p\":\"36\",\"cid\":\"100103type=1&q=\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c&t=3\",\"sid\":\"t_wap_ios\",\"type\":\"1\",\"t\":\"32\",\"page\":\"1\",\"seqid\":930173021,\"nettype\":\"4g\",\"gis\":\"119.493949,25.954432\",\"ua\":\"iPhone8,1__weibo__6.10.0__iphone__os9.3.4\",\"refer\":\"c_type=32&filter_type=friends&pos=3\",\"uicode\":\"10000003\",\"luicode\":\"10000011\",\"lfid\":\"106003type%3D25%26filter_type%3Dfriends\",\"ext\":[]}\n";
  string prev = "<mobile_query>{\"log_type\":1,\"status\":1000,\"date\":\"";
  // string tm ="2017-01-10 05:59:59";

  string post =
      "\",\"uid\":\"1892354604\",\"search\":"
      "\"\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c\",\"ip\":\"220.249.162."
      "120\",\"Mozilla\":\"106A093010\",\"module\":[{\"cate\":1,\"info\":[{"
      "\"uid\":2057592134,\"cate\":1}]},{\"cate\":158,\"info\":[{\"mid\":"
      "\"4062103712952002\",\"adid\":\"133073\",\"social\":1}]},{\"cate\":26,"
      "\"info\":[{\"mid\":\"4062043466821593\",\"social\":1,\"cluster\":6},{"
      "\"mid\":\"4062045743496137\",\"social\":1,\"cluster\":2}]},{\"cate\":26,"
      "\"info\":[{\"mid\":\"4062048205577663\",\"social\":1,\"cluster\":2}]},{"
      "\"cate\":79,\"info\":[{\"doc_id\":\"12882780539554645228\",\"is_"
      "headimg\":1},{\"is_manual\":\"0\",\"doc_id\":\"17379660607775038273\"},{"
      "\"is_manual\":\"0\",\"doc_id\":\"15117295549520192103\"}]},{\"cate\":31,"
      "\"info\":[{\"mid\":\"4062073324842127\",\"social\":1,\"cluster\":2},{"
      "\"mid\":\"4062087669953303\",\"social\":1},{\"mid\":"
      "\"4062135149948651\",\"social\":1,\"cluster\":2},{\"mid\":"
      "\"4062090169920472\",\"social\":1},{\"mid\":\"4062155466294034\","
      "\"social\":1}]},{\"cate\":125,\"info\":[]},{\"cate\":113,\"info\":[{"
      "\"mid\":\"4062097669740119\",\"social\":1},{\"mid\":"
      "\"4062148558342833\",\"social\":1,\"cluster\":2},{\"mid\":"
      "\"4062059602135903\",\"social\":1,\"cluster\":2},{\"mid\":"
      "\"4062058272722918\",\"social\":1,\"cluster\":2},{\"mid\":"
      "\"4062173116920411\"},{\"mid\":\"4062170440937955\"},{\"mid\":"
      "\"4062159820642934\"},{\"mid\":\"4062155869766278\"},{\"mid\":"
      "\"4062153722124677\"},{\"mid\":\"4062153437178331\"},{\"mid\":"
      "\"4062147778518547\",\"cluster\":2},{\"mid\":\"4062147577567605\"},{"
      "\"mid\":\"4062146168196801\"},{\"mid\":\"4062141054458693\"}]},{"
      "\"cate\":19,\"info\":[]}],\"v_p\":\"36\",\"cid\":\"100103type=1&q="
      "\u5c0f\u9f50\u9f50\u7684\u5927\u4e16\u754c&t=3\",\"sid\":\"t_wap_ios\","
      "\"type\":\"1\",\"t\":\"32\",\"page\":\"1\",\"seqid\":930173021,"
      "\"nettype\":\"4g\",\"gis\":\"119.493949,25.954432\",\"ua\":\"iPhone8,1__"
      "weibo__6.10.0__iphone__os9.3.4\",\"refer\":\"c_type=32&filter_type="
      "friends&pos=3\",\"uicode\":\"10000003\",\"luicode\":\"10000011\","
      "\"lfid\":\"106003type%3D25%26filter_type%3Dfriends\",\"ext\":[]}\n";
  //	string databody =
  //	clock_gettime(CLOCK_REALTIME, &ts);
  while (count < 2000) {
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      printf("faild to create socket\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("fail to connnect to server\n");
    }
    //	sprintf(buff,"<%d>%s", threadnum, databody.c_str());
    string databody = prev + curr_time() + post;
    clock_gettime(CLOCK_REALTIME, &ts);
    send(sock, databody.c_str(), databody.length(), 0);
    clock_gettime(CLOCK_REALTIME, &te);
    secdiff += diff(ts, te).tv_sec;
    nsecdiff += diff(ts, te).tv_nsec;
    count++;
    close(sock);
    //	sleep(1);
  }
  //	clock_gettime(CLOCK_REALTIME, &te);
  //	secdiff += diff(ts, te).tv_sec;
  //	nsecdiff += diff(ts, te).tv_nsec;
  close(sock);
  long long meantime = secdiff * 1000 / count + (nsecdiff / count) / 1000000;
  printf("<%d> mean time: %lld\n", threadnum, meantime);
  return 0;
}

timespec diff(timespec start, timespec end) {
  timespec diffval;
  if ((end.tv_nsec - start.tv_nsec) < 0) {
    diffval.tv_sec = end.tv_sec - start.tv_sec - 1;
    diffval.tv_nsec = end.tv_nsec - start.tv_nsec + 1000000000;
  } else {
    diffval.tv_sec = end.tv_sec - start.tv_sec;
    diffval.tv_nsec = end.tv_nsec - start.tv_nsec;
  }

  return diffval;
}

string curr_time() {
  time_t now_time = time(NULL);
  tm *local;
  local = localtime(&now_time);
  char buf[128] = {0};
  strftime(buf, 64, "%Y-%m-%d %H:%M:%S", local);
  string st = buf;
  return st;
}
