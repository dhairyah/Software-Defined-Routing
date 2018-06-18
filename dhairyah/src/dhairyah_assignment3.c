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

#include "../include/global.h"
#include "../include/connection_manager.h"

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
 
 
 /*References:
 https://ubwins.cse.buffalo.edu/cse-489_589/pa3/swetankk_pa3.tar
 Beej Programming Guide
 https://www.gnu.org/software/libc/manual/html_node/Waiting-for-I_002fO.html
 https://stackoverflow.com/questions/10192903/time-in-milliseconds
 */
int main(int argc, char **argv)
{

	INIT_FLAG = 0;
    sscanf(argv[1], "%" SCNu16, &CONTROL_PORT);
    init(); // blocking function

    return 0;
}
