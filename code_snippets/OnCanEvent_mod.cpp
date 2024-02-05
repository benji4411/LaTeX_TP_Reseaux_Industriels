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
    
	return(1);
}