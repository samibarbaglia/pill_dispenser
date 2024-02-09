#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// define the stepper motor and fork (basically the wheel)
#define OPTICAL_FORK 28
#define COIL_A1 2
#define COIL_A2 3
#define COIL_B1 6
#define COIL_B2 13

// define else
#define BUFFER_MAX 10
#define CALIB_BUTTON 9 //sw_0
#define RUN_BUTTON 12 //rotary_switch
#define LED_0 22
#define PIEZO_SENSOR 27
#define TIMER_INTERVAL_SECONDS 30

// globals :(
const int sequence[] = {4, 12, 8, 72, 64, 8256, 8192, 2196};
bool opt_fork_triggered = false;
bool calib_button_triggered = false;
bool piezo_triggered = false;
static int current_step = 0;
static int steps_per_round = 0;
bool led_on = true;

// UART configuration
#define UART_ID uart0
#define BAUD_RATE 9600
#if 0
#define UART_NR 0
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#else
#define UART_NR 1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
#endif


// FUNCTIONS
void interrupt_handler(uint gpio, uint32_t event_mask);

// initialize coils and fork
void init_program(void);

void init_uart();

void init_buttons();

// run stepper motor
void run_motor(int step);
// calibrate the motor
void calibrate_motor();

// run N amount of slots
void run_slots(int slot);

// pwm configurations for led
void configure_pwm(int led_pin, uint16_t light_brightness);

// control the brightness of led
void light_brightness(int brightness);

// function to blink the LED
void led_blink();

// stabilize button press
bool pressed(int button);

// mark down the start time for timer for later use
void start_timer(struct timeval *start_time);

// calculates time using the current time minus start time
double get_elapsed_time(struct timeval *start_time);

// function for if no pill detected
int piezo_blink(int current_turn);

// function for spinning the wheel
void spin_the_wheel();

#endif

