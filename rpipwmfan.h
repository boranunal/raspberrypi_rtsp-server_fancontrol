/* fan/rpipwmfan.h
 * Header file for Raspberry Pi PWM fan control and monitoring.
 * This file defines functions and constants used in the fan control system.
 * It includes necessary libraries for GPIO control, signal handling, and TCP server functionality.
 */ 

#ifndef RPIPWMFAN_H
#define RPIPWMFAN_H

#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pigpio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

#define PWM_GPIO 12
#define PWM_FREQ 25000
#define PORT 3000
#define BUFFER_SIZE 1024

unsigned int temp_to_dutycycle(float temp);
float read_cpu_temp();
void timer_handler(int signum);
void* tcp_server_thread(void* arg);

#endif // RPIPWMFAN_H