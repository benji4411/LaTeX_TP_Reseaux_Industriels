//   Copyright (c) by Annecy Electronique, Saint Jorioz, France
//
//   Environment :    PC  (Langage C)
//
//   DESCRIPTION:
//
//   Déclaration des structures de communications entre un programme d'extension et MuxTrace
//   CE FICHIER NE DOIT PAS ETRE MODIFIE.
//   CE FICHIER DOIT ETRE INCLUT A VOTRE PROJET.
//
//   Structures declaration between plugin programm and MuxTrace software
//   THIS FILE MUST NOT BE CHANGED.
//   THIS FILE MUST BE INCLUDED TO YOUR PROJECT.
//
//   HISTORY:
//
//		V 1.00     Philippe CHAZOT      04/03/2004
//		- Created.
//
//		V 1.01     Alexandre GAMBIER    03/02/2005
//		- Add MUXPRG_SDK_VERSION constant value WHICH MUST NOT BE CHANGED
//		- Add GetSDKVersion exported function which is used in order to get Muxtrace software
//		  the possibility to know the SDK used.
//		- Add GetInfos exported function which gives the possibility to the to write informations
//		  such as version number and a description string.
//		- Add OnNwcEvent exported functions
//		- Add OnGetVariableName exported functions which give the possibility to use external
//		  variable intialized in the muxtrace interface.
//		- Add OnSetVariableValue exported functions which give the possibility to use external
//		  variable intialized in the muxtrace interface.
//		- Add OnReplayFrame exported functions which give the possibility to schedule the replay
//		- Add OnTriggerReplay exported functions which give the possibility to schedule the replay
//		- Add LinSetMode internal function support.
//		- Add LinWakeUpMode internal function support.
//		- Add Iso14230SendMsg internal function support.
//		- Add OpenProject internal function support
//		- Add SuspendReplay internal function support
//		- Add ResumeReplay internal function support

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "prgmux.h"
#ifdef  __BORLANDC__
#pragma hdrstop
#endif
unsigned int dwExxoFifoPtRead=0,dwExxoFifoPtWrite=0;
tExxoFifoMsg hExxoFifoMsg[EXXO_MAX_FIFO];

_EXTERNC int _MUXPRG GetSDKVersion(void)
{
    return(MUXPRG_SDK_VERSION);
}

_EXTERNC int _MUXPRG OnPreStart(void)
{
    dwExxoFifoPtRead=0;
	dwExxoFifoPtWrite=0;
    return(1);
}

_EXTERNC int _MUXPRG OnSend(tExxoFifoMsg *hCurExxoFifoMsg)
{
    if (!_ExxoFifoEmpty())
    {
        *hCurExxoFifoMsg=hExxoFifoMsg[dwExxoFifoPtRead];
        _ExxoIncFifoPtRead();
        return(1);
    }
    return(0);
}


_EXTERNC tMuxStatus DisplayMsg(char *szText)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_DISPLAY_MSG;
        strcpy(hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoDisplayMsg.szText,szText);
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI CanSendMsg(unsigned short wCard, unsigned short wBus, tCanMsg *hCanMsg)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_CAN_SEND_MSG;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoCanMsg.wCard  = wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoCanMsg.wBus   = wBus;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoCanMsg.hCanMsg= *hCanMsg;
		_ExxoIncFifoPtWrite();
		return(STATUS_OK);
	}
	else
	{
		return(STATUS_ERR_FIFOFULL);
	}
}

tMuxStatus _MUXAPI LinSendMsg(unsigned short wCard, unsigned short wBus, tLinMsg *hLinMsg)
{
	if (!_ExxoFifoFull())
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_LIN_SEND_MSG;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinMsg.wCard  = wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinMsg.wBus   = wBus;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinMsg.hLinMsg= *hLinMsg;
		_ExxoIncFifoPtWrite();
		return(STATUS_OK);
	}
	else
	{
		return(STATUS_ERR_FIFOFULL);
	}
}

tMuxStatus _MUXAPI IsoSendMsg(unsigned short wCard, unsigned short wBus, tIsoMsg *hIsoMsg)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_ISO_SEND_MSG;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoMsg.wCard  = wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoMsg.wBus   = wBus;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoMsg.hIsoMsg= *hIsoMsg;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
		return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI Iso14230SendMsg(unsigned short wCard, unsigned short wBus, tIso14230Msg *hIso14230Msg)
{
	if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_ISO14230_SEND_MSG;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIso14230Msg.wCard        = wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIso14230Msg.wBus         = wBus;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIso14230Msg.hIso14230Msg = *hIso14230Msg;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI IsoWaitResponse(unsigned short wCard, unsigned short wBus, unsigned short wTimeOut)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_ISO_WAIT_RESPONSE;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoWaitResponse.wCard    = wCard;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoWaitResponse.wBus     = wBus;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoWaitResponse.wTimeOut = wTimeOut;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI IsoChangeBaudRate(unsigned short wCard, unsigned short wBus, tIsoBaudRate eBaudRateTx, tIsoBaudRate eBaudRateRx)
{
	if (!_ExxoFifoFull())
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_ISO_CHANGE_BAUDRATE;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoChangeBaudrate.wCard       = wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoChangeBaudrate.wBus        = wBus;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoChangeBaudrate.eBaudRateTx = eBaudRateTx;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIsoChangeBaudrate.eBaudRateRx = eBaudRateRx;
		_ExxoIncFifoPtWrite();
		return(STATUS_OK);
	}
	else
	{
		return(STATUS_ERR_FIFOFULL);
	}
}

