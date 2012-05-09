#include "coordinator.h"

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>           
#include <avr/interrupt.h> 
#include <stdlib.h>


Coordinator::Coordinator(void)
{


}
Coordinator::~Coordinator(void)
{

}



void Coordinator::v_UartInit(void)
{
	UCSRC=(1<<URSEL)|(1<<UPM1)|(1<<UCSZ1)|(1<<UCSZ0);
	UBRRL=51; // Baud Rate! dodac do parametrow
	UCSRB=(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
	sei();
}


void Coordinator::v_SendByte(char byte)
{
	while(!(UCSRA&(1<<UDRE)));
	UDR=(char)byte;
}

void Coordinator::v_SendString(char* string)
{
	int i=0;

	for(i=0;i<strlen(string);i++)
	{
		v_SendByte(string[i]);
	}
}



void Coordinator::v_FrameAn(char* s_Frame,char ch_Case)
{
	if(ch_Case=='B'||ch_Case=='U'){V_CastRecieved(s_Frame);}
	else if(ch_Case=='N'){v_NewNodeJoined(s_Frame);}
	else if(ch_Case=='E'){v_Error(s_Frame); b_LastSuccess=false;}
	else if(ch_Case=='O'){b_LastSuccess=true;}
	
}

void Coordinator::V_CastRecieved(char* s_Frame)	//BCAST:000D6F000005A666,04=test     04 -> hex na int
{
	int j=0,i=0;
	char ch_TempEUI[16];
	char ch_TempLength[2];
	int i_Length=0;
	
	for(j=9;j<25;j++)
	{
		ch_TempEUI[i]=s_Frame[j];
		i++;
	}
	ch_TempLength[0]=s_Frame[26];
	ch_TempLength[1]=s_Frame[27];	
	i_Length=atoi(ch_TempLength);
	
	j=0;
	for(i=28;i<(i+i_Length);i++)
	{
		s_CommandAn[j]=s_Frame[i];
		j++;
	}
	b_HasCommand=true;
}


void Coordinator::v_NewNodeJoined(char* s_Frame)//  NEWNODE:<node EUI64>,<NodeID>,<parent EUI64> EUI=000D6F0000012345 <- jak na int
{
	int i=0,j=0;
	char ch_TempEUI[16];
	
	for(j=9;j<25;j++)
	{
		ch_TempEUI[i]=s_Frame[j];
		i++;
	}
	
	for(i=0;i<sizeof(i_AdressTab);i++)
	{
		if(i_AdressTab[i]!=NULL)
		{
			i_AdressTab[i]=atoi(ch_TempEUI);
		}
	}
}

void Coordinator::v_Error(char* s_Frame) // ERROR:XX    xx-  number xx hex!   Zobaczyæ jak przekonwertowaæ
{
	s_LastError[0]=s_Frame[6];
	s_LastError[1]=s_Frame[7];
}



SIGNAL(SIG_UART_RECV)
{
	char byte=UDR;
	char ch_Case='\0';
/*
	N - newnode joined pan	NEWNODE:<node EUI64>,<NodeID>,<parent EUI64>
	E - error occured		ERROR:<errorcode>
	B - broadcast recieved	BCAST: AT+BCAST:00,Hello world   00 - all, 01 - neighbours BCAST:000D6F000005A666,04=test
	U - unicast recieved  	UCAST:[<EUI64>]<length>=<data>   AT+UCAST:000D6F0000012345=Hello
	O - OK
*/

	if(byte!='CR' || byte!='LF')//zczaiæ znaki dla CR i LF
	{
		s_TempBuffor[i_Pos]=byte;
		if(i_Pos==0)
		{
		 	// konstrukcja switcha!
			ch_Case=byte;
		}
		i_Pos++;
	}
	else
	{
		s_Frame=*s_TempBuffor; // zczaiæ!
//		v_BuffFlush();
		v_FrameAn(s_Frame,ch_Case);	
	}
}

void Coordinator::v_SendCommand(int CommandNumber)
{
	
}

/* Switch na poszczegolne komendy!   Chyba? */
void Coordinator::v_SendBroadcast(char* Text)	//AT+BCAST:00,Hello world
{
	char s_Temp[300];
		//5 - pozycja komendy AT+BCAST:00
//	sprintf(s_Temp,"%s,%s",s_CommandsSet[5],Text); //zczaic na konstrukcjê
	v_SendString(s_Temp);
}

void Coordinator::v_SendUniCast(char* Text, int ModuleNum)	//AT+UCAST:000D6F0000012345=Hello
{	
	char s_Temp[300];
		//6 - pozycja komendy AT+UCAST
//	sprintf(s_Temp,"%s:%s=%s",s_CommandsSet[6],i_AdressTab[ModuleNum],Text); //zczaic na konstrukcjê
	v_SendString(s_Temp);
}

