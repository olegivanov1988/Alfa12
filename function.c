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
// Начальная инициализация
void Start_Init()
{
	// установка общего запрета прерываний
	cli();
		
	// вызов модуля начальных установок ЖКИ
	LCDSetup2();		
	
	DDRG |= (1 << PING4);			// определяем как выход пин с лампочкой "Работа"
	//PORTG |= (1 << PORTG4);
		
	DDRE &= ~(1 << DDE7);
	PORTE |= (1 << PORTE7);
	EICRB |= (1 << ISC71);			// ISC71 = 1 и ISC70 = 0 - прерывание по падающему фронту (нажатие клавиши)
	//EICRB |= (1 << ISC70);		// ISC71 = 1 и ISC70 = 1 - прерывание по нарастающему фронту (отжатие клавиши)
	EIMSK |= (1 << INT7);
	
	// настраиваем 16ти разрядный таймер счётчик 1
	TCCR1A = 0;								// всё отключено
	TCCR1B |= (1 << CS12) | (1 << CS10);	// устанавливаем делитель 1024
	TCCR1B &= ~(1 << CS11);					// 
		
	// инициализируем все каналы
	DDRA |= (1 << DDRA0) | (1 << DDRA1) | (1 << DDRA2) | (1 << DDRA3) | (1 << DDRA4) | (1 << DDRA5);				// как выходы
	PORTA &= ~(1 << PORTA0) & ~(1 << PORTA1) & ~(1 << PORTA2) & ~(1 << PORTA3) & ~(1 << PORTA4)	& ~(1 << PORTA5);	// устанавливаем 0
	
	DDRB |= (1 << DDRB0) | (1 << DDRB1) | (1 << DDRB2) | (1 << DDRB3) | (1 << DDRB4) | (1 << DDRB5);				// как выходы
	PORTB &= ~(1 << PORTB0) & ~(1 << PORTB1) & ~(1 << PORTB2) & ~(1 << PORTB3) & ~(1 << PORTB4)	& ~(1 << PORTB5);	// устанавливаем 0
	
	DDRF |= (1 << DDRF0) | (1 << DDRF1) | (1 << DDRF2) | (1 << DDRF3) | (1 << DDRF4) | (1 << DDRF5);				// как выходы
	PORTF &= ~(1 << PORTF0) & ~(1 << PORTF1) & ~(1 << PORTF2) & ~(1 << PORTF3) & ~(1 << PORTF4)	& ~(1 << PORTF5);	// устанавливаем 0
	
	DDRD |= (1 << DDRD0) | (1 << DDRD1) | (1 << DDRD2) | (1 << DDRD3) | (1 << DDRD4) | (1 << DDRD5);				// как выходы
	PORTD &= ~(1 << PORTD0) & ~(1 << PORTD1) & ~(1 << PORTD2) & ~(1 << PORTD3) & ~(1 << PORTD4)	& ~(1 << PORTD5);	// устанавливаем 0
	
	DDRC |= (1 << DDRC0) | (1 << DDRC1) | (1 << DDRC2) | (1 << DDRC3) | (1 << DDRC4) | (1 << DDRC5);				// как выходы
	PORTC &= ~(1 << PORTC0) & ~(1 << PORTC1) & ~(1 << PORTC2) & ~(1 << PORTC3) & ~(1 << PORTC4)	& ~(1 << PORTC5);	// устанавливаем 0
	
	DDRE |= (1 << DDRE0) | (1 << DDRE1) | (1 << DDRE2) | (1 << DDRE3) | (1 << DDRE4) | (1 << DDRE5);				// как выходы
	PORTA &= ~(1 << PORTE0) & ~(1 << PORTE1) & ~(1 << PORTE2) & ~(1 << PORTE3) & ~(1 << PORTE4)	& ~(1 << PORTE5);	// устанавливаем 0
	
	// инициализируем строб
	DDRE |= (1 << DDRE6);		// как выходы
	PORTE |= (1 << PORTE6);		// устанавливаем 1
		
	// установка общего разрешения прерываний
	sei();
}

