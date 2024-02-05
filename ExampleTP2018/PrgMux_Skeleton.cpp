/*	 Copyright (c) by Annecy Electronique, Saint Jorioz, France

   Environment :	PC	(Langage C)

   DESCRIPTION:

	D�finition du squelette � utiliser pour le d�veloppement d'un programme d'extension
    du MuxTrace. Les fonctions non souhait�es doivent mise en commentaire.

	Pour plus d'informations sur les fonctions pouvant �tre utilis�es dans ce plugin, tel que
	CanSendMsg, VanSendMsg, ..., consulter les documentations.

    CE FICHIER DOIT ETRE COPIE AVANT D'ETRE MODIFIE.
    NE PAS OUBLIER D'INCLURE LE FICHIER PRGMUX.CPP A VOTRE PROJET.

    Skeleton definition that should be used when writing plugin programm for the
    muxtrace software. Unused functions should be comment.

	For more informations about available functions that could be used in this plugin,
	see the documentation files.

    THIS FILE MUST BE COPIED BEFORE BEING CHANGED.
    DO NOT FORGET TO INCLUDE THE PRGMUX.CPP FILE TO YOUR PROJECT.

   HISTORY:
     	V 1.01	   Alexandre GAMBIER	03/02/2005
        - Created
*/

#include <windows.h>
#include <stdio.h>
#include "prgmux.h"

unsigned short wNbOfFramesBeforeSuspendReplay=10, wFramesCount=0;
tCanMsg g_hCanMsgTx;
tCanMsg g_hCanMsgTx_aux;

/*	GetInfos
		Called when the plugin is loaded in order to get some information about it.

	Parameters
    	szInfos > OUT > Null terminated string that should receive the plugin's informations.
                        Buffer size of 266 bytes inluding the null terminating character.
        iVersion > OUT > Receive the version number using BCD integer.
                         ex. v2.36 > 0x236
    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG GetInfos(char *szInfos, int *iVersion)
{
	// Informations
	strcpy(szInfos, "Sample for the muxtrace software, showing each available functions.");
    // Version
    (*iVersion) = 0x318;

	return(1);
}

/*	OnStart
		Called each time the user start a new communication session.

	Parameters
        None

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnStart(void)
{
    int i = 0;
	DisplayMsg("****************************************************************************");
	DisplayMsg("* MUXTRACE TP EXAMPLE for MASTER CSSE AND SESI");
	DisplayMsg("****************************************************************************");
	DisplayMsg("");
	DisplayMsg("Analyze and explain");

    ZeroMemory( &g_hCanMsgTx, sizeof( g_hCanMsgTx ) );
    ZeroMemory( &g_hCanMsgTx_aux, sizeof( g_hCanMsgTx_aux ) );
    // CAN frame configuration
    g_hCanMsgTx.dwIdent = 0x208; // n� Ident
    g_hCanMsgTx.eTypeId = CAN_ID_STD; // Standard ID
    g_hCanMsgTx.wDataLen = 8; // Size of frame
    // auxiliar CAN frame configuration
    g_hCanMsgTx_aux.dwIdent = 0x001; // n� Ident
    g_hCanMsgTx_aux.eTypeId = CAN_ID_STD; // Standard ID
    g_hCanMsgTx_aux.wDataLen = 8; // Size of frame
    for(i=0;i<8;i++)
    {
        g_hCanMsgTx.bData[i] = 0xFF;
        g_hCanMsgTx_aux.bData[i] = 0x00;
    }
	wFramesCount = 0;

	return(1);
}

/*	OnStop
		Called each time a communication session is stopped.

	Parameters
        None

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnStop(void)
{
	DisplayMsg("OnStop has occured");
	DisplayMsg("  Enter all the needed code before the end of the communication session...");
	DisplayMsg("");

	return(1);
}

/*	OnGetVariableName
		Called by muxtrace in order to know all exported variable that can be
		change by the user.
		Variable names are case sensitive.

	Parameters
		wIndex         > IN  > Zero based index of the variable to get (max=15)
		szVariable     > OUT > Variable name
		szDefaultValue > OUT > Default value

	Return values
		Should return 0 on error or when there is no more variables
*/
_EXTERNC int _MUXPRG OnGetVariableName(unsigned short wIndex, char *szVariable, char *szDefaultValue)
{
	DisplayMsg("OnGetVariableName has occured");

	switch(wIndex)
	{
		case 0:
			DisplayMsg("  Return 'wNbOfFramesBeforeSuspendReplay' variable");
			strcpy(szVariable    , "wNbOfFramesBeforeSuspendReplay");
			strcpy(szDefaultValue, "10");
			break;
		default:
			DisplayMsg("  Return no variable");
			return(0);
//			break;
	}
	DisplayMsg("");

	return(1);
}

