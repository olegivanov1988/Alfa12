/*
 * function.c
 *
 * Created: 29.08.2021 13:30:36
 *  Author: oleg
 */ 
#include "main.h"
void LCDSetup2();

extern float	Delay;
extern uint8_t	Language;
//----------------------------------------/ FUNCTION /----------------------------------------------
// ��������� �������������
void Start_Init()
{
	// ��������� ������ ������� ����������
	cli();
		
	// ����� ������ ��������� ��������� ���
	LCDSetup2();		
	
	DDRG |= (1 << PING4);			// ���������� ��� ����� ��� � ��������� "������"
	//PORTG |= (1 << PORTG4);
		
	DDRE &= ~(1 << DDE7);
	PORTE |= (1 << PORTE7);
	EICRB |= (1 << ISC71);			// ISC71 = 1 � ISC70 = 0 - ���������� �� ��������� ������ (������� �������)
	//EICRB |= (1 << ISC70);		// ISC71 = 1 � ISC70 = 1 - ���������� �� ������������ ������ (������� �������)
	EIMSK |= (1 << INT7);
	
	// ����������� 16�� ��������� ������ ������� 1
	TCCR1A = 0;								// �� ���������
	TCCR1B |= (1 << CS12) | (1 << CS10);	// ������������� �������� 1024
	TCCR1B &= ~(1 << CS11);					// 
		
	// �������������� ��� ������
	DDRA |= (1 << DDRA0) | (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3) | (1 << DDRA4) | (1 << DDRA5);				// ��� ������
	PORTA &= ~(1 << PORTA0) & ~(1 << PORTA1) & ~(1 << PORTA2) & ~(1 << PORTA3) & ~(1 << PORTA4)	& ~(1 << PORTA5);	// ������������� 0
	
	DDRB |= (1 << DDRB0) | (1 << DDRB1) | (1 << DDRB2) | (1 << DDRB3) | (1 << DDRB4) | (1 << DDRB5);				// ��� ������
	PORTB &= ~(1 << PORTB0) & ~(1 << PORTB1) & ~(1 << PORTB2) & ~(1 << PORTB3) & ~(1 << PORTB4)	& ~(1 << PORTB5);	// ������������� 0
	
	DDRF |= (1 << DDRF0) | (1 << DDRF1) | (1 << DDRF2) | (1 << DDRF3) | (1 << DDRF4) | (1 << DDRF5);				// ��� ������
	PORTF &= ~(1 << PORTF0) & ~(1 << PORTF1) & ~(1 << PORTF2) & ~(1 << PORTF3) & ~(1 << PORTF4)	& ~(1 << PORTF5);	// ������������� 0
	
	DDRD |= (1 << DDRD0) | (1 << DDRD1) | (1 << DDRD2) | (1 << DDRD3) | (1 << DDRD4) | (1 << DDRD5);				// ��� ������
	PORTD &= ~(1 << PORTD0) & ~(1 << PORTD1) & ~(1 << PORTD2) & ~(1 << PORTD3) & ~(1 << PORTD4)	& ~(1 << PORTD5);	// ������������� 0
	
	DDRC |= (1 << DDRC0) | (1 << DDRC1) | (1 << DDRC2) | (1 << DDRC3) | (1 << DDRC4) | (1 << DDRC5);				// ��� ������
	PORTC &= ~(1 << PORTC0) & ~(1 << PORTC1) & ~(1 << PORTC2) & ~(1 << PORTC3) & ~(1 << PORTC4)	& ~(1 << PORTC5);	// ������������� 0
	
	DDRE |= (1 << DDRE0) | (1 << DDRE1) | (1 << DDRE2) | (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);				// ��� ������
	PORTA &= ~(1 << PORTE0) & ~(1 << PORTE1) & ~(1 << PORTE2) & ~(1 << PORTE3) & ~(1 << PORTE4)	& ~(1 << PORTE5);	// ������������� 0
	
	// �������������� �����
	DDRE |= (1 << DDRE6);		// ��� ������
	PORTE |= (1 << PORTE6);		// ������������� 1
		
	// ��������� ������ ���������� ����������
	sei();
}

