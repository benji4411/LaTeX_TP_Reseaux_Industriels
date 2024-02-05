/*	OnTimer
		Called each time the timer counter has changed.

	Parameters
		dwTime > IN > New value of the timer counter, defined in milliseconds.

    Return values
    	Should return 0 on error
*/
_EXTERNC int _MUXPRG OnTimer(DWORD dwTime)
{
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

	return(1);
}