/*	OnSetVariableValue
		Called each time a variable get a new value.

	Parameters
		wIndex     > IN  > Zero based index of the variable to get (max=15)
		szVariable > IN > Variable name
		szValue    > IN > Value name

	Return values
		Should return 0 on error
*/
_EXTERNC int _MUXPRG OnSetVariableValue(const unsigned short wIndex, const char *szVariable, const char *szValue)
{
	char	szTmp[512];

	/*DisplayMsg("OnSetVariableValue has occured");
	sprintf(szTmp, "  Variable '%s' get the new value '%s'", szVariable, szValue);
	DisplayMsg(szTmp);
	DisplayMsg("");*/
	switch(wIndex)
	{
		case 0:
			wNbOfFramesBeforeSuspendReplay = atoi(szValue);
			break;
	}

	return(1);
}

/*	OnKey
		Called each time the user has pressed a key.

	Parameters
        lKeyValue > IN > Value of the key that has been pressed.
                         Contains virtual key code values (VK_F1, VK_DEL, ...)
    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnKey(int lKeyValue)
{
	char	szOut[100];

	DisplayMsg("OnKey has occured");
    sprintf(szOut, "  lKeyValue=%d", lKeyValue);
    DisplayMsg(szOut);
    if(lKeyValue==VK_F2) DisplayMsg("  The user has pressed the F2 key...");

	if(lKeyValue=='r' || lKeyValue=='R')
	{
		DisplayMsg("  The user want to resume the replay");
		ResumeReplay();
	}
	else if(lKeyValue=='s' || lKeyValue=='S')
	{
		DisplayMsg("  The user want to suspend the replay");
		SuspendReplay();
	}
	DisplayMsg("");

		return(1);
}

/*	OnTimer
		Called each time the timer counter has changed.

	Parameters
		dwTime > IN > New value of the timer counter, defined in milliseconds.

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnTimer(DWORD dwTime)
{
    //Our professional code

    g_hCanMsgTx.dwIdent=0x44D;

    if((dwTime%100)==0)
    {

        //CanSendMsg(0, 0, &g_hCanMsgTx);
    }



    /*
	char	szOut[100];
	int i;
	if( (dwTime%5000)==0)
    {	// Each 5 seconds only
    	DisplayMsg("OnTimer has occured");
        sprintf(szOut, "  dwTime=%d", dwTime);
    	DisplayMsg(szOut);
    	DisplayMsg("");
    	i=0;
    }
    if( (dwTime%500)==0) // Every 500ms we enter here
    {
        // Send a CAN packet on CAN 0
        CanSendMsg( 0, 0, &g_hCanMsgTx );
        // Send a CAN packet on CAN 1
        CanSendMsg( 0, 1, &g_hCanMsgTx );
        // Send a CAN packet on CAN 2
        CanSendMsg( 0, 2, &g_hCanMsgTx );
        g_hCanMsgTx.dwIdent++;
        if(g_hCanMsgTx.dwIdent > 0x218)
        {
            g_hCanMsgTx.dwIdent = 0x208;
        }
        for(i=0;i<8;i++)
        {
            g_hCanMsgTx.bData[i]++;
        }
    }
    */

	return(1);
}

