/*
 * termo_project.c
 *
 * Created: 28.08.2021 16:18:30
 * Author : Oleg Ivanov
 * v1.8
 */ 

#include <avr/io.h>
#include "main.h"

//--------------------------------------------------------------------------------------------------
//	Определение переменных
//
//--------------------------------------------------------------------------------------------------
uint8_t State = STOP;
uint8_t Channel = 0;
//uint8_t CurrentLineOptions = LINE3;
uint8_t CurrentLineOptions;
float Delay = 1000.;
uint8_t Language = RUSSIAN;
BOOL timer_in_options = FALSE;
uint8_t DebugNumberChannel = 0;
uint8_t CurrentLineDebugChannelMenu = LINE3;
uint8_t CurrentNameLine3DebugChannelMenu = ON_CHANNEL;
//---------------------------------------/ MAIN /---------------------------------------------------
int main(void)
{	
	Start_Init();
	
	Language = eeprom_read_byte(20);
	if(Language == 0xFF)
	{
		//DDRB |= (1 << PINB1);
		//PORTB |= (1 << PORTB1);
		Language =  RUSSIAN;	// значение по-умолчанию
		eeprom_write_byte(20, Language);
	}
	
	// определение строки указателя пункта в меню уставок
	switch(Language)
	{
		case RUSSIAN:
			CurrentLineOptions = LINE2;
			break;
		case ENGLISH:
			CurrentLineOptions = LINE3;
			break;
	}		
	
	MainScreen();		// выводим название прибора
	_delay_ms(3000);	// выдерживаем паузу 3с
	
	StandByScreen();	// выводим надпись ожидания измерения	

    while (1)
    {
		/*if(~PINE & (1 << PINE7)) {
			DDRB |= (1 << PINB1);		// работало
			PORTB |= (1 << PORTB1);
		}*/
		switch(State)
		{
			// загружаем экран с уставками
			case LOAD_OPTIONS:
				LoadOptionScreen(CurrentLineOptions);
				State = OPTIONS;
				break;
			case CHANGE_OPTIONS:			
				CurrentLineOptions = ChangeOptions(CurrentLineOptions);
				State = OPTIONS;
				break;
			case EXIT_OPTIONS:								
				if(Channel == 0 || Channel == LAST_CHANNEL) {
					StandByScreen();
					State = STOP;
				} else {
					State = START;	
				}
				break;
			case OPTIONS:
				if(timer_in_options == TRUE && TCNT1 > 7813) {
					if(CurrentLineOptions == LINE4) {
						Language = eeprom_read_byte(20);	// не изменяем язык, используем тот, который записан в памяти 
					} else {
						SaveScreen();
						eeprom_update_byte(20, Language);						
					}
					//DDRB |= (1 << PINB1);
					//PORTB |= (1 << PORTB1);
				}
				break;
			case LOAD_DEBUG_OPTIONS:
				DebugScreen();
				State = DEBUG_OPTIONS;
				break;
			case CHANGE_DEBUG_CHANNEL:
				ChangeDebugChannelOnSCreen(&DebugNumberChannel);
				State = DEBUG_OPTIONS;
				break;
			case LOAD_DEBUG_CHANNEL:
				DebugChannelScreen(DebugNumberChannel);
				State = DEBUG_CHANNEL;
				break;
			case CHANGE_MENU_DEBUG_CHANNEL:
				ChangeMenuDebugChannel(&CurrentLineDebugChannelMenu);
				State = DEBUG_CHANNEL;
				break;
			case START_DEBUG:
				StartMeasurementSingleChannel(DebugNumberChannel);
				State = DEBUG_CHANNEL;
				break;
			case STOP_DEBUG:
				StopMeasurementSingleChannel(DebugNumberChannel);
				State = DEBUG_CHANNEL;			
				break;
		}
		
		if(State == START || State == CONTINUE) {
			PORTG |= (1 << PORTG4);		// включаем лампочку "Работа"
			
			if(State == START) Start();
#if VERSION == 12
			while(Channel != LAST_CHANNEL) {
				Channel = Measurement_v2(Channel);
				if(State == PAUSE || State == LOAD_OPTIONS) break;
			}
#else
			while(Channel != 1) {
				Channel = Measurement_v2(Channel);
				if(State == PAUSE || State == LOAD_OPTIONS) break;
			}			
#endif
			// закончили измерение
			PORTG &= ~(1 << PORTG4);	// выключаем лампочку "Работа"
			
			if(State == START || State == CONTINUE) {
				Finished();
				StandByScreen();	// выводим надпись ожидания измерения
				State = STOP;
				Channel = 0;				
			}
		}
    }
}

