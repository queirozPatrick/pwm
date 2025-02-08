#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include <stdio.h> // Para usar printf

#define PINO_SERVO 22
#define LED_RED 12
#define PWM_CICLO 24999  // 20ms (50Hz) para servos
#define DIVISOR_CLK 100  // Divisor de clock para obter o período correto

// Função para converter microssegundos em nível PWM
uint32_t microssegundos_para_nivel(uint32_t us) {
    return (uint32_t)(us * 1.25f); // 1µs ≈ 1.25 unidades
}

// Função para mapear o ângulo do servo para o brilho do LED (0 a 255)
uint8_t mapeia_brilho_led(int angulo) {
    return (uint8_t)(angulo * 255 / 180); // Mapeia 0-180 para 0-255
}

// Função de interrupção de wrap (não será usada neste projeto)
void trataInterrupcao() {
    static int angulo = 0;
    static bool flag = true;
    pwm_clear_irq(pwm_gpio_to_slice_num(PINO_SERVO));

    if (flag) {
        angulo++;
        if (angulo > 180) {  
            angulo = 180;
            flag = false;
        }
    } else {
        angulo--;
        if (angulo < 0) {
            angulo = 0;
            flag = true;
        }
    }

    // Mapeia o ângulo para o ciclo de trabalho (1ms a 2ms)
    uint16_t largura_pulso = 1000 + (angulo * 1000 / 180);  // 1000us a 2000us
    pwm_set_gpio_level(PINO_SERVO, largura_pulso);

    // Atualiza o brilho do LED
    uint8_t brilho = mapeia_brilho_led(angulo);
    pwm_set_gpio_level(LED_RED, brilho * PWM_CICLO / 255);
}

// Função para inicializar o PWM do servo e do LED
void inicializa() {
    // Configura o PWM do servo
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM);
    uint slice_num_servo = pwm_gpio_to_slice_num(PINO_SERVO);
    pwm_config config_servo = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_servo, DIVISOR_CLK);
    pwm_config_set_wrap(&config_servo, PWM_CICLO);
    pwm_init(slice_num_servo, &config_servo, true);

    // Configura o PWM do LED
    gpio_set_function(LED_RED, GPIO_FUNC_PWM);
    uint slice_num_led = pwm_gpio_to_slice_num(LED_RED);
    pwm_config config_led = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_led, DIVISOR_CLK);
    pwm_config_set_wrap(&config_led, PWM_CICLO);
    pwm_init(slice_num_led, &config_led, true);

    printf("PWM inicializado para servo e LED.\n");
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial para depuração
    uint slice_num_servo = pwm_gpio_to_slice_num(PINO_SERVO);

    // Inicializa o PWM do servo e do LED
    inicializa();

    // Desabilita a interrupção para controlar o servo manualmente
    pwm_set_irq_enabled(slice_num_servo, false);

    // Move o servo para 2400µs (2.4ms) - 180 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(2400));
    pwm_set_gpio_level(LED_RED, 255 * PWM_CICLO / 255); // LED no brilho máximo
    printf("Servo em 180°, LED no brilho máximo.\n");
    sleep_ms(5000);

    // Move o servo para 1470µs (1.47ms) - 90 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(1470));
    pwm_set_gpio_level(LED_RED, 127 * PWM_CICLO / 255); // LED na metade do brilho
    printf("Servo em 90°, LED na metade do brilho.\n");
    sleep_ms(5000);

    // Move o servo para 500µs (0.5ms) - 0 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(500));
    pwm_set_gpio_level(LED_RED, 0); // LED apagado
    printf("Servo em 0°, LED apagado.\n");
    sleep_ms(5000);

    // Transição suave de 0° a 180° com incremento de ±5µs e atraso de 10ms
    uint32_t nivel_inicial = microssegundos_para_nivel(500);  // 625
    uint32_t nivel_final = microssegundos_para_nivel(2400);   // 3000

    while (1) {
        // Movimento de 0° a 180°
        for (uint32_t i = nivel_inicial; i <= nivel_final; i += 5) {
            pwm_set_gpio_level(PINO_SERVO, i);
            uint8_t brilho = mapeia_brilho_led((i - nivel_inicial) * 180 / (nivel_final - nivel_inicial));
            pwm_set_gpio_level(LED_RED, brilho * PWM_CICLO / 255);
            sleep_ms(10);  // Atraso de 10ms para suavidade
        }

        // Movimento de 180° a 0°
        for (uint32_t i = nivel_final; i >= nivel_inicial; i -= 5) {
            pwm_set_gpio_level(PINO_SERVO, i);
            uint8_t brilho = mapeia_brilho_led((i - nivel_inicial) * 180 / (nivel_final - nivel_inicial));
            pwm_set_gpio_level(LED_RED, brilho * PWM_CICLO / 255);
            sleep_ms(10);  // Atraso de 10ms para suavidade
        }
    }

    return 0;
}