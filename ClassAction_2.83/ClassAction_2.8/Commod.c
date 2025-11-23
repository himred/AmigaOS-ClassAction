
/****************************************

        Class Action Source

        (c) by 1994 Salim Gasmi

        File : Commod.c

        Goal : Handle commodity stuff

        Ver  : 2.00

****************************************/

#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/alib_protos.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <workbench/startup.h>
#include <workbench/Workbench.h>
#include <lib/rexx.h>
#include "gadgets.h"
#include "includes.h"
#include <stdio.h>
#include <libraries/commodities.h>

#include "ClassAction_rev.h"

#define CX_SHOW 1l

extern CxObj *Broker;

/****************************************/
/*                                      */
/* Function : InitCx()                  */
/*                                      */
/* Goal     : Initialize the Commodity  */
/*                                      */
/* Return Values :                      */
/*                                      */
/*      -1: failed                      */
/*       0: Okay                        */
/*                                      */
/****************************************/

int InitCx()
{

        char to[200];
        int errorcode;
        CxObj *hotkey;

        sprintf(to, "%s  � 1995 Gasmi Salim.", VERS);

        struct NewBroker newbroker =
            {
                NB_VERSION,
                "ClassAction",
                to,
                LocaleMsg[CA_COMMOD],
                NBU_NOTIFY | NBU_UNIQUE,
                COF_SHOW_HIDE,
                0,
                NULL,
                0};

        CxBase = (struct Library *)OpenLibrary("commodities.library", 37l);
        if (CxBase == NULL)
                return (-1);

        CxPort = (struct MsgPort *)CreateMsgPort();
        if (CxPort == NULL)
                return (-1);

        SigCx = 1 << CxPort->mp_SigBit;

        newbroker.nb_Pri = CxPri;
        newbroker.nb_Port = CxPort;

        Broker = (CxObj *)CxBroker(&newbroker, &errorcode);
        if (errorcode == CBERR_DUP || Broker == NULL)
                return (-1);

        hotkey = HotKey(CAHotKey, CxPort, CX_SHOW);
        AttachCxObj(Broker, hotkey);

        ActivateCxObj(Broker, 1l);
        return (0);
}

/****************************************/
/*                                      */
/* Function : HandleCx()                */
/*                                      */
/* Goal     : Handle Cx messages        */
/*                                      */
/* Return Values :                      */
/*                                      */
/*      0: Nothing important got        */
/*      1: we must kill ClassAction     */
/*      2: Show/Hide The Interface      */
/*                                      */
/****************************************/

int HandleCx()
{
        struct Message *cx_msg;
        int msgid, msgtype;

        while (cx_msg = (struct Message *)GetMsg(CxPort))
        {
                msgid = CxMsgID((CxMsg *)cx_msg);
                msgtype = CxMsgType((CxMsg *)cx_msg);
                ReplyMsg(cx_msg);
                switch (msgtype)
                {
                case CXM_IEVENT:
                        switch (msgid)
                        {
                        case CX_SHOW:
                                return (2);
                        }

                        break;

                case CXM_COMMAND:
                        switch (msgid)
                        {
                        case CXCMD_UNIQUE:
                                return (0);
                        case CXCMD_KILL:
                                return (1);
                        case CXCMD_APPEAR:
                                if (Test_Wnd == NULL)
                                        return (2);
                                break;
                        case CXCMD_DISAPPEAR:
                                if (Test_Wnd != NULL)
                                        return (2);
                                break;
                        case CXCMD_DISABLE:
                                ActivateCxObj(Broker, 0l);
                                break;
                        case CXCMD_ENABLE:
                                ActivateCxObj(Broker, 1l);
                        }
                        break;
                }
        }
        return (0);
}

/****************************************/
/*                                      */
/* Function : FreeCx()                  */
/*                                      */
/* Goal     : Free allocated stuff      */
/*            for the commodity         */
/*                                      */
/* Return Values : NONE                 */
/*                                      */
/****************************************/

void FreeCx()
{
        struct Message *msg;

        if (Broker)
                DeleteCxObjAll(Broker);
        if (CxPort)
        {
                while (msg = (struct Message *)GetMsg(CxPort))
                        ReplyMsg(msg);
                DeleteMsgPort(CxPort);
        }
        if (CxBase)
                CloseLibrary(CxBase);
}
