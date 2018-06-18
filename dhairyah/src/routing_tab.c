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

#include "../include/global.h"
#include "../include/control_header_lib.h"
#include "../include/network_util.h"

#define PADDING_OFFSET 0x02
#define NEXT_HOP_OFFSET 0x04
#define COST_OFFSET 0x06
#define PAY_LEN 0x08*ROUTER_CNT

void routing_table_response(int sock_index)
{
	
	printf("\nRouting started");
	
	uint16_t response_len, pad = 0;
	char *cntrl_response_header, *cntrl_response_payload, *cntrl_response;
	int i;

	cntrl_response_payload = (char *) malloc(PAY_LEN);
	bzero(cntrl_response_payload, PAY_LEN);
	
	
	
	for(i=0;i<ROUTER_CNT;i++)
	{
		
		//ROUTER_ID[i] = htons(ROUTER_ID[i]);
		//NEXT_HOP[i] = htons(NEXT_HOP[i]);
		//COST[i] = htons(COST[i]);
		
		memcpy(cntrl_response_payload+(0x08*i), &ROUTER_ID[i], sizeof(ROUTER_ID[i]));
		memcpy(cntrl_response_payload+PADDING_OFFSET+(0x08*i), &pad, sizeof(pad));
		memcpy(cntrl_response_payload+NEXT_HOP_OFFSET+(0x08*i), &NEXT_HOP[i], sizeof(NEXT_HOP[i]));
		memcpy(cntrl_response_payload+COST_OFFSET+(0x08*i), &COST[i], sizeof(COST[i]));		
		
	}
	

	printf("\nafter for loop");
	fflush(stdout);
	
	cntrl_response_header = create_response_header(sock_index, 2, 0, PAY_LEN);
	
	response_len = CNTRL_RESP_HEADER_SIZE+PAY_LEN;
	cntrl_response = (char *) malloc(response_len);
	/* Copy Header */
	memcpy(cntrl_response, cntrl_response_header, CNTRL_RESP_HEADER_SIZE);
	free(cntrl_response_header);
	/* Copy Payload */
	memcpy(cntrl_response+CNTRL_RESP_HEADER_SIZE, cntrl_response_payload, PAY_LEN);
	free(cntrl_response_payload);

	sendALL(sock_index, cntrl_response, response_len);

	free(cntrl_response);
	
	
	
}



void crash_response(int sock_index)
{

	char *cntrl_response_header;
	uint16_t response_len;
	
	cntrl_response_header = create_response_header(sock_index, 4, 0, 0);
	response_len = CNTRL_RESP_HEADER_SIZE;	
	
	sendALL(sock_index, cntrl_response_header, response_len);

	free(cntrl_response_header);
}



void update_response(int sock_index, char *cntrl_payload)
{

	char *cntrl_response_header;
	uint16_t response_len, router_id, cost;
	int i;
	
	cntrl_response_header = create_response_header(sock_index, 3, 0, 0);
	response_len = CNTRL_RESP_HEADER_SIZE;	
	
	sendALL(sock_index, cntrl_response_header, response_len);

	free(cntrl_response_header);
	
	memcpy(&router_id, cntrl_payload, sizeof(router_id));
	memcpy(&cost, cntrl_payload+0x02, sizeof(cost));
	
	for(i=0;i<ROUTER_CNT;i++)
	{
		if(ROUTER_ID[i] == router_id)
		{

				if(cost == 65535)
				{
					memcpy(&NEXT_HOP[i], &cost, sizeof(NEXT_HOP[i]));
					memcpy(&COST[i], &cost, sizeof(COST[i]));
				}
				else
				{
					memcpy(&NEXT_HOP[i], &router_id, sizeof(NEXT_HOP[i]));
					memcpy(&COST[i], &cost, sizeof(COST[i]));
				}
				
		}
	}
}