//----------------------------------------/ FUNCTION /----------------------------------------------
#if VERSION == 12
static char *string0[] = {"АЛЬФА 12", "ALFA 12"};
#else
static char *string0[] = {"АЛЬФА 1", "ALFA 1"};
#endif
void MainScreen()
{
	LCDClear2();				// Очищаем экран
	
	putsxy2(4, LINE2, string0[0 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
static char *string1[] = {" Нажмите кнопку", "  Push button", "   измерение", "  measurement"};
void StandByScreen()
{
	LCDClear2();				// Очищаем экран
	
	putsxy2(0, LINE2, string1[0 + Language]);
	putsxy2(0, LINE3, string1[2 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
static char *string2[] = {"Измерение       ", "Measurement     ", "Термопара       ", "Thermocouple    ", "Напряжение1     ", "Voltage #1      ", "Напряжение2     ", "Voltage #2      "};
void Start(void)
{
	LCDClear2();				// Очищаем экран
	
	putsxy2(0, LINE1, string2[0 + Language]);
	putsxy2(0, LINE2, string2[2 + Language]);
	putsxy2(0, LINE3, string2[4 + Language]);
	//putsxy2(0, LINE4, string2[6 + Language]);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция вывода "Измерение закончено" на экран
static char *string3[] = {"   Измерение", " Measurement is", "   закончено", "   finalized"};
void Finished(void)
{
	LCDClear2();				// Очищаем экран	
	
	putsxy2(0, LINE2, string3[0 + Language]);
	putsxy2(0, LINE3, string3[2 + Language]);
	
	_delay_ms(3000);			// Выдерживаем паузу
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция измерения одного блока каналов
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
	
	// выводим номер термопары
	putsxy2(14, LINE2, number_main_ch[current_main_ch]);
	
	// стробируем
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);

	// закрываем главный канал
	if(current_main_ch < 6)
	PORTA &= ~(1 << current_main_ch);
	else
	PORTB &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// выдерживаем пауза 1с
	
	// включаем подканал 1
	putsxy2(14, LINE3, channel_1[current_main_ch]);				// выводим номер канала в напряжении1
	if(current_main_ch < 6) {
		DDRF |= (1 << current_main_ch);
		PORTF |= (1 << current_main_ch);
	} else {
		DDRD |= (1 << (current_main_ch - 6));
		PORTD |= (1 << (current_main_ch - 6));
	}
	
	// стробируем
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);	
	
	// закрываем подканал 1
	if(current_main_ch < 6)
		PORTF &= ~(1 << current_main_ch);
	else
		PORTD &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// выдерживаем пауза 1с
			
	// включаем подканал 2
	putsxy2(14, LINE4, channel_2[current_main_ch]);
	if(current_main_ch < 6) {
		DDRC |= (1 << current_main_ch);
		PORTC |= (1 << current_main_ch);
	} else {
		DDRE |= (1 << (current_main_ch - 6));
		PORTE |= (1 << (current_main_ch - 6));
	}
	
	// стробируем
	DDRE |= (1 << DDRE6);
	PORTE |= (1 << PORTE6);
	_delay_ms(1000);
	PORTE &= ~(1 << PORTE6);
	
	// закрываем подканал 2
	if(current_main_ch < 6)
		PORTC &= ~(1 << current_main_ch);
	else
		PORTE &= ~(1 << (current_main_ch - 6));
	_delay_ms(1000);								// выдерживаем пауза 1с
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция стробирования одного из каналов
// возвращает номер следующего канала
uint8_t Measurement_v2(uint8_t channel)
{
	if(channel >= 0 && channel < 12) {
		// выводим номер измерения
		putsxy2(14, LINE1, number_main_ch[channel]);
		if(channel < 6) {
			DDRA |= (1 << channel);
			PORTA |= (1 << channel);
		} else {
			DDRB |= (1 << (channel - 6));
			PORTB |= (1 << (channel - 6));
		}
		
		// выводим номер термопары
		putsxy2(14, LINE2, number_main_ch[channel]);
		
		// стробируем
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);							// выдерживаем пауза заданную в настойках

		// закрываем главный канал
		if(channel < 6)
			PORTA &= ~(1 << channel);
		else
			PORTB &= ~(1 << (channel - 6));
		
		return 	(channel + 12);
	} else if(channel >= 12 && channel < 24) {
		channel = channel - 12;
		// включаем подканал 1
		putsxy2(14, LINE3, channel_1[channel]);		// выводим номер канала в напряжении1
		if(channel < 6) {
			DDRF |= (1 << channel);
			PORTF |= (1 << channel);
		} else {
			DDRD |= (1 << (channel - 6));
			PORTD |= (1 << (channel - 6));
		}
		
		// стробируем
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);						// выдерживаем пауза заданную в настойках
		
		// закрываем подканал 1
		if(channel < 6)
			PORTF &= ~(1 << channel);
		else
			PORTD &= ~(1 << (channel - 6));
		
		if(channel == 11) return (LAST_CHANNEL);
		return 	(channel + 1);
	} /*else if(channel >= 24 && channel < 36) {
		channel = channel - 24;
		// включаем подканал 2
		putsxy2(14, LINE4, channel_2[channel]);
		if(channel < 6) {
			DDRC |= (1 << channel);
			PORTC |= (1 << channel);
		} else {
			DDRE |= (1 << (channel - 6));
			PORTE |= (1 << (channel - 6));
		}
		
		// стробируем
		PORTE &= ~(1 << PORTE6);
		_delay_us(1);
		PORTE |= (1 << PORTE6);
		
		_delay_ms(Delay);						// выдерживаем пауза заданную в настойках
		
		// закрываем подканал 2
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
		// выводим номер измерения
		putsxy2(14, LINE1, number_main_ch[channel]);
		if(channel < 6) {
			DDRA |= (1 << channel);
			PORTA |= (1 << channel);
		} else {
			DDRB |= (1 << (channel - 6));
			PORTB |= (1 << (channel - 6));
		}
	
		// выводим номер термопары
		putsxy2(14, LINE2, number_main_ch[channel]);
	
		// стробируем
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);

		// закрываем главный канал
		if(channel < 6)
			PORTA &= ~(1 << channel);
		else
			PORTB &= ~(1 << (channel - 6));
		_delay_ms(Delay);							// выдерживаем пауза заданную в настойках
		
		return 	(channel + 12); 
	} else if(channel >= 12 && channel < 24) {
		channel = channel - 12;
 		// включаем подканал 1
		putsxy2(14, LINE3, channel_1[channel]);		// выводим номер канала в напряжении1
		if(channel < 6) {
			DDRF |= (1 << channel);
			PORTF |= (1 << channel);
		} else {
			DDRD |= (1 << (channel - 6));
			PORTD |= (1 << (channel - 6));
		}
	
		// стробируем
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);
	
		// закрываем подканал 1
		if(channel < 6)
			PORTF &= ~(1 << channel);
		else
			PORTD &= ~(1 << (channel - 6));
		_delay_ms(Delay);						// выдерживаем пауза заданную в настойках
		
		return 	(channel + 24); 
	} else if(channel >= 24 && channel < 36) {
		channel = channel - 24;
		// включаем подканал 2
		putsxy2(14, LINE4, channel_2[channel]);
		if(channel < 6) {
			DDRC |= (1 << channel);
			PORTC |= (1 << channel);
		} else {
			DDRE |= (1 << (channel - 6));
			PORTE |= (1 << (channel - 6));
		}
	
		// стробируем
		DDRE |= (1 << DDRE6);
		PORTE |= (1 << PORTE6);
		_delay_ms(Delay);
		PORTE &= ~(1 << PORTE6);
	
		// закрываем подканал 2
		if(channel < 6)
			PORTC &= ~(1 << channel);
		else
			PORTE &= ~(1 << (channel - 6));
		_delay_ms(Delay);						// выдерживаем пауза заданную в настойках
		
		if(channel == 11) return (LAST_CHANNEL);
		return 	(channel + 1);
	}
}*/

//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция вывода настроек на экран
static char *string4[] = {"    Уставки:", "    Set up:", "Русский", "English", "Режим отладки", "Debugging mode"};
void LoadOptionScreen(uint8_t currentLine)
{
	LCDClear2();				// Очищаем экран
	
	putsxy2(0, LINE1, string4[0 + Language]);
	putsxy2(1, LINE2, string4[2]);
	putsxy2(1, LINE3, string4[3]);
	putsxy2(1, LINE4, string4[4 + Language]);
	
	putcxy2(0, currentLine, 0x90);
}

//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция сдвига курсора в меню настроек
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
// Выводим экран "Сохранено"
static char *string5[] = {"   Сохранено    ", "      Saved     "};
void SaveScreen()
{
	LCDClear2();				// Очищаем экран
	
	putsxy2(0, LINE2, string5[0 + Language]);
	
	_delay_ms(1500);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// Выводим экран "Режим отладки"
static char *string6[] = {" Режим отладки  ", " Debugging mode ", "Канал ", "Channel "};
void DebugScreen()
{
	LCDClear2();				// Очищаем экран
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
// Выводим на экран режим отладки конкретного канала
static char *string7[] = {"включить ", "turn on  ", "выключить", "turn off ", "выход", "exit "};
void DebugChannelScreen(uint8_t channel)
{
	LCDClear2();				// Очищаем экран
	putsxy2(0, LINE1, string6[0 + Language]);
	putsxy2(0, LINE2, string6[2 + Language]);
	putsxy2(8, LINE2, number_main_ch[channel]);
	putsxy2(10, LINE2, ":");
	putsxy2(1, LINE3, string7[0 + Language]);
	putsxy2(1, LINE4, string7[4 + Language]);
	
	putcxy2(0, LINE3, 0x90);
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция сдвига курсора в режиме отладки канала
void ChangeMenuDebugChannel(uint8_t *currentNameLine)
{
	switch(*currentNameLine)
	{
		case LINE3:						// если выбрана строка меню "включить" или "выключить" 
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
// Функция старта стробирования одного из каналов
void StartMeasurementSingleChannel(uint8_t channel)
{
	putsxy2(1, LINE3, string7[2 + Language]);	// меняем пункт меню на "выключить"
	
	if(channel < 6) {
		DDRA |= (1 << channel);
		PORTA |= (1 << channel);
	} else {
		DDRB |= (1 << (channel - 6));
		PORTB |= (1 << (channel - 6));
	}
	
	// стробируем
	PORTE &= ~(1 << PORTE6);
	_delay_us(1);
	PORTE |= (1 << PORTE6);
		
	_delay_ms(Delay);							// выдерживаем пауза заданную в настойках

	// закрываем главный канал
	if(channel < 6)
		PORTA &= ~(1 << channel);
	else
		PORTB &= ~(1 << (channel - 6));
	
	// включаем подканал 1
	if(channel < 6) {
		DDRF |= (1 << channel);
		PORTF |= (1 << channel);
	} else {
		DDRD |= (1 << (channel - 6));
		PORTD |= (1 << (channel - 6));
	}

	// стробируем
	PORTE &= ~(1 << PORTE6);
	_delay_us(1);
	PORTE |= (1 << PORTE6);					
}
//----------------------------------------/ FUNCTION /----------------------------------------------
// Функция остановки стробирования одного из каналов
void StopMeasurementSingleChannel(uint8_t channel)
{
	//DDRB |= (1 << PINB1);
	//PORTB |= (1 << PORTB1);
	
	putsxy2(1, LINE3, string7[0 + Language]);	// меняем пункт меню на "включить"
	
	// закрываем подканал 1
	if(channel < 6)
		PORTF &= ~(1 << channel);
	else
		PORTD &= ~(1 << (channel - 6));
}