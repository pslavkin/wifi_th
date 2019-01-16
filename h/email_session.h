#ifndef  EMAIL_SESSION
#define  EMAIL_SESSION

#include "state_machine.h"
//----------------------------------------------------------------------------------------------------
enum Email_Session_Event_Code	{
					Send_Email_Event		= 0xDC00
		           	};
//----------------------------------------------------------------------------------------------------
extern void 		Email_Session_Rti	(void);
extern void 		Init_Email_Session	(void);
extern const State** 	Email_Session		(void);
//----------------------------------------------------------------------------------------------------
extern const State* 	Email_Session_App	(void);
extern void 		Send_Email		(void);
//----------------------------------------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
//All SMTP defines
#define SMTP_BUF_LEN            100
#define GMAIL_HOST_NAME         "smtp.gmail.com"
#define GMAIL_HOST_PORT         465
#define YAHOO_HOST_NAME         "smtp.mail.yahoo.com"
#define YAHOO_HOST_PORT         25
#define USER                    "slavkin.pablo@gmail.com" 	//Set Sender/Source Email Address
#define USER_RFC                "slavkin.pablo@gmail.com" 	//Set Sender/Source Email Address
#define PASS                    "trialman"              		//Set Sender/Source Email Password
#define RCPT_RFC                "ventas@disenioconinigenio.com.ar"  	//Set Recipient Email Password
#define EMAIL_SUB               "Hello from Simplelink!"
#define DEFAULT_EMAIL_MSG       "mensaje de correo"
///////////////////////////////////////////////////////////////////////////////

#endif

