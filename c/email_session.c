#include "str.h"
#include "wlan.h"
#include "uart_if.h"
#include "common.h"
#include "email.h"
#include "email_session.h"
#include "wifi_session.h"
#include "wifi_socket.h"
#include "wifi_network.h"
#include "state_machine.h"
#include "events.h"
#include "schedule.h"
#include "type_conversion.h"
#include "buttons.h"
#include "parameters.h"
#include "flash.h"
#include "debug.h"
#include "str.h"
#include "one_wire_transport.h"
#include "tmp_control.h"
#include "leds_session.h"
#include <string.h>

static const State
	Welcome[];
//	smtpINIT[],
//	smtpFROM[],
//	smtpAUTH[],
//	smtpFROM[],
//	smtpRCPT[],
//	smtpDATA[],
//	smtpMESSAGE[],
//	smtpERROR[];

const State*		Email_Session_Sm;
const State*		Email_Session_App(void)	{return Welcome;}
//------------------------------------------------------------------
void 			Init_Email_Session	(void)
{
	Email_Session_Sm=Welcome;
}
const State**	Email_Session		(void)			{return &Email_Session_Sm;}
void		Email_Session_Rti	(void)			{Atomic_Send_Event(Rti_Event,Email_Session());}
//------------WELCOME----------------------------------------------------------------------------------------
// *SMTP* UART-defined config variables for Email Application
char pcEmailto[25];
char *pcOfemailto;
char pcEmailsubject[25];
char *pcOfemailsubject;
char pcEmailmessage[64];
char *pcOfemailmessage;


static void  Welcome_Send_Email	(void)
{
}
void Something_Received_Email	(void)
{
	DBG_EMAIL_SESSION_PRINT("llego algo al mail\r\n");
}

//----------------Callback-----------------------------------------------------------
void Send_Email	(void)		{Atomic_Send_Event(Send_Email_Event,Email_Session());}
//----------------------------------------------------------------------------------------------------
static const State Welcome[] =
{
	Send_Email_Event		,Welcome_Send_Email				,Welcome,
	ANY_Event			,Something_Received_Email			,Welcome,
};

