#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/queue.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>

typedef enum {FALSE, TRUE} bool;

#define ERROR(err_msg) {perror(err_msg); exit(EXIT_FAILURE);}

/* https://scaryreasoner.wordpress.com/2009/02/28/checking-sizeof-at-compile-time/ */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)])) // Interesting stuff to read if you are interested to know how this works

#define MAX_ROUTER_CNT 5

uint16_t CONTROL_PORT, ROUTER_CNT, INTERVAL, ROUTER_ID[MAX_ROUTER_CNT], ROUTER_PORT[MAX_ROUTER_CNT], DATA_PORT[MAX_ROUTER_CNT], COST[MAX_ROUTER_CNT], NEXT_HOP[MAX_ROUTER_CNT];
uint32_t IP[MAX_ROUTER_CNT];
int ROUTER_SOCKET, HEAD_FD, INIT_FLAG, NEIGHBOR_FLAG[MAX_ROUTER_CNT], head_fd, MYID;
uint16_t TIMER_ROUTER[20];
double TIMER[20];
fd_set master_list, watch_list;
struct sockaddr_in myrouteraddr;
struct timeval currnet_time,previous_time;

#endif