/*	OnCanEvent
		Called each time an event has occured on a CAN network.

	Parameters
        hCanEvent > IN > Definition of the event (see refmux.h and documentation files).

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnCanEvent(tCanEvent *hCanEvent)
{
    char msgDisplayed[100];

    // 100km -> 0x13 -> 00100110
    const char vitesse_limit = 0x26;
    const char volume_up = 0x08;

    const unsigned short XCAN_SI   = 0; // default set in MUXTRACE
    const unsigned short XCAN_CONF = 1;
    const unsigned short XCAN_CAR  = 2;

    // Detect events with ID 0x44D (change in speed)
    if(hCanEvent->dwIdent == 0x44D)
    {
        if(hCanEvent->wDataLen >= 1) // Security that data exists
        {
            // const char that stores the first byte of Detected Event
            const char firstByte = hCanEvent->bData[0];

            // If speed read is greater than defined limit
            if(firstByte >= vitesse_limit)
            {
                DisplayMsg("HIGH SPEED !!!");
                sprintf(msgDisplayed, "    Speed is : %d Km/h", firstByte* 10/4);
                DisplayMsg(msgDisplayed);

                // Configure auxiliar 'tCanMsg' to volume frame
                g_hCanMsgTx_aux.dwIdent = 0x21F;
                g_hCanMsgTx_aux.wDataLen = 3;
                g_hCanMsgTx_aux.bData[0] = 0x08;
                // Turn volume UP !!!
                CanSendMsg(0, XCAN_CAR, &g_hCanMsgTx_aux);
            }
            else
            {
                // Configure auxiliar 'tCanMsg' to volume frame
                g_hCanMsgTx_aux.dwIdent = 0x21F;
                g_hCanMsgTx_aux.wDataLen = 3;
                g_hCanMsgTx_aux.bData[0] = 0x04;
                // Turn volume DOWN :/
                CanSendMsg(0, XCAN_CAR, &g_hCanMsgTx_aux);
            }
        }
    }

    /*
	char			szOut[100];
    unsigned long	dwTime;

	DisplayMsg("OnCanEvent has occured");
    // Want to use it using millisecond unit
    dwTime = hCanEvent->dwTimeStamp/10;
    sprintf(szOut, "  Event has occured on network n�%d at %02d:%02d:%02d.%03d", hCanEvent->wBus+1, dwTime/3600000, dwTime/60000, dwTime/1000, dwTime%1000);

	DisplayMsg(szOut);
	DisplayMsg("");
    */
	return(1);
}

