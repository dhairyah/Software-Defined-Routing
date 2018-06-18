#ifndef ROUTING_TAB_H_
#define ROUTING_TAB_

void routing_table_response(int sock_index);
void crash_response(int sock_index);
void update_response(int sock_index, char *cntrl_payload);

#endif