// INT7_vect - внешнее прерывание 7
uint8_t button_level = HIGH_LEVEL;
ISR(INT7_vect)
{
	if(button_level == HIGH_LEVEL) {
		button_level = LOW_LEVEL;				// кнопка нажата
		
		EIMSK &= ~(1 << INT7);					// выключаем прерывание
		EICRB |= (1 << ISC71) | (1 << ISC70);	// выставляем прерывание по нарастающему фронту ISC71 = 1 и ISC70 = 1
		EIMSK |= (1 << INT7);					// включаем прерывание
		
		//DDRB |= (1 << PINB1);
		//PORTB &= ~(1 << PORTB1);
		
		// выставляем флаг запуска таймера/счётчик 1 в окне опций
		if(State == OPTIONS)
			timer_in_options = TRUE;
		
		TCNT1 = 0;								// включаем таймер-счётчик 1
		_delay_ms(30);
		return;
	}
	
	if(button_level == LOW_LEVEL) {
		button_level = HIGH_LEVEL;				// кнопка отжата
		
		//DDRB |= (1 << PINB1);
		//PORTB |= (1 << PORTB1);
		
		EIMSK &= ~(1 << INT7);					// выключаем прерывание
		EICRB |= (1 << ISC71);					// выставляем прерывание по падающему фронту ISC71 = 1 и ISC70 = 0
		EICRB &= ~(1 << ISC70);					//
		EIMSK |= (1 << INT7);					// включаем прерывание
				
		switch(State){
			case STOP:
				State = START;
				break;
			case START:
				State = PAUSE;
				break;
			case PAUSE:
				State = CONTINUE;
				break;
			case CONTINUE:
				State = PAUSE;
				break;
			case OPTIONS:
				State = CHANGE_OPTIONS;
				timer_in_options = FALSE;
				break;
			case DEBUG_OPTIONS:
				State = CHANGE_DEBUG_CHANNEL;
				break;
			case DEBUG_CHANNEL:
				State = CHANGE_MENU_DEBUG_CHANNEL;
				break;
		}
		
		if(TCNT1 > 7813) {									// 7813 - это 1с
			switch(State) {
				case CHANGE_OPTIONS:
					if(CurrentLineOptions == LINE4) {		// если долгое нажатие кнопки и в опциях выбран "Режим отладки"
						State = LOAD_DEBUG_OPTIONS;
					} else {
						State = EXIT_OPTIONS;
					}				
					break;
				case CHANGE_DEBUG_CHANNEL:
					State = LOAD_DEBUG_CHANNEL;
					break;
				case CHANGE_MENU_DEBUG_CHANNEL:				// если долгое нажатие кнопки в режиме отладки канала
					switch(CurrentLineDebugChannelMenu) {
						case LINE3:						
							if(CurrentNameLine3DebugChannelMenu == ON_CHANNEL) {
								CurrentNameLine3DebugChannelMenu = OFF_CHANNEL;
								State = START_DEBUG;
							} else {
								CurrentNameLine3DebugChannelMenu = ON_CHANNEL;
								State = STOP_DEBUG;
							}
							break;
						case LINE4:
							State = EXIT_OPTIONS;
							CurrentLineDebugChannelMenu = LINE3;	// присваиваем первоначальное состояние
							if(CurrentNameLine3DebugChannelMenu == OFF_CHANNEL) {	// если вдруг был выполнен выход с включённым отладочным
								if(DebugNumberChannel < 6)							// закрываем подканал 1
									PORTF &= ~(1 << DebugNumberChannel);
								else
									PORTD &= ~(1 << (DebugNumberChannel - 6));
								CurrentNameLine3DebugChannelMenu = ON_CHANNEL;
							}
							DebugNumberChannel = 0;
							break;
					}
					break;
				default:
					State = LOAD_OPTIONS;
			}
		}
		
		_delay_ms(30);
	}
}
