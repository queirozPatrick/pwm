#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22       // GPIO 22 para controle do servomotor
#define LED_R_PIN 12       // GPIO 12 para o LED vermelho
#define PWM_FREQ 50        // Frequência de 50Hz (período de 20ms)

// Função para definir o ciclo ativo do PWM em microssegundos
void set_servo_position(uint slice, uint channel, uint pulse_width_us) {
    // Calcula o valor de wrap com base na frequência de PWM
    uint32_t wrap = 125000000 / PWM_FREQ; // 125MHz / 50Hz = 2.500.000
    pwm_set_wrap(slice, wrap);

    // Converte o tempo de pulso em microssegundos para o valor de contagem do PWM
    uint32_t level = (pulse_width_us * wrap) / 20000; // 20000us = 20ms
    pwm_set_chan_level(slice, channel, level);
}

// Função para ajustar a intensidade do LED vermelho
void set_led_red(uint8_t red) {
    // Ajusta o ciclo de trabalho do LED vermelho
    pwm_set_gpio_level(LED_R_PIN, red * 100); // Vermelho
}

int main() {
    stdio_init_all(); // Inicializa o sistema padrão de I/O

    // Configura o pino do servomotor como PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint servo_slice = pwm_gpio_to_slice_num(SERVO_PIN);
    uint servo_channel = pwm_gpio_to_channel(SERVO_PIN);

    // Configura o pino do LED vermelho como PWM
    gpio_set_function(LED_R_PIN, GPIO_FUNC_PWM);
    uint slice_r = pwm_gpio_to_slice_num(LED_R_PIN);

    // Configura o divisor de clock para obter a frequência desejada
    pwm_set_clkdiv(servo_slice, 100.0); // Divisor de clock para 1.25MHz (125MHz / 100)
    pwm_set_clkdiv(slice_r, 100.0);

    // Habilita o PWM nos slices correspondentes
    pwm_set_enabled(servo_slice, true);
    pwm_set_enabled(slice_r, true);

    // Define o ciclo ativo para 2400µs (180 graus)
    set_servo_position(servo_slice, servo_channel, 2400);
    set_led_red(255); // LED vermelho no máximo (180 graus)
    sleep_ms(5000); // Aguarda 5 segundos

    // Define o ciclo ativo para 1470µs (90 graus)
    set_servo_position(servo_slice, servo_channel, 1470);
    set_led_red(127); // LED vermelho na metade da intensidade (90 graus)
    sleep_ms(5000); // Aguarda 5 segundos

    // Define o ciclo ativo para 500µs (0 graus)
    set_servo_position(servo_slice, servo_channel, 500);
    set_led_red(0); // LED vermelho desligado (0 graus)
    sleep_ms(5000); // Aguarda 5 segundos

    // Movimentação periódica do braço do servomotor entre 0 e 180 graus
    while (true) {
        // Movimentação de 0 a 180 graus
        for (uint pulse_width_us = 500; pulse_width_us <= 2400; pulse_width_us += 5) {
            set_servo_position(servo_slice, servo_channel, pulse_width_us);

            // Ajusta a intensidade do LED vermelho conforme o ângulo do servomotor
            uint8_t red = (pulse_width_us - 500) * 255 / 1900; // Mapeia 500-2400µs para 0-255
            set_led_red(red);

            sleep_ms(10); // Atraso de 10ms para movimentação suave
        }

        // Movimentação de 180 a 0 graus
        for (uint pulse_width_us = 2400; pulse_width_us >= 500; pulse_width_us -= 5) {
            set_servo_position(servo_slice, servo_channel, pulse_width_us);

            // Ajusta a intensidade do LED vermelho conforme o ângulo do servomotor
            uint8_t red = (pulse_width_us - 500) * 255 / 1900; // Mapeia 500-2400µs para 0-255
            set_led_red(red);

            sleep_ms(10); // Atraso de 10ms para movimentação suave
        }
    }

    return 0;
}