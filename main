#include "main.h"

// MAIN PROGRAM //
int main() {
    init_uart();
    init_program();
    init_buttons();
    configure_pwm(LED_0, 0);
    int state = 1;

    printf("> Press SW_O to calibrate\n\n");

    while (true) {
        switch (state) {
            case 1:
                while (led_on) { // led_on = true
                    if (calib_button_triggered) {
                        led_on = false;
                        calibrate_motor();
                    } else {
                        led_blink();
                    }
                }
                state = 2;
                break;
            case 2:
                light_brightness(1000);
                spin_the_wheel();
                state = 1;
                break;
        }
    }
}



// FUNCTIONS

// function for handling interrupt for LED and button press
void interrupt_handler(uint gpio, uint32_t event_mask) {
    if (gpio == CALIB_BUTTON) {
        calib_button_triggered = true;
    } else if (gpio == OPTICAL_FORK) {
        opt_fork_triggered = true;
    } else if (gpio == PIEZO_SENSOR && !piezo_triggered) {
        piezo_triggered = true;
    }
}

// initialize coils and fork
void init_program(void) {
    stdio_init_all();

    gpio_init(COIL_A1);
    gpio_set_dir(COIL_A1, GPIO_OUT);

    gpio_init(COIL_A2);
    gpio_set_dir(COIL_A2, GPIO_OUT);

    gpio_init(COIL_B1);
    gpio_set_dir(COIL_B1, GPIO_OUT);

    gpio_init(COIL_B2);
    gpio_set_dir(COIL_B2, GPIO_OUT);

    gpio_init(OPTICAL_FORK);
    gpio_set_dir(OPTICAL_FORK, GPIO_IN);
    gpio_pull_up(OPTICAL_FORK);
    gpio_set_irq_enabled_with_callback(OPTICAL_FORK, GPIO_IRQ_EDGE_FALL, true, interrupt_handler);

    gpio_init(PIEZO_SENSOR);
    gpio_set_dir(PIEZO_SENSOR, GPIO_IN);
    gpio_pull_up(PIEZO_SENSOR);
    gpio_set_irq_enabled_with_callback(PIEZO_SENSOR, GPIO_IRQ_EDGE_FALL, true, interrupt_handler);

    gpio_set_irq_enabled_with_callback(CALIB_BUTTON, GPIO_IRQ_EDGE_FALL, true, interrupt_handler);

}

// initialize uart
void init_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

// initialize buttons
void init_buttons() {
    gpio_init(CALIB_BUTTON);
    gpio_set_dir(CALIB_BUTTON, GPIO_IN);
    gpio_pull_up(CALIB_BUTTON);

    gpio_init(RUN_BUTTON);
    gpio_set_dir(RUN_BUTTON, GPIO_IN);
    gpio_pull_up(RUN_BUTTON);
}


// run stepper motor
void run_motor(int step) {
    for (int i = 0; i < step; i++) {
        gpio_put_all(sequence[current_step]);
        if (current_step == 7) {
            current_step = 0;
        } else {
            current_step++;
        }
        uint32_t start_time = time_us_32();
        while (time_us_32() - start_time < 1000) {
            tight_loop_contents();
        }
    }
}

// calibrate the motor
void calibrate_motor() {
    steps_per_round = 0; // steps per full round
    int count = 0; // steps for any round, including calibration round
    printf("CALIBRATING...\n");

    opt_fork_triggered = false;
    for (int i = 0; i < 2; i++) {
        while (!opt_fork_triggered) { // when optical fork is not triggered
            run_motor(1);
            if (i ==
                0) { // if first round (aka calibration round, which might not be a full round) do not calculate steps_per_round, count only steps for display
                count++;
            } else { // after the first round count the steps for a sure full round
                steps_per_round++;
                count++;
            }
        }
        printf("ROUND %d: %d steps\n", i + 1, count);
        opt_fork_triggered = false; // must be false for a new turn
        count = 0; // must be emptied so every print has only the steps for that round
    }

    run_motor(145); // final aligning steps
    printf("AVG STEPS PER ROUND: %d (+ final aligning steps: 145)\n\n"
           "> Press the rotary button to dispense\n", steps_per_round);
}