/*	OnLinEvent
		Called each time an event has occured on a LIN network.

	Parameters
        hLinEvent > IN > Definition of the event (see refmux.h and documentation files).

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnLinEvent(tLinEvent *hLinEvent)
{
	char			szOut[100];
    unsigned long	dwTime;

	DisplayMsg("OnLinEvent has occured");
    // Want to use it using millisecond unit
    dwTime = hLinEvent->dwTimeStamp/10;
    sprintf(szOut, "  Event has occured on network n�%d at %02d:%02d:%02d.%03d", hLinEvent->wBus+1, dwTime/3600000, dwTime/60000, dwTime/1000, dwTime%1000);
	DisplayMsg(szOut);
	DisplayMsg("");

	return(1);
}

/*	OnIsoEvent
		Called each time an event has occured on an ISO network.

	Parameters
        hIsoEvent > Definition of the event (see refmux.h and documentation files).

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnIsoEvent(tIsoEvent *hIsoEvent)
{
	char			szOut[100];
    unsigned long	dwTime;

	DisplayMsg("OnIsoEvent has occured");
    // Want to use it using millisecond unit
    dwTime = hIsoEvent->dwTimeStamp/10;
    sprintf(szOut, "  Event has occured on network n�%d at %02d:%02d:%02d.%03d", hIsoEvent->wBus+1, dwTime/3600000, dwTime/60000, dwTime/1000, dwTime%1000);
	DisplayMsg(szOut);
	DisplayMsg("");

	return(1);
}

/*	OnVanEvent
		Called each time an event has occured on a VAN network.

	Parameters
		hVanEvent > IN > Definition of the event (see refmux.h and documentation files).

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnVanEvent(tVanEvent *hVanEvent)
{
	char			szOut[100];
    unsigned long	dwTime;

	DisplayMsg("OnVanEvent has occured");
    // Want to use it using millisecond unit
    dwTime = hVanEvent->dwTimeStamp/10;
    sprintf(szOut, "  Event has occured on network n�%d at %02d:%02d:%02d.%03d", hVanEvent->wBus+1, dwTime/3600000, dwTime/60000, dwTime/1000, dwTime%1000);
	DisplayMsg(szOut);
	DisplayMsg("");

	return(1);
}

/*	OnReplayFrame
		Called each time a frame will be replayed

	Parameters
    	hOnReplay        > IN     > definition of the frame that will be replayed (see prgmux.h)
        wReplayThisFrame > IN/OUT > Set to 0 if you don't want to replay this frame (default value = 1)
        wSuspendReplay   > IN/OUT > Set to 1 if you to suspend the replay

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnReplayFrame(ST_PRGMUX_ONREPLAYFRAME *hOnReplay, unsigned short *pwReplayThisFrame, unsigned short *pwSuspendReplay)
{
	char	szOut[100];
 	/*
	wFramesCount++;
	if(wFramesCount%2) wReplayThisFrame = 0;
	if(wFramesCount==wNbOfFramesBeforeSuspendReplay)
	{
		wFramesCount   = 0;
		wSuspendReplay = 1;
	}
	*/
	DisplayMsg("OnReplayFrame has occured");
	switch(hOnReplay->eBusType)
	{
	   	case PRGMUX_BUSTYPE_CAN:
			sprintf(szOut, "  Replay CAN frame : Ident = %X  NbOfData = %hd", hOnReplay->dwIdent, hOnReplay->wDataLen);
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_VAN:
			sprintf(szOut, "  Replay VAN frame : Ident = %X  NbOfData = %hd", hOnReplay->dwIdent, hOnReplay->wDataLen);
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_LIN:
			sprintf(szOut, "  Replay LIN frame : Ident = %X  NbOfData = %hd", hOnReplay->dwIdent, hOnReplay->wDataLen);
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_ISO:
			sprintf(szOut, "  Replay ISO frame : Ident = %X  NbOfData = %hd", hOnReplay->dwIdent, hOnReplay->wDataLen);
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_NWC:
			sprintf(szOut, "  Replay NWC frame : Ident = %X  NbOfData = %hd", hOnReplay->dwIdent, hOnReplay->wDataLen);
			DisplayMsg(szOut);
			break;
	}
	DisplayMsg("");

	return(1);
}

/*	OnTriggerReplay
		Called each time a trigger is found into a file

	Parameters
    	hOnTrigger     > IN     > details of trigger (see prgmux.h)
        wSuspendReplay > IN/OUT > Set to 1 if you to suspend the replay

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnTriggerReplay(ST_PRGMUX_ONTRIGGERREPLAY *hOnTrigger, unsigned short *wSuspendReplay)
{
	char	szOut[100];

	DisplayMsg("OnTriggerReplay has occured");
	switch(hOnTrigger->eBusType)
	{
	   	case PRGMUX_BUSTYPE_CAN:
			sprintf(szOut, "  Trigger on CAN file");
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_VAN:
			sprintf(szOut, "  Trigger on VAN file");
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_LIN:
			sprintf(szOut, "  Trigger on LIN file");
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_ISO:
			sprintf(szOut, "  Trigger on ISO file");
			DisplayMsg(szOut);
			break;
	   	case PRGMUX_BUSTYPE_NWC:
			sprintf(szOut, "  Trigger on NWC file");
			DisplayMsg(szOut);
			break;
	}
	DisplayMsg("");

	return(1);
}
