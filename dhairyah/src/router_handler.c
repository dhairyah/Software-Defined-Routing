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
#include "../include/network_util.h"
#include "../include/control_header_lib.h"
#include "../include/author.h"
#include "../include/init.h"
#include "../include/routing_tab.h"

#define ROUTER_PORT_OFFSET 0x04
#define PADDING_OFFSET 0x06
#define ROUTER_ID_OFFSET 0x08
#define COST_OFFSET 0x0A
#define PAY_LEN (0x0C*ROUTER_CNT)+0x08

struct sockaddr_in myrouteraddr;

ssize_t nbytes = 68;

int create_router_sock()
{
	int sock;
	
	
    sock = socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&myrouteraddr,sizeof(myrouteraddr));
	
	myrouteraddr.sin_family = AF_INET;
	myrouteraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myrouteraddr.sin_port = (ROUTER_PORT[MYID]);
	
	bind(sock,(struct sockaddr *) &myrouteraddr, sizeof(myrouteraddr));

    return sock;
}


uint16_t router_recv_hook(int sock_index)
{

	char *cntrl_payload = (char *) malloc(sizeof(char)*500), ip4[INET_ADDRSTRLEN];
	int addrlen = sizeof(myrouteraddr), i,j, ny_id;
	uint16_t router_id, next_hop, cost, ny_router_id;
	uint32_t ny_ip;
	ssize_t bytes = 0;
	
	bzero(cntrl_payload, sizeof(char)*500);

	bytes = recvfrom(ROUTER_SOCKET, cntrl_payload, nbytes, 0, (struct sockaddr *)&myrouteraddr, &addrlen);
	
	if(bytes != 0)
	{
    while(bytes != nbytes)
        bytes += recvfrom(ROUTER_SOCKET, cntrl_payload, nbytes-bytes, 0, (struct sockaddr *)&myrouteraddr, &addrlen);
	}
	
	memcpy(&ny_ip, cntrl_payload+0x04, sizeof(ny_ip));
	
	for(i=0;i<ROUTER_CNT;i++)
	{
		if(IP[i]==ny_ip)
		{
			ny_id =i;
			ny_router_id = ROUTER_ID[i];
			break;
		}
	}
	
	printf("\nRouting table of  : %d",ntohs(ny_router_id));
	fflush(stdout);
	
	for(i=0;i<ROUTER_CNT;i++)
	{
		memcpy(&router_id, cntrl_payload+ROUTER_ID_OFFSET+(0x0C*i)+0x08, sizeof(router_id));
		memcpy(&cost, cntrl_payload+COST_OFFSET+(0x0C*i)+0x08, sizeof(cost));
		
		printf("\n%d sudhi java cost is %d",ntohs(router_id),ntohs(cost));
		fflush(stdout);
		
		for(j=0;j<ROUTER_CNT;j++)
		{
			if(ROUTER_ID[j] == router_id)
			{
				cost = ntohs(cost);
				COST[ny_id] = ntohs(COST[ny_id]);
				COST[j] = ntohs(COST[j]);
				
				printf("\nbefore cost : %d. cost to neigh : %d and cost of nei : %d",COST[j],COST[ny_id],cost);
				fflush(stdout);
				
				if(((cost + COST[ny_id])<=COST[j]) && COST[j] != 0)
				{
					printf("\nbefore cost : %d",COST[j]);
					fflush(stdout);
					COST[j] = (cost + COST[ny_id]);
					//COST[ny_id] = htons(COST[ny_id]);
					NEXT_HOP[j] = NEXT_HOP[ny_id];
					
					printf("\nafter cost : %d",COST[j]);
					fflush(stdout);
					
				}
				
				COST[j] = htons(COST[j]);
				COST[ny_id]  = htons(COST[ny_id]);
			}
		}
		
	}
	
	free(cntrl_payload);
	return ny_router_id;
	
	
}



void send_routing_table()
{
	char *routing_table;
	int i, udp_sock;
	struct sockaddr_in adjrouteraddr;
	uint16_t pad = 0, total_number;
	
	total_number = htons(ROUTER_CNT);

	routing_table = (char *) malloc(sizeof(char)*500);
	bzero(routing_table, sizeof(char)*500);
	
	memcpy(routing_table, &total_number, sizeof(total_number));
	memcpy(routing_table+0x02, &ROUTER_PORT[MYID], sizeof(ROUTER_PORT[MYID]));
	memcpy(routing_table+0x04, &IP[MYID], sizeof(&IP[MYID]));
	
	for(i=0;i<ROUTER_CNT;i++)
	{	
		memcpy(routing_table+(0x0C*i)+0x08, &IP[i], sizeof(IP[i]));
		memcpy(routing_table+ROUTER_PORT_OFFSET+(0x0C*i)+0x08, &ROUTER_PORT[i], sizeof(&ROUTER_PORT[i]));
		memcpy(routing_table+PADDING_OFFSET+(0x0C*i)+0x08, &pad, sizeof(pad));
		memcpy(routing_table+ROUTER_ID_OFFSET+(0x0C*i)+0x08, &ROUTER_ID[i], sizeof(&ROUTER_ID[i]));
		memcpy(routing_table+COST_OFFSET+(0x0C*i)+0x08, &COST[i], sizeof(COST[i]));		
		
	}
	
	
	
	for(i=0;i<ROUTER_CNT;i++)
	{		
		if(NEIGHBOR_FLAG[i] == 1)
		{
		
			udp_sock = socket(AF_INET,SOCK_DGRAM,0);
	
			bzero(&adjrouteraddr,sizeof(adjrouteraddr));
	
			adjrouteraddr.sin_family = AF_INET;
			adjrouteraddr.sin_addr.s_addr = (IP[i]);
			adjrouteraddr.sin_port = (ROUTER_PORT[i]);
							
			ssize_t bytes = 0;
			
			bytes = sendto(udp_sock, routing_table, nbytes, 0, (struct sockaddr *)&adjrouteraddr, sizeof(adjrouteraddr));
			
			if(bytes != 0)
			{
				while(bytes != nbytes)
				bytes += sendto(udp_sock, routing_table, nbytes-bytes, 0, (struct sockaddr *)&adjrouteraddr, sizeof(adjrouteraddr));
			}
			
			printf("\nrouting tale senr to  : %d",ntohs(ROUTER_ID[i]));
			fflush(stdout);
			
			close(udp_sock);
		}	
		
	}
	
	free(routing_table);
	
}


void check_router_status()
{
	/*for(int i=0;i<ROUTER_CNT;i++)
	{
		if(ROUTER_ID[i] == TIMER_ROUTER[0])
		{
			UPDATE_CNT[i]++;
			if(UPDATE_CNT[i] >=3)
			{
				COST[i] =  65535;
				NEXT_HOP[i] = 65535;
				NEIGHBOR_FLAG[i] = 0;
			}
		}
	}
		*/

}
