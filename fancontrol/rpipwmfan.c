/* fan/rpipwmfan.c
 * Raspberry Pi PWM fan control and monitoring.
 * This program reads the CPU temperature, adjusts the PWM duty cycle accordingly,
 * and provides a TCP server to report the current temperature and duty cycle.
 */
#include "rpipwmfan.h"

timer_t timerid;

float current_temp = 0.0;
unsigned int current_duty = 0;

unsigned int temp_to_dutycycle(float temp) {
    if (temp < 40.0)
        return 200000;
    else if (temp > 80.0)
        return 1000000;
    else
        return (unsigned int)((temp - 40.0) * (1000000 - 200000) / (80.0 - 40.0) + 200000);
}

float read_cpu_temp() {
    FILE *fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (!fp) return -1;
    int temp_milli;
    fscanf(fp, "%d", &temp_milli);
    fclose(fp);
    return temp_milli / 1000.0;
}

void timer_handler(int signum) {
    float temp = read_cpu_temp();
    current_temp = temp;
    if (temp < 0) {
        fprintf(stderr, "Failed to read CPU temperature\n");
        return;
    }
    unsigned int duty = temp_to_dutycycle(temp);
    current_duty = duty;
    gpioHardwarePWM(PWM_GPIO, PWM_FREQ, duty);

    fprintf(stdout, "CPU Temp: %.2f C, PWM Duty: %u%%\n", temp, duty/10000);
    fflush(stdout);
}

void* tcp_server_thread(void* arg) {
    int server, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char send_buffer[BUFFER_SIZE];
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket failed");
        return NULL;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server);
        return NULL;
    }

    listen(server, 3);
    printf("Server listening on port %d\n", PORT);

    while (1) {
        new_socket = accept(server, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        snprintf(send_buffer, sizeof(send_buffer),
            "%.2f:%u\n",
            current_temp, current_duty / 10000);
        send(new_socket, send_buffer, strlen(send_buffer), 0);
        close(new_socket);
    }

    return NULL;
}

int main() {
    if (gpioInitialise() < 0) {
        fprintf(stderr, "pigpio init failed\n");
        return 1;
    }

    // Setup signal handler for timer
    struct sigaction sa;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        gpioTerminate();
        return 1;
    }

    // Create the timer
    struct sigevent sev;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1) {
        perror("timer_create");
        gpioTerminate();
        return 1;
    }

    // Start timer: first expiration after 1 sec, repeat every 5 sec
    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 5;
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        gpioTerminate();
        return 1;
    }
    pthread_t server_thread;
    if(pthread_create(&server_thread, NULL, tcp_server_thread, NULL) != 0) {
        perror("Failed to create TCP server thread");
        gpioTerminate();
        return 1;
    }
    // Keep the program running to receive timer signals
    while (1) {
        pause();  
    }

    // Cleanup (never reached here)
    gpioHardwarePWM(PWM_GPIO, 0, 0);
    gpioTerminate();

    return 0;
}