tMuxStatus _MUXAPI CanChangeBaudRate( unsigned short wCard, unsigned short wBus, tCanBus *hCanBus )
{
	if ( !_ExxoFifoFull() )
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[ dwExxoFifoPtWrite ].eExxoCodeFifo=EXXO_CODE_CAN_CHANGE_BAUDRATE;
		hExxoFifoMsg[ dwExxoFifoPtWrite ].u.hExxoCanChangeBaudrate.wCard	= wCard;
		hExxoFifoMsg[ dwExxoFifoPtWrite ].u.hExxoCanChangeBaudrate.wBus		= wBus;
		hExxoFifoMsg[ dwExxoFifoPtWrite ].u.hExxoCanChangeBaudrate.hCanBus	= *hCanBus;
		_ExxoIncFifoPtWrite();
		return( STATUS_OK );
	}
	else
	{
		return( STATUS_ERR_FIFOFULL );
	}
}

tMuxStatus _MUXAPI VanSendMsg(unsigned short wCard, unsigned short wBus, tVanMsg *hVanMsg)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_VAN_SEND_MSG;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoVanMsg.wCard  = wCard;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoVanMsg.wBus   = wBus;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoVanMsg.hVanMsg= *hVanMsg;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI IOSetOutput(unsigned short wCard, unsigned short wOutputValue, unsigned short wOutputMask)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_IO_SET_OUTPUT;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIOSetOutput.wCard        = wCard;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIOSetOutput.wOutputValue = wOutputValue;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoIOSetOutput.wOutputMask  = wOutputMask;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI LinSetSleepMode(unsigned short wCard, unsigned short wBus)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_LIN_SETSLEEPMODE;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinSetSleepMode.wCard  = wCard;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinSetSleepMode.wBus   = wBus;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
	else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

tMuxStatus _MUXAPI LinSetWakeUpMode(unsigned short wCard, unsigned short wBus)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_LIN_SETWAKEUPMODE;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinSetWakeUpMode.wCard  = wCard;
        hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoLinSetWakeUpMode.wBus   = wBus;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

_EXTERNC tMuxStatus Trigger(void)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_TRIGGER;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

_EXTERNC tMuxStatus Stop(void)
{
    if (!_ExxoFifoFull())
    { /* Message is copy into the fifo */
        hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_STOP;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
		return(STATUS_ERR_FIFOFULL);
    }
}

_EXTERNC tMuxStatus OpenProject(char *szProjectPath, unsigned short wAutoRun, unsigned short wNoSave)
{
	if (!_ExxoFifoFull())
	{	/* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_OPENPROJECT;
		strcpy(hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoOpenProject.szProjectPath, szProjectPath);
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoOpenProject.wAutoRun = wAutoRun;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoOpenProject.wNoSave  = wNoSave;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

_EXTERNC tMuxStatus SuspendReplay(void)
{
	if (!_ExxoFifoFull())
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_SUSPENDREPLAY;
        _ExxoIncFifoPtWrite();
        return(STATUS_OK);
    }
    else
    {
        return(STATUS_ERR_FIFOFULL);
    }
}

_EXTERNC tMuxStatus ResumeReplay(void)
{
	if (!_ExxoFifoFull())
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_RESUMEREPLAY;
		_ExxoIncFifoPtWrite();
		return(STATUS_OK);
	}
	else
	{
		return(STATUS_ERR_FIFOFULL);
	}
}

tMuxStatus _MUXAPI I2CTransfer(unsigned short wCard, tI2CInfo *hI2CInfo)
{
	if (!_ExxoFifoFull())
	{ /* Message is copy into the fifo */
		hExxoFifoMsg[dwExxoFifoPtWrite].eExxoCodeFifo=EXXO_CODE_I2C_TRANSFER;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoI2CTransfer.wCard		= wCard;
		hExxoFifoMsg[dwExxoFifoPtWrite].u.hExxoI2CTransfer.hI2CInfo	= *hI2CInfo;
		_ExxoIncFifoPtWrite();
		return(STATUS_OK);
	}
	else
	{
		return(STATUS_ERR_FIFOFULL);
	}
}