// run N amount of slots
void run_slots(int slot) {
    int steps = 0;

    if (slot == 0) {
        steps = steps_per_round;
    } else {
        steps = (slot * steps_per_round) / 8;
    }
    run_motor(steps);
}

// pwm configurations for led
void configure_pwm(int led_pin, uint16_t light_brightness) {

    uint led_pwm_slice = pwm_gpio_to_slice_num(led_pin);
    uint led_pwm_channel = pwm_gpio_to_channel(led_pin);

    pwm_set_enabled(led_pwm_slice, false);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&config, 125);
    pwm_config_set_wrap(&config, 999);
    pwm_init(led_pwm_slice, &config, false);
    pwm_set_chan_level(led_pwm_slice, led_pwm_channel, 0);
    gpio_set_function(led_pin, GPIO_FUNC_PWM);
    pwm_set_enabled(led_pwm_slice, true);
}

// control the brightness of led
void light_brightness(int brightness) {
    uint led_pwm_slice = pwm_gpio_to_slice_num(LED_0);
    uint led_pwm_channel = pwm_gpio_to_channel(LED_0);

    pwm_set_chan_level(led_pwm_slice, led_pwm_channel, brightness);
}

// function to blink the LED
void led_blink() {
    light_brightness(1000);
    sleep_ms(750);
    light_brightness(0);
    sleep_ms(750);
}

// stabilize button press
bool pressed(int button) {
    int press = 0;
    int release = 0;
    while (press < 3 && release < 3) {
        if (!gpio_get(button)) {
            press++;
            release = 0;
        } else {
            release++;
            press = 0;
        }
        sleep_ms(10);
    }
    if (press > release) return true;
    else return false;
}

// mark down the start time for timer for later use
void start_timer(struct timeval *start_time) {
    gettimeofday(start_time, NULL);
}

// calculates time using the current time minus start time
double get_elapsed_time(struct timeval *start_time) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    return (current_time.tv_sec - start_time->tv_sec) +
           // takes into account the time the program spends in this function by calculating the current time - the time the function was called
           (current_time.tv_usec - start_time->tv_usec) / 1e6; // plus
}

// function for if no pill detected
int piezo_blink(int current_turn) {
    if (!piezo_triggered && current_turn != 0) { // piezo_triggered is only false if the pill didn't drop after the run_slots, if it drops it changes to true and skips this
        printf(">> NO PILL DETECTED FOR DAY %d\n", current_turn);

        for (int blink_count = 0; blink_count < 5; blink_count++) {
            led_blink();
        }
        light_brightness(0);
    } else {
        printf(">> PILL DISPENSED\n");
    }
    return current_turn;
}


// function for spinning the wheel
void spin_the_wheel() {
    struct timeval start_time;
    double elapsed_time;
    int max_turns = 7;
    int current_turn = 0;

    if (pressed(RUN_BUTTON)) {
        printf("\nDISPENSING STARTED...\n");
        light_brightness(0);

        piezo_triggered = false; // must be turned into false to be able to detect if the pill fell
        start_timer(&start_time);
        run_slots(1); // after this the piezo either triggers or not

        current_turn++;
        printf("\n> Day %d\n", current_turn);
        sleep_ms(40);// waits for the pill to drop, originally should be around 27ms, but added 13ms error margin so its 40ms

        piezo_blink(current_turn);

        while (current_turn < max_turns) {
            elapsed_time = get_elapsed_time(&start_time); // finds what time it is

            if (elapsed_time >= TIMER_INTERVAL_SECONDS) { // only executes if 30 secs have passed
                piezo_triggered = false; // must be turned into false to be able to detect if the pill fell
                start_timer(&start_time);
                run_slots(1); // after this the piezo either triggers or not

                current_turn++;
                printf("\n> Day %d\n", current_turn);
                sleep_ms(100); // waits for the pill to drop, originally should be around 27ms, but added 13ms error margin so its 40ms

                piezo_blink(current_turn);
            }
        }

        calib_button_triggered = false; // changes true to false after finishing
        led_on = true; // starts blinking the LED light while waiting for calibration
        printf("\nWaiting for calibration...\n");
    }
}
