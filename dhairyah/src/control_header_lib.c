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
#include <netinet/in.h>

#include "../include/global.h"
#include "../include/control_header_lib.h"

#define CNTRL_RESP_CONTROL_CODE_OFFSET 0x04
#define CNTRL_RESP_RESPONSE_CODE_OFFSET 0x05
#define CNTRL_RESP_PAYLOAD_LEN_OFFSET 0x06

char* create_response_header(int sock_index, uint8_t control_code, uint8_t response_code, uint16_t payload_len)
{
    char *buffer;
    char *cntrl_resp_header;

    struct sockaddr_in addr;
    socklen_t addr_size;

    buffer = (char *) malloc(sizeof(char)*CNTRL_RESP_HEADER_SIZE);
	
    cntrl_resp_header = buffer;
    

    addr_size = sizeof(struct sockaddr_in);
    getpeername(sock_index, (struct sockaddr *)&addr, &addr_size);


	/* Controller IP Address */
	memcpy(cntrl_resp_header, &(addr.sin_addr), sizeof(struct in_addr));
	/* Control Code */
	memcpy(cntrl_resp_header+CNTRL_RESP_CONTROL_CODE_OFFSET, &control_code, sizeof(control_code));
	/* Response Code */
	memcpy(cntrl_resp_header+CNTRL_RESP_RESPONSE_CODE_OFFSET, &response_code, sizeof(response_code));
	/* Payload Length */
	payload_len = htons(payload_len);
	memcpy(cntrl_resp_header+CNTRL_RESP_PAYLOAD_LEN_OFFSET, &payload_len, sizeof(payload_len));


    return buffer;
}