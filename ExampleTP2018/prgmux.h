/*   Copyright (c) by Annecy Electronique, Saint Jorioz, France

   Environment :    PC  (Langage C)

   DESCRIPTION:

   Déclaration des structures de communications entre un programme d'extension et MuxTrace
   CE FICHIER NE DOIT PAS ETRE MODIFIE.
   CE FICHIER DOIT ETRE INCLUT A VOTRE PROJET.

   Structures declaration between plugin programm and MuxTrace software
   THIS FILE MUST NOT BE CHANGED.
   THIS FILE MUST BE INCLUDED TO YOUR PROJECT.

   HISTORY:

        V 1.00     Philippe CHAZOT      04/03/2004
        - Created.

        V 1.01     Alexandre GAMBIER    03/02/2005
        - Add MUXPRG_SDK_VERSION constant value WHICH MUST NOT BE CHANGED
        - Add GetSDKVersion exported function which is used in order to get Muxtrace software
          the possibility to know the SDK used.
        - Add GetInfos exported function which gives the possibility to the to write informations
		  such as version number and a description string.
		- Add OnNwcEvent exported functions
		- Add OnGetVariableName exported functions which give the possibility to use external
		  variable intialized in the muxtrace interface.
		- Add OnSetVariableValue exported functions which give the possibility to use external
		  variable intialized in the muxtrace interface.
		- Add OnReplayFrame exported functions which give the possibility to schedule the replay
		- Add OnTriggerReplay exported functions which give the possibility to schedule the replay
		- Add LinSetMode internal function support.
		- Add LinWakeUpMode internal function support.
		- Add Iso14230SendMsg internal function support.
		- Add OpenProject internal function support
		- Add SuspendReplay internal function support
		- Add ResumeReplay internal function support

        V 1.02		Alexandre GAMBIER	22/09/2005
        - Add IsoWaitResponse internal function support.
        - Add IsoChangeBaudrate internal function support.
*/

#include "refmux.h"

#ifndef _PRGMUX_H_
#define _PRGMUX_H_

#pragma pack(push,2) 	  			        // 2-byte alignment: MSVC & Borland

#ifdef __cplusplus
#define	_EXTERNC	extern "C"
#else
#define	_EXTERNC	extern
#endif	// __cplusplus

//  _MUXPRG is used to export DLL function
#define _MUXPRG __declspec(dllexport) __stdcall

// DO NOT CHANGE THIS VERSION NUMBER
#define MUXPRG_SDK_VERSION          0x102

// Available Bus type
typedef enum
{
	PRGMUX_BUSTYPE_CAN	=	0,				// Can bus
	PRGMUX_BUSTYPE_VAN	=	1,				// Van bus
	PRGMUX_BUSTYPE_LIN	=	2,				// Lin bus
	PRGMUX_BUSTYPE_ISO	=	3,				// Iso bus
	PRGMUX_BUSTYPE_NWC	=	4,				// NWC bus
	PRGMUX_BUSTYPE_ANA	=	5,				// "ANA" bus
	PRGMUX_BUSTYPE_FULL	=	0xFFFFFFFF  	// Force enum to be on 4 bytes
}EN_PRGMUX_BUSTYPE;

//
typedef struct
{
	unsigned long		dwTimeStamp;		// Time of event in 100 µSec
    unsigned long		dwIdent;			// Ident
    EN_PRGMUX_BUSTYPE	eBusType;			// Bus type
    unsigned short		wXtdIdent;			// Extended ident ?
	unsigned short		wBus;				// Bus number
	unsigned short		wDataLen;			// Data Length
	unsigned char		bData[4096];		// Buffer of data
    unsigned char		bReserved[32];		// Reserved for future use
}ST_PRGMUX_ONREPLAYFRAME;

typedef struct
{
    EN_PRGMUX_BUSTYPE	eBusType;			// Bus type
	unsigned short		wBus;				// Bus number
}ST_PRGMUX_ONTRIGGERREPLAY;

