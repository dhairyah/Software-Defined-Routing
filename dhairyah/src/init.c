/**
 * @dhairyah_assignment3
 * @author  Dhairya Hardikbhai Desai <dhairyah@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */

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

#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"
#include "../include/router_handler.h"

#define INTERVAL_OFFSET 0x02
#define ROUTER_ID_OFFSET 0x04
#define ROUTER_PORT_OFFSET 0x06
#define DATA_PORT_OFFSET 0x08
#define COST_OFFSET 0x0A
#define IP_OFFSET 0x0C

void init_response(int sock_index,char *cntrl_payload)
{
	uint16_t response_len;
	char *cntrl_response_header, *cntrl_response,ip4[INET_ADDRSTRLEN];
	int i;

	cntrl_response_header = create_response_header(sock_index, 1, 0, 0);

	response_len = CNTRL_RESP_HEADER_SIZE;
	cntrl_response = (char *) malloc(response_len);
	
	//Copy Header
	memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
	free(cntrl_response_header);
	
	//Send the control response
	sendALL(sock_index, cntrl_response, response_len); 

	free(cntrl_response);
	
	//Reading the init payload sent by the controller
	
	memcpy(&ROUTER_CNT, cntrl_payload, sizeof(ROUTER_CNT));
	ROUTER_CNT = ntohs(ROUTER_CNT);
	
	memcpy(&INTERVAL, cntrl_payload+INTERVAL_OFFSET, sizeof(INTERVAL));
	INTERVAL = ntohs(INTERVAL);
	
	//printf("\nInnterva: %" PRIu16 "\n",INTERVAL);
	
	for(i=0;i<ROUTER_CNT;i++)
	{
		
		memcpy(&ROUTER_ID[i], cntrl_payload+ROUTER_ID_OFFSET+(0x0C*i), sizeof(ROUTER_ID[i]));
		memcpy(&NEXT_HOP[i], cntrl_payload+ROUTER_ID_OFFSET+(0x0C*i), sizeof(NEXT_HOP[i]));
	//	ROUTER_ID[i] = ntohs(ROUTER_ID[i]);
	//	NEXT_HOP[i] = ntohs(NEXT_HOP[i]);
		
		printf("\nRouter ID : %d",ntohs(ROUTER_ID[i]));
		fflush(stdout);	
		
		
		
		memcpy(&ROUTER_PORT[i], cntrl_payload+ROUTER_PORT_OFFSET+(0x0C*i), sizeof(ROUTER_PORT[i]));
	//	ROUTER_PORT[i] = ntohs(ROUTER_PORT[i]);
		
		
		
		
		memcpy(&DATA_PORT[i], cntrl_payload+DATA_PORT_OFFSET+(0x0C*i), sizeof(DATA_PORT[i]));
	//	DATA_PORT[i] = ntohs(DATA_PORT[i]);
		
		
		
		
		
		memcpy(&COST[i], cntrl_payload+COST_OFFSET+(0x0C*i), sizeof(COST[i]));
	//	COST[i] = ntohs(COST[i]);
		
		printf("\nCost : %d",ntohs(COST[i]));
		fflush(stdout);
		
		if(COST[i] == 65535)
		{
			memcpy(&NEXT_HOP[i], &COST[i], sizeof(NEXT_HOP[i]));
			NEIGHBOR_FLAG[i] = 0;
		}
		else if(COST[i]==0)
		{
			MYID = i;
			TIMER_ROUTER[0] = ROUTER_ID[i];
			NEIGHBOR_FLAG[i] = 0;
		}		
		else
		{
			NEIGHBOR_FLAG[i] = 1; 
			
		}
		
		
		
		
		memcpy(&IP[i], cntrl_payload+IP_OFFSET+(0x0C*i), sizeof(IP[i]));
		
		
		inet_ntop(AF_INET, &(IP[i]), ip4, INET_ADDRSTRLEN);
		
	}
	
	ROUTER_SOCKET = create_router_sock(); //this will create a UDP socket on router port for receiving routing tables
	FD_SET(ROUTER_SOCKET, &master_list);
	if(ROUTER_SOCKET > head_fd) head_fd = ROUTER_SOCKET;
	
	printf("\nUDP listening socket created : %d",ROUTER_SOCKET);
	fflush(stdout);
	
	TIMER[0] = (double)(INTERVAL*1000000);
	INIT_FLAG = 1;
	gettimeofday(&previous_time,NULL);

	
}