/*
 * OpenDispense 2 
 * UCC (University [of WA] Computer Club) Electronic Accounting System
 *
 * handler_doror.c - Door Relay code
 *
 * This file is licenced under the 3-clause BSD Licence. See the file
 * COPYING for full details.
 */
#define	DEBUG	1
#define	USE_POPEN	0

#include "common.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pty.h>

#define DOOR_UNLOCKED_DELAY	5	// Time in seconds before the door re-locks

// === IMPORTS ===

// === PROTOTYPES ===
 int	Door_InitHandler();
 int	Door_CanDispense(int User, int Item);
 int	Door_DoDispense(int User, int Item);

// === GLOBALS ===
tHandler	gDoor_Handler = {
	"door",
	Door_InitHandler,
	Door_CanDispense,
	Door_DoDispense
};
char	*gsDoor_SerialPort = "/dev/ttyS3";

// == CODE ===
int Door_InitHandler(void)
{
	return 0;
}

/**
 */
int Door_CanDispense(int User, int Item)
{
	#if DEBUG
	printf("Door_CanDispense: (User=%i,Item=%i)\n", User, Item);
	#endif
	// Sanity please
	if( Item != 0 )	return -1;
	
	if( !(Bank_GetFlags(User) & USER_FLAG_DOORGROUP) )
	{
		#if DEBUG
		printf("Door_CanDispense: User %i not in door\n", User);
		#endif
		return 1;
	}
	
	#if DEBUG
	printf("Door_CanDispense: User %i can open the door\n", User);
	#endif
	
	return 0;
}

/**
 * \brief Actually do a dispense from the coke machine
 */
int Door_DoDispense(int User, int Item)
{
	 int	door_serial_handle;
	
	#if DEBUG
	printf("Door_DoDispense: (User=%i,Item=%i)\n", User, Item);
	#endif
	
	// Sanity please
	if( Item != 0 )	return -1;
	
	// Check if user is in door
	if( !(Bank_GetFlags(User) & USER_FLAG_DOORGROUP) )
	{
		#if DEBUG
		printf("Door_CanDispense: User %i not in door\n", User);
		#endif
		return 1;
	}
	
	door_serial_handle = InitSerial(gsDoor_SerialPort, 9600);

	if( write(door_serial_handle, "ATH1\n", 5) != 5 ) {
		fprintf(stderr, "Unable to open door (sending ATH1)\n");
		perror("Sending ATH1");
		return -1;
	}
	
	// Wait before re-locking
	sleep(DOOR_UNLOCKED_DELAY);

	if( write(door_serial_handle, "ATH0\n", 5) != 5 ) {
		fprintf(stderr, "Oh, hell! Door not re-locking, big error (sending ATH0 failed)\n");
		perror("Sending ATH0");
		return -1;
	}

	close(door_serial_handle);
	
	#if DEBUG
	printf("Door_DoDispense: User %i opened door\n", User);
	#endif

	return 0;
}

