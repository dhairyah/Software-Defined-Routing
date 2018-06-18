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

#include <stdio.h>
#include <sys/select.h>
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


#include "../include/connection_manager.h"
#include "../include/global.h"
#include "../include/control_handler.h"
#include "../include/router_handler.h"



void init()
{
	
	
    control_socket = create_control_sock(); //this tcp socket will listen for control messages from the controller

    //router_socket and data_socket will be initialized after INIT from controller

    FD_ZERO(&master_list);
    FD_ZERO(&watch_list);

    FD_SET(control_socket, &master_list);
    head_fd = control_socket;
	
	struct timeval timer_value;
	
	timer_value.tv_sec = 0;

    int selret, sock_index, fdaccept;
	uint16_t ret_router_id;

    while(TRUE){
        watch_list = master_list;
		
		
		if(TIMER[0] < 1)
		{
			TIMER[0] = 0;
		}
		timer_value.tv_usec = TIMER[0];
		timer_value.tv_sec = 0;
		
		if(INIT_FLAG == 0)
		{
			printf("\nNull select");
			fflush(stdout);
			selret = select(head_fd+1, &watch_list, NULL, NULL, NULL);
		}
		else
		{
			gettimeofday(&previous_time,NULL);
			//sleep(5);
			selret = select(head_fd+1, &watch_list, NULL, NULL, &timer_value);
		}
        if(selret < 0)
		{
            ERROR("select failed.");
		}
		else if(selret == 0)
		{
			send_routing_table();
			TIMER[0] = (double)(INTERVAL*1000000);
			printf("\nSend to neighbours");
			fflush(stdout);
			
		}		
		
		else
		{
			for(sock_index=0; sock_index<=head_fd; sock_index+=1){
				
				printf("\nfor ma" );
				fflush(stdout);
			
				if(FD_ISSET(sock_index, &watch_list)){
					
					printf("\nif ma %d",sock_index );
					fflush(stdout);
					
					/* control_socket */
					if(sock_index == control_socket){
						fdaccept = new_control_conn(sock_index);

						FD_SET(fdaccept, &master_list);
						if(fdaccept > head_fd) head_fd = fdaccept;
						
					}

					/* router_socket */
					else if(sock_index == ROUTER_SOCKET){
						ret_router_id = router_recv_hook();
						printf("\nRouting table received from %d",ntohs(ret_router_id) );
						fflush(stdout);
					}

					/* data_socket */
					else if(sock_index == data_socket){
						//new_data_conn(sock_index);
						printf("\ndata socket" );
						fflush(stdout);
					}

					/* Existing connection */
					else{

						
						if(isControl(sock_index)){

							if(!control_recv_hook(sock_index)) FD_CLR(sock_index, &master_list);
						}
						//else if isData(sock_index);
						else ERROR("Unknown socket index");
					}
				}
			}
			
			
			gettimeofday(&currnet_time,NULL);
			//printf("\nOld time : %d",TIMER[0] );
			TIMER[0] = TIMER[0] - ((((double)currnet_time.tv_sec-(double)previous_time.tv_sec)*1000000)+(((double)currnet_time.tv_usec-(double)previous_time.tv_usec)));
			printf("\nNew time : %d",TIMER[0] );
			fflush(stdout);
		
		}	
		
    }
}