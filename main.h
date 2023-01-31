/*
 * main.h
 *
 * Created: 29.08.2021 13:25:06
 *  Author: oleg
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define		F_CPU		8000000UL				// Ситемное определение - частота кварца - для корректной работы
												// библиотечных функций задержки ???
#define		__DELAY_BACKWARD_COMPATIBLE__
#include	<util/delay.h>
#include	<avr/interrupt.h>
#include	<avr/sfr_defs.h>
#include	<avr/eeprom.h>

#include	"LCD.h"								// файл заголовка управления ЖКИ
//#include	"function.c"
//**************************************************************************************************
#define		VERSION					12	// для компиляции Альфа 12: 12, для Альфа 1: любое другое число

// Языки интерфейса
#define		RUSSIAN					0
#define		ENGLISH					1

// Определения типов
typedef	uint8_t	BOOL;
#define		TRUE					1
#define		FALSE					0

// Режимы работы
#define STOP						0
#define START						1
#define PAUSE						2
#define CONTINUE					3
#define LOAD_OPTIONS				4
#define OPTIONS						5
#define CHANGE_OPTIONS				6
#define LOAD_DEBUG_OPTIONS			7
#define DEBUG_OPTIONS				8
#define CHANGE_DEBUG_CHANNEL		9
#define LOAD_DEBUG_CHANNEL			10
#define DEBUG_CHANNEL				11
#define CHANGE_MENU_DEBUG_CHANNEL	12
#define EXIT_OPTIONS				13
#define START_DEBUG					14
#define STOP_DEBUG					15

#define LAST_CHANNEL				24

#define SHORT_BUTTON				0
#define LONG_BUTTON					1

#define LOW_LEVEL					0
#define HIGH_LEVEL					1

#define ON_CHANNEL					0
#define OFF_CHANNEL					1
//#define EXIT_DEBUG					2
//**************************************************************************************************


#endif /* MAIN_H_ */