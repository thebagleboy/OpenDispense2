/*
 * OpenDispense2
 *
 * This code is published under the terms of the Acess licence.
 * See the file COPYING for details.
 *
 * common.h - Core Header
 */
#ifndef _COMMON_H_
#define _COMMON_H_

// === CONSTANTS ===
#define	DEFAULT_CONFIG_FILE	"/etc/opendispense/main.cfg"
#define	DEFAULT_ITEM_FILE	"/etc/opendispense/items.cfg"

// === HELPER MACROS ===
#define _EXPSTR(x)	#x
#define EXPSTR(x)	_EXPSTR(x)

// === STRUCTURES ===
typedef struct sItem	tItem;
typedef struct sUser	tUser;
typedef struct sConfigItem	tConfigItem;
typedef struct sHandler	tHandler;

struct sItem
{
	char	*Name;	//!< Display Name
	 int	Price;	//!< Price
	
	tHandler	*Handler;	//!< Handler for the item
	short	ID;	//!< Item ID
};

struct sUser
{
	 int	ID;		//!< User ID (LDAP ID)
	 int	Balance;	//!< Balance in cents
	 int	Bytes;	//!< Traffic Usage
	char	Name[];	//!< Username
};

struct sConfigItem
{
	char	*Name;
	char	*Value;
};

struct sHandler
{
	char	*Name;
	 int	(*Init)(int NConfig, tConfigItem *Config);
	 int	(*CanDispense)(int User, int ID);
	 int	(*DoDispense)(int User, int ID);
};

// === GLOBALS ===
extern tItem	*gaItems;
extern int	giNumItems;
extern tHandler	*gaHandlers[];
extern int	giNumHandlers;
extern int	giDebugLevel;

// === FUNCTIONS ===
extern int	DispenseItem(int User, tItem *Item);

// --- Logging ---
extern void	Log_Error(const char *Format, ...);
extern void	Log_Info(const char *Format, ...);

// --- Cokebank Functions ---
extern int	Transfer(int SourceUser, int DestUser, int Ammount, const char *Reason);
extern int	GetBalance(int User);
extern char	*GetUserName(int User);
extern int	GetUserID(const char *Username);

#endif