// Structure declaration
#define EXXO_MAX_FIFO    30
typedef enum
{
    EXXO_CODE_CAN_SEND_MSG          = 0x00,         // CAN Message
    EXXO_CODE_LIN_SEND_MSG          = 0x01,         // LIN Message
    EXXO_CODE_NWC_SEND_MSG          = 0x02,         // Diag on CAN Message
    EXXO_CODE_VAN_SEND_MSG          = 0x03,         // VAN Message
    EXXO_CODE_ISO_SEND_MSG          = 0x04,         // ISO Message
    EXXO_CODE_ISO14230_SEND_MSG     = 0x05,         // ISO14230 Message
    EXXO_CODE_DISPLAY_MSG           = 0x10,         // Display Message
    EXXO_CODE_IO_SET_OUTPUT         = 0x11,         // IO set output
    EXXO_CODE_TRIGGER               = 0x12,         // Trigger
    EXXO_CODE_STOP                  = 0x13,         // Stop analysys
    EXXO_CODE_LIN_SETSLEEPMODE      = 0x14,         // LIN Sleep Frame
    EXXO_CODE_LIN_SETWAKEUPMODE     = 0x15,         // LIN WakeUp Frame
    EXXO_CODE_OPENPROJECT           = 0x16,         // Open a new project
    EXXO_CODE_SUSPENDREPLAY         = 0x17,         // Suspend the current replay
    EXXO_CODE_RESUMEREPLAY          = 0x18,         // Open a new project
    EXXO_CODE_ISO_WAIT_RESPONSE     = 0x19,         // Wait for a iso frame without sending a command (in master mode)
    EXXO_CODE_ISO_CHANGE_BAUDRATE   = 0x20,         // Change the baudrate
	EXXO_CODE_I2C_TRANSFER			= 0x21,         // Envoi I2C
	EXXO_CODE_CAN_CHANGE_BAUDRATE	= 0x22,			// Change the CAN baudrate
	EXXO_CODE_END                   = 0xFFFFFFFF    // End of enum
} tExxoCodeFifo;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    tCanMsg         hCanMsg;
} tExxoCanMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    tLinMsg         hLinMsg;
} tExxoLinMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    unsigned short  wChannel;
    tNwcMsg         hNwcMsg;
} tExxoNwcMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    tVanMsg         hVanMsg;
} tExxoVanMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    tIsoMsg         hIsoMsg;
} tExxoIsoMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    tIso14230Msg    hIso14230Msg;
} tExxoIso14230Msg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
    unsigned short	wTimeOut;
} tExxoIsoWaitResponse;

typedef struct
{
	unsigned short  wCard;
	unsigned short  wBus;
	tIsoBaudRate	eBaudRateTx;
	tIsoBaudRate	eBaudRateRx;
} tExxoIsoChangeBaudrate;

typedef struct
{
	unsigned short	wCard;
	unsigned short	wBus;
	tCanBus			hCanBus;
} tExxoCanChangeBaudrate;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wOutputValue;
    unsigned short  wOutputMask;
} tExxoIOSetOutput;

typedef struct
{
    char            szText[1024];
} tExxoDisplayMsg;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
} tExxoLinSetSleepMode;

typedef struct
{
    unsigned short  wCard;
    unsigned short  wBus;
} tExxoLinSetWakeUpMode;

typedef struct
{
    char			szProjectPath[256];
	unsigned short	wAutoRun;
	unsigned short	wNoSave;
} tExxoOpenProject;

typedef struct
{
	unsigned short	wCard;
	tI2CInfo		hI2CInfo;
} tExxoI2CTransfer;