//static const State smtpINIT[] =
//{
//                // Create buffer, Read so we can chek for 220 'OK' from server
//                memset(g_cBuf,0,100);
//                //TODO: verify this use
//                lRetVal = sl_Recv(g_lSmtpSocket,g_cBuf,sizeof(g_cBuf),0);
//                if(lRetVal < 0)
//                {
//                    g_uiSmtpStatus= smtpERROR;
//                    g_uiEmailErrorInfo = SL_EMAIL_ERROR_INIT;
//                    break;
//                }
//
//                // If buffer has 220, set state to HELO
//                if(g_cBuf[0] == cSmtpCodeReady[0] &&
//                   g_cBuf[1] == cSmtpCodeReady[1] &&
//                   g_cBuf[2] == cSmtpCodeReady[2])
//                {
//                        g_uiSmtpStatus= smtpHELO;
//                }
//                // Else error, set state to ERROR
//                else
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_INIT;
//                }
//          break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpINIT[] =
//{
//			//   The client normally sends the EHLO command to the
//			//   server, indicating the client's identity.  In addition to opening the
//			//   session, use of EHLO indicates that the client is able to process
//			//   service extensions and requests that the server provide a list of the
//			//   extensions it supports.
//            case smtpHELO:
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, "HELO localhost", NULL, g_cBuf);
//                if(lRetVal < 0)
//                {
//                    g_uiSmtpStatus= smtpERROR;
//                    g_uiEmailErrorInfo = SL_EMAIL_ERROR_HELO;
//                    break;
//                }
//
//                // If buffer has 250, set state to AUTH
//                if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                         g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                         g_cBuf[2] == cSmtpCodeOkReply[2])
//                {
//                        g_uiSmtpStatus= smtpAUTH;
//                }
//                // Else error, set state to ERROR
//                else
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_HELO;
//                }
//            break;
//
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpAUTH[] =
//{
//			// The client sends the AUTH command with SASL mechanism to use with
//            case smtpAUTH:
//		// SASL PLain - Authentication with server username and password
//		//Refer - http://www.ietf.org/rfc/rfc4616.txt
//                lRetVal = _generateBase64Key((char*)g_sEmail.Username, g_cBasekey1);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                        break;
//                }
//                lRetVal = _generateBase64Key((char*)g_sSourcePass.Password, g_cBasekey2);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                        break;
//                }
//
//                // Send request to server for authentication
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, "AUTH LOGIN", NULL, g_cBuf);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                        break;
//                }
//                // If buffer is 334, give username in base64
//                if(g_cBuf[0] == cSmtpCodeAuthReply[0] &&
//                         g_cBuf[1] == cSmtpCodeAuthReply[1] &&
//                         g_cBuf[2] == cSmtpCodeAuthReply[2])
//                {
//                        lRetVal = _sendSMTPCommand(g_lSmtpSocket, g_cBasekey1, NULL, g_cBuf);
//                        if(lRetVal < 0)
//                        {
//                                g_uiSmtpStatus= smtpERROR;
//                                g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                                break;
//                        }
//                        // If buffer is 334, give password in base64
//                        if(g_cBuf[0] == cSmtpCodeAuthReply[0] &&
//                           g_cBuf[1] == cSmtpCodeAuthReply[1] &&
//                           g_cBuf[2] == cSmtpCodeAuthReply[2])
//                        {
//                                lRetVal = _sendSMTPCommand(g_lSmtpSocket, g_cBasekey2, NULL,
//                                                     g_cBuf);
//                                if(lRetVal < 0)
//                                {
//                                        g_uiSmtpStatus= smtpERROR;
//                                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                                        break;
//                                }
//                        }
//                }
//                if(g_cBuf[0] == cSmtpCodeAuthSuccess[0] &&
//                         g_cBuf[1] == cSmtpCodeAuthSuccess[1] &&
//                         g_cBuf[2] == cSmtpCodeAuthSuccess[2])
//                {
//                        // Authentication was successful, set state to FROM
//                        g_uiSmtpStatus= smtpFROM;
//                }
//                // Else error, set state to ERROR
//                else
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_AUTH;
//                }
//
//              break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpFROM[] =
//{
//             // The SMTP transaction starts with a MAIL command which includes the
//             // sender information
//             // MAIL FROM:<reverse-path> [SP <mail-parameters> ] <CRLF>
//            case smtpFROM:
//
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpMailFrom,
//                                        (char*)g_sEmail.Username, g_cBuf);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_FROM;
//                        break;
//                }
//
//                // If buffer has 250, set state to RCPT
//                if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                         g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                         g_cBuf[2] == cSmtpCodeOkReply[2])
//                {
//                        g_uiSmtpStatus= smtpRCPT;
//                }
//                else
//                {
//                        memcpy(g_cUsernameRfc,"<",1);
//                        memcpy(&g_cUsernameRfc[1], g_sEmail.Username,
//                                    strlen((char*)g_sEmail.Username));
//                        memcpy(&g_cUsernameRfc[1+strlen((char*)g_sEmail.Username)],
//                                   ">",1);
//                        lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpMailFrom,
//                                                         g_cUsernameRfc, g_cBuf);
//                        if(lRetVal < 0)
//                        {
//                                g_uiSmtpStatus= smtpERROR;
//                                g_uiEmailErrorInfo = SL_EMAIL_ERROR_FROM;
//                                break;
//                        }
//                        if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                           g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                           g_cBuf[2] == cSmtpCodeOkReply[2])
//                        {
//                                g_uiSmtpStatus= smtpRCPT;
//                        }
//                        else
//                        {
//                            g_uiSmtpStatus= smtpERROR;
//                            g_uiEmailErrorInfo = SL_EMAIL_ERROR_FROM;
//                        }
//                }
//              break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpRCPT[] =
//{
//            // Send the destination email to the smtp server
//            //RCPT TO:<forward-path> [ SP <rcpt-parameters> ] <CRLF>
//            case smtpRCPT:
//
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpRcpt,
//                             (char*)g_sDestination.Email, g_cBuf);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_RCPT;
//                        break;
//                }
//
//                // If buffer has 250, set state to DATA
//                if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                         g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                         g_cBuf[2] == cSmtpCodeOkReply[2])
//                {
//                    g_uiSmtpStatus= smtpDATA;
//                }
//                else
//            {
//                    memcpy(g_cDestinationEmailRfc,"<",1);
//                    memcpy(&g_cDestinationEmailRfc[1], g_sDestination.Email,
//                                    strlen((char*)g_sDestination.Email));
//
//                    memcpy(&g_cDestinationEmailRfc
//                            [1+strlen((char*)g_sDestination.Email)],">",1);
//
//                    lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpRcpt,
//                                                      g_cDestinationEmailRfc, g_cBuf);
//                    if(lRetVal < 0)
//                    {
//                            g_uiSmtpStatus= smtpERROR;
//                            g_uiEmailErrorInfo = SL_EMAIL_ERROR_RCPT;
//                            break;
//                    }
//                    // If buffer has 250, set state to DATA
//                    if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                       g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                       g_cBuf[2] == cSmtpCodeOkReply[2])
//                    {
//                        g_uiSmtpStatus= smtpDATA;
//                    }
//                    else
//                    {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_RCPT;
//                    }
//                }
//              break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpDATA[] =
//{
//            // Send the "DATA" message to the server, indicates client is ready to
//            // construct  the body of the email
//            // DATA <CRLF>
//            case smtpDATA:
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpData, NULL,
//                                                 g_cBuf);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_DATA;
//                        break;
//                }
//                // If buffer has 250, set state to MESSAGE
//                if(g_cBuf[0] == cSmtpCodeIntermedReply[0] &&
//                         g_cBuf[1] == cSmtpCodeIntermedReply[1] &&
//                         g_cBuf[2] == cSmtpCodeIntermedReply[2])
//                {
//                        g_uiSmtpStatus= smtpMESSAGE;
//                }
//                else
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_DATA;
//                }
//              break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpMESSAGE[] =
//{
//            case smtpMESSAGE:
//                // Send actual Message, preceded by FROM, TO and Subject
//
//                // Start with E-Mail's "Subject:" field
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpSubject,
//                                                   (char*)g_sSubject.Value, NULL);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // Add E-mail's "To:" field
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpTo,
//                                                  (char*)g_sDestination.Email, NULL);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // Add E-mail's "From:" field
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, (char *)cSmtpFrom,
//                                                   (char*)g_sEmail.Username, NULL);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // Send CRLF
//                lRetVal = sl_Send(g_lSmtpSocket,cSmtpCrlf,strlen(cSmtpCrlf),0);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // Send body of message
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket, g_cMessage, NULL, NULL);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // End Message
//                lRetVal = _sendSMTPCommand(g_lSmtpSocket,(char *)cSmtpDataEnd,NULL,
//                                                 g_cBuf);
//                if(lRetVal < 0)
//                {
//                        g_uiSmtpStatus= smtpERROR;
//                        g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                        break;
//                }
//
//                // Server will send 250 is successful. Move into QUIT state.
//                if(g_cBuf[0] == cSmtpCodeOkReply[0] &&
//                         g_cBuf[1] == cSmtpCodeOkReply[1] &&
//                         g_cBuf[2] == cSmtpCodeOkReply[2])
//                {
//                        // Disconnect from server by sending QUIT command
//                    lRetVal = sl_Send(g_lSmtpSocket,cSmtpQuit,strlen(cSmtpQuit),0);
//                    if(lRetVal < 0)
//                    {
//                            g_uiSmtpStatus= smtpERROR;
//                            g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                            break;
//                    }
//                    // Close socket and reset
//                    sl_Close(g_lSmtpSocket);
//                    g_lSmtpSocket = 0xFFFFFFFF;
//
//                    //Reset the state machine
//                    g_uiSmtpStatus= smtpINIT;
//
//                    exit = 1;
//                    return g_uiEmailErrorInfo;
//                }
//                else
//                {
//                    g_uiSmtpStatus= smtpERROR;
//                    g_uiEmailErrorInfo = SL_EMAIL_ERROR_MESSAGE;
//                }
//
//                break;
// ANY_Event			,Rien						,Welcome,
//};
//static const State smtpERROR[] =
//{
//            // Error Handling for SMTP
//            case smtpERROR:
//                // Close socket and reset
//                sl_Close(g_lSmtpSocket);
//
//                //Reset the state machine
//                g_uiSmtpStatus= smtpINIT;
//
//                exit = 1;
//                return g_uiEmailErrorInfo;
//
// ANY_Event			,Rien						,Welcome,
//};
