#pragma once




class Coordinator
{

public:
	
	Coordinator();
	~Coordinator();
	void v_UartInit(void);
	void v_CommandInit(void);
	
	void v_SendByte(char byte);
	void v_SendString(char* string);
	void v_SendCommand(int CommandNumber);
	void v_SendBroadcast(char* Text);
	void v_SendUniCast(char* Text, int ModuleNum);
	void v_FrameAn(char* s_Frame,char ch_Case);
	
	void V_CastRecieved(char* s_Frame);
	void v_NewNodeJoined(char* s_Frame);
	void v_Error(char* s_Frame);
	
	
	int i_AdressTab[20];	// mo¿e 2 wymiarowa char?	- tablica na EUI ka¿dego modu³u
	char s_TempBuffor[1024];
	char s_Frame[1024];
	char s_CommandAn[1024];	
	bool b_HasCommand;
	
	char s_LastCommandSend[1024];
	char s_LastFrameRecieved[1024];
	char s_LastError[2];
	char s_CommandsSet[20][20];	// tablica komend typu AT+BCAST,00: lub AT+JN itp
	bool b_LastSuccess;
	int i_Poss;

};