// Global communication structure
typedef struct
{
	tExxoCodeFifo   eExxoCodeFifo;
	union
	{
		tExxoCanMsg             hExxoCanMsg;
		tExxoLinMsg             hExxoLinMsg;
		tExxoNwcMsg             hExxoNwcMsg;
		tExxoVanMsg             hExxoVanMsg;
		tExxoIsoMsg             hExxoIsoMsg;
		tExxoIso14230Msg        hExxoIso14230Msg;
		tExxoIsoWaitResponse	hExxoIsoWaitResponse;
		tExxoIsoChangeBaudrate	hExxoIsoChangeBaudrate;
		tExxoIOSetOutput        hExxoIOSetOutput;
		tExxoDisplayMsg         hExxoDisplayMsg;
		tExxoLinSetSleepMode    hExxoLinSetSleepMode;
		tExxoLinSetWakeUpMode   hExxoLinSetWakeUpMode;
		tExxoOpenProject        hExxoOpenProject;
		tExxoI2CTransfer		hExxoI2CTransfer;
		tExxoCanChangeBaudrate	hExxoCanChangeBaudrate;
	} u;
} tExxoFifoMsg;

#define _ExxoIncFifoPtRead()  (dwExxoFifoPtRead =((dwExxoFifoPtRead+1 )%EXXO_MAX_FIFO))
#define _ExxoIncFifoPtWrite() (dwExxoFifoPtWrite=((dwExxoFifoPtWrite+1)%EXXO_MAX_FIFO))
#define _ExxoFifoFull()       (((dwExxoFifoPtWrite+1)%EXXO_MAX_FIFO) == dwExxoFifoPtRead)
#define _ExxoFifoEmpty()      (dwExxoFifoPtWrite == dwExxoFifoPtRead)


// functions declaration that can be called to send command to MuxTrace
_EXTERNC tMuxStatus DisplayMsg(char *szText);
_EXTERNC tMuxStatus Trigger(void);
_EXTERNC tMuxStatus Stop(void);
_EXTERNC tMuxStatus OpenProject(char *szProjectPath, unsigned short wAutoRun, unsigned short wNoSave);
_EXTERNC tMuxStatus SuspendReplay(void);
_EXTERNC tMuxStatus ResumeReplay(void);

_EXTERNC int _MUXPRG GetSDKVersion(void);
_EXTERNC int _MUXPRG OnPreStart(void);
_EXTERNC int _MUXPRG OnSend(tExxoFifoMsg *hCurExxoFifoMsg);
_EXTERNC int _MUXPRG GetInfos(char *szInfos, int *piVersion);
_EXTERNC int _MUXPRG OnStart(void);
_EXTERNC int _MUXPRG OnStop(void);
_EXTERNC int _MUXPRG OnGetVariableName(unsigned short wIndex, char *szVariable, char *szDefaultValue);
_EXTERNC int _MUXPRG OnSetVariableValue(const unsigned short wIndex, const char *szVariable, const char *szValue);
_EXTERNC int _MUXPRG OnKey(int lKeyValue);
_EXTERNC int _MUXPRG OnTimer(unsigned long dwTime);
_EXTERNC int _MUXPRG OnCanEvent(tCanEvent *hCanEvent);
_EXTERNC int _MUXPRG OnNwcEvent(tNwcEvent *hNwcEvent);
_EXTERNC int _MUXPRG OnLinEvent(tLinEvent *hLinEvent);
_EXTERNC int _MUXPRG OnIsoEvent(tIsoEvent *hIsoEvent);
_EXTERNC int _MUXPRG OnVanEvent(tVanEvent *hVanEvent);
_EXTERNC int _MUXPRG OnReplayFrame(ST_PRGMUX_ONREPLAYFRAME *hOnReplay, unsigned short *pwReplayThisFrame, unsigned short *pwSuspendReplay);
_EXTERNC int _MUXPRG OnTriggerReplay(ST_PRGMUX_ONTRIGGERREPLAY *hOnTrigger, unsigned short *pwSuspendReplay);
_EXTERNC int _MUXPRG OnI2CTransfer(tI2CInfo *hI2CInfo);
_EXTERNC int _MUXPRG OnShowGUI( HWND hWnd, int iWidth, int iHeight );

#pragma pack(pop) 	  			        // 2-byte alignment: MSVC & Borland

#endif  // End of _PRGMUX_H_