//----------------------------------------/ FUNCTION /----------------------------------------------
#if VERSION == 12
static char *string0[] = {"����� 12", "ALFA 12"};
#else
static char *string0[] = {"����� 1", "ALFA 1"};
#endif
void MainScreen()
{
	LCDClear2();				// ������� �����
	
	putsxy2(4, LINE2, string0[0 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
static char *string1[] = {" ������� ������", "  Push button", "   ���������", "  measurement"};
void StandByScreen()
{
	LCDClear2();				// ������� �����
	
	putsxy2(0, LINE2, string1[0 + Language]);
	putsxy2(0, LINE3, string1[2 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
static char *string2[] = {"���������       ", "Measurement     ", "���������       ", "Thermocouple    ", "����������1     ", "Voltage #1      ", "����������2     ", "Voltage #2      "};
void Start(void)
{
	LCDClear2();				// ������� �����
	
	putsxy2(0, LINE1, string2[0 + Language]);
	putsxy2(0, LINE2, string2[2 + Language]);
	putsxy2(0, LINE3, string2[4 + Language]);
	//putsxy2(0, LINE4, string2[6 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������ "��������� ���������" �� �����
static char *string3[] = {"   ���������", " Measurement is", "   ���������", "   finalized"};
void Finished(void)
{
	LCDClear2();				// ������� �����	
	
	putsxy2(0, LINE2, string3[0 + Language]);
	putsxy2(0, LINE3, string3[2 + Language]);
	
	_delay_ms(3000);			// ����������� �����
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ��������� ������ ����� �������
static char *number_main_ch[] = {" 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9", "10", "11", "12"};
static char *channel_1[] = {"13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24"};
static char *channel_2[] = {"25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36"};	
void Measurement(uint8_t current_main_ch)
{
	putsxy2(14, LINE1, number_main_ch[current_main_ch]);
	if(current_main_ch < 6) {
		DDRA |= (1 << current_main_ch);
		PORTA |= (1 << current_main_ch);
	} else {
		DDRB |= (1 << (current_main_ch - 6));
		PORTB |= (1 << (current_main_ch - 6));		
	}
	
	// ������� ����� ���������
	putsxy2(14, LINE2, number_main_ch[current_main_ch]);
	
	// ����������
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);

	// ��������� ������� �����
	if(current_main_ch < 6)
	PORTA &= ~(1 << current_main_ch);
	else
	PORTB &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// ����������� ����� 1�
	
	// �������� �������� 1
	putsxy2(14, LINE3, channel_1[current_main_ch]);				// ������� ����� ������ � ����������1
	if(current_main_ch < 6) {
		DDRF |= (1 << current_main_ch);
		PORTF |= (1 << current_main_ch);
	} else {
		DDRD |= (1 << (current_main_ch - 6));
		PORTD |= (1 << (current_main_ch - 6));
	}
	
	// ����������
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);	
	
	// ��������� �������� 1
	if(current_main_ch < 6)
		PORTF &= ~(1 << current_main_ch);
	else
		PORTD &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// ����������� ����� 1�
			
	// �������� �������� 2
	putsxy2(14, LINE4, channel_2[current_main_ch]);
	if(current_main_ch < 6) {
		DDRC |= (1 << current_main_ch);
		PORTC |= (1 << current_main_ch);
	} else {
		DDRE |= (1 << (current_main_ch - 6));
		PORTE |= (1 << (current_main_ch - 6));
	}
	
	// ����������
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);
	
	// ��������� �������� 2
	if(current_main_ch < 6)
		PORTC &= ~(1 << current_main_ch);
	else
		PORTE &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// ����������� ����� 1�
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������������� ������ �� �������
// ���������� ����� ���������� ������
uint8_t Measurement_v2(uint8_t channel)
{
	if(channel >= 0 && channel < 12) {
		// ������� ����� ���������
		putsxy2(14, LINE1, number_main_ch[channel]);
		if(channel < 6) {
			DDRA |= (1 << channel);
			PORTA |= (1 << channel);
		} else {
			DDRB |= (1 << (channel - 6));
			PORTB |= (1 << (channel - 6));
		}
		
		// ������� ����� ���������
		putsxy2(14, LINE2, number_main_ch[channel]);
		
		// ����������
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);							// ����������� ����� �������� � ���������

		// ��������� ������� �����
		if(channel < 6)
			PORTA &= ~(1 << channel);
		else
			PORTB &= ~(1 << (channel - 6));
		
		return 	(channel + 12);
	} else if(channel >= 12 && channel < 24) {
		channel = channel - 12;
		// �������� �������� 1
		putsxy2(14, LINE3, channel_1[channel]);		// ������� ����� ������ � ����������1
		if(channel < 6) {
			DDRF |= (1 << channel);
			PORTF |= (1 << channel);
		} else {
			DDRD |= (1 << (channel - 6));
			PORTD |= (1 << (channel - 6));
		}
		
		// ����������
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);						// ����������� ����� �������� � ���������
		
		// ��������� �������� 1
		if(channel < 6)
			PORTF &= ~(1 << channel);
		else
			PORTD &= ~(1 << (channel - 6));
		
		if(channel == 11) return (LAST_CHANNEL);
		return 	(channel + 1);
	} /*else if(channel >= 24 && channel < 36) {
		channel = channel - 24;
		// �������� �������� 2
		putsxy2(14, LINE4, channel_2[channel]);
		if(channel < 6) {
			DDRC |= (1 << channel);
			PORTC |= (1 << channel);
		} else {
			DDRE |= (1 << (channel - 6));
			PORTE |= (1 << (channel - 6));
		}
		
		// ����������
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);						// ����������� ����� �������� � ���������
		
		// ��������� �������� 2
		if(channel < 6)
			PORTC &= ~(1 << channel);
		else
			PORTE &= ~(1 << (channel - 6));
		
		if(channel == 11) return (LAST_CHANNEL);
		return 	(channel + 1);
	}*/
}
				/* old version*/
/*uint8_t Measurement_v2(uint8_t channel)
{
	if(channel >= 0 && channel < 12) {
		// ������� ����� ���������
		putsxy2(14, LINE1, number_main_ch[channel]);
		if(channel < 6) {
			DDRA |= (1 << channel);
			PORTA |= (1 << channel);
		} else {
			DDRB |= (1 << (channel - 6));
			PORTB |= (1 << (channel - 6));
		}
	
		// ������� ����� ���������
		putsxy2(14, LINE2, number_main_ch[channel]);
	
		// ����������
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);

		// ��������� ������� �����
		if(channel < 6)
			PORTA &= ~(1 << channel);
		else
			PORTB &= ~(1 << (channel - 6));
		_delay_ms(Delay);							// ����������� ����� �������� � ���������
		
		return 	(channel + 12); 
	} else if(channel >= 12 && channel < 24) {
		channel = channel - 12;
 		// �������� �������� 1
		putsxy2(14, LINE3, channel_1[channel]);		// ������� ����� ������ � ����������1
		if(channel < 6) {
			DDRF |= (1 << channel);
			PORTF |= (1 << channel);
		} else {
			DDRD |= (1 << (channel - 6));
			PORTD |= (1 << (channel - 6));
		}
	
		// ����������
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);
	
		// ��������� �������� 1
		if(channel < 6)
			PORTF &= ~(1 << channel);
		else
			PORTD &= ~(1 << (channel - 6));
		_delay_ms(Delay);						// ����������� ����� �������� � ���������
		
		return 	(channel + 24); 
	} else if(channel >= 24 && channel < 36) {
		channel = channel - 24;
		// �������� �������� 2
		putsxy2(14, LINE4, channel_2[channel]);
		if(channel < 6) {
			DDRC |= (1 << channel);
			PORTC |= (1 << channel);
		} else {
			DDRE |= (1 << (channel - 6));
			PORTE |= (1 << (channel - 6));
		}
	
		// ����������
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);
	
		// ��������� �������� 2
		if(channel < 6)
			PORTC &= ~(1 << channel);
		else
			PORTE &= ~(1 << (channel - 6));
		_delay_ms(Delay);						// ����������� ����� �������� � ���������
		
		if(channel == 11) return (LAST_CHANNEL);
		return 	(channel + 1);
	}
}*/

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������ �������� �� �����
static char *string4[] = {"    �������:", "    Set up:", "�������", "English", "����� �������", "Debugging mode"};
void LoadOptionScreen(uint8_t currentLine)
{
	LCDClear2();				// ������� �����
	
	putsxy2(0, LINE1, string4[0 + Language]);
	putsxy2(1, LINE2, string4[2]);
	putsxy2(1, LINE3, string4[3]);
	putsxy2(1, LINE4, string4[4 + Language]);
	
	putcxy2(0, currentLine, 0x90);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������ ������� � ���� ��������
uint8_t ChangeOptions(uint8_t currentLine)
{
	switch(currentLine)
	{
		case LINE2:
			putcxy2(0, LINE2, 0x20);
			putcxy2(0, LINE3, 0x90);
			Language = ENGLISH;
			return LINE3;
			
		case LINE3:
			putcxy2(0, LINE3, 0x20);
			putcxy2(0, LINE4, 0x90);
			//Language = RUSSIAN;
			return LINE4;
			
		case LINE4:
			putcxy2(0, LINE4, 0x20);
			putcxy2(0, LINE2, 0x90);
			Language = RUSSIAN;
			return LINE2;
	}
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ����� "���������"
static char *string5[] = {"   ���������    ", "      Saved     "};
void SaveScreen()
{
	LCDClear2();				// ������� �����
	
	putsxy2(0, LINE2, string5[0 + Language]);
	
	_delay_ms(1500);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ����� "����� �������"
static char *string6[] = {" ����� �������  ", " Debugging mode ", "����� ", "Channel "};
void DebugScreen()
{
	LCDClear2();				// ������� �����
	putsxy2(0, LINE1, string6[0 + Language]);
	putsxy2(0, LINE2, string6[2 + Language]);
	
	putsxy2(8, LINE2, number_main_ch[0]);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
void ChangeDebugChannelOnSCreen(uint8_t *channel)
{
	*channel = *channel + 1;
	if(*channel == 12) *channel = 0;
	
	putsxy2(8, LINE2, number_main_ch[*channel]);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� �� ����� ����� ������� ����������� ������
static char *string7[] = {"�������� ", "turn on  ", "���������", "turn off ", "�����", "exit "};
void DebugChannelScreen(uint8_t channel)
{
	LCDClear2();				// ������� �����
	putsxy2(0, LINE1, string6[0 + Language]);
	putsxy2(0, LINE2, string6[2 + Language]);
	putsxy2(8, LINE2, number_main_ch[channel]);
	putsxy2(10, LINE2, ":");
	putsxy2(1, LINE3, string7[0 + Language]);
	putsxy2(1, LINE4, string7[4 + Language]);
	
	putcxy2(0, LINE3, 0x90);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������ ������� � ������ ������� ������
void ChangeMenuDebugChannel(uint8_t *currentNameLine)
{
	switch(*currentNameLine)
	{
		case LINE3:						// ���� ������� ������ ���� "��������" ��� "���������" 
			putcxy2(0, LINE3, 0x20);
			putcxy2(0, LINE4, 0x90);
			*currentNameLine = LINE4;
			break;
		
		case LINE4:
			putcxy2(0, LINE4, 0x20);
			putcxy2(0, LINE3, 0x90);
			*currentNameLine = LINE3;
			break;
	}	
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ������ ������������� ������ �� �������
void StartMeasurementSingleChannel(uint8_t channel)
{
	putsxy2(1, LINE3, string7[2 + Language]);	// ������ ����� ���� �� "���������"
	
	if(channel < 6) {
		DDRA |= (1 << channel);
		PORTA |= (1 << channel);
	} else {
		DDRB |= (1 << (channel - 6));
		PORTB |= (1 << (channel - 6));
	}
	
	// ����������
	PORTE &= ~(1 << PORTE6);
	_delay_us(1);
	PORTE |= (1 << PORTE6);
		
	_delay_ms(Delay);							// ����������� ����� �������� � ���������

	// ��������� ������� �����
	if(channel < 6)
		PORTA &= ~(1 << channel);
	else
		PORTB &= ~(1 << (channel - 6));
	
	// �������� �������� 1
	if(channel < 6) {
		DDRF |= (1 << channel);
		PORTF |= (1 << channel);
	} else {
		DDRD |= (1 << (channel - 6));
		PORTD |= (1 << (channel - 6));
	}

	// ����������
	PORTE &= ~(1 << PORTE6);
	_delay_us(1);
	PORTE |= (1 << PORTE6);					
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// ������� ��������� ������������� ������ �� �������
void StopMeasurementSingleChannel(uint8_t channel)
{
	//DDRB |= (1 << PINB1);
	//PORTB |= (1 << PORTB1);
	
	putsxy2(1, LINE3, string7[0 + Language]);	// ������ ����� ���� �� "��������"
	
	// ��������� �������� 1
	if(channel < 6)
		PORTF &= ~(1 << channel);
	else
		PORTD &= ~(1 << (channel - 6));
}