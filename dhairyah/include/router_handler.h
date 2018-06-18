#ifndef ROUTER_HANDLER_H_
#define ROUTER_HANDLER_H_

int create_router_sock();

uint16_t router_recv_hook();

void send_routing_table();

void check_router_status();

#endif