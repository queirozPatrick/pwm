#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"

#define PINO_SERVO 22
#define PWM_CICLO 24999  // 20ms (50Hz) para servos
#define DIVISOR_CLK 100  // Divisor de clock para obter o período correto

// Função para converter microssegundos em nível PWM
uint32_t microssegundos_para_nivel(uint32_t us) {
    return (uint32_t)(us * 1.25f); // 1µs ≈ 1.25 unidades
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
}

// Função para inicializar o PWM
void inicializa() {
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, DIVISOR_CLK); // Divisor de clock
    pwm_config_set_wrap(&config, PWM_CICLO);      // Wrap
    pwm_init(slice_num, &config, true);
}

int main() {
    uint slice_num = pwm_gpio_to_slice_num(PINO_SERVO);

    // Inicializa o PWM
    inicializa();

    // Desabilita a interrupção para controlar o servo manualmente
    pwm_set_irq_enabled(slice_num, false);

    // Move o servo para 2400µs (2.4ms) - 180 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(2400));
    sleep_ms(5000);

    // Move o servo para 1470µs (1.47ms) - 90 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(1470));
    sleep_ms(5000);

    // Move o servo para 500µs (0.5ms) - 0 graus
    pwm_set_gpio_level(PINO_SERVO, microssegundos_para_nivel(500));
    sleep_ms(5000);

    // Transição suave de 0° a 180° com incremento de ±5µs e atraso de 10ms
    uint32_t nivel_inicial = microssegundos_para_nivel(500);  // 625
    uint32_t nivel_final = microssegundos_para_nivel(2400);   // 3000

    while (1) {
        // Movimento de 0° a 180°
        for (uint32_t i = nivel_inicial; i <= nivel_final; i += 5) {
            pwm_set_gpio_level(PINO_SERVO, i);
            sleep_ms(10);  // Atraso de 10ms para suavidade
        }

        // Movimento de 180° a 0°
        for (uint32_t i = nivel_final; i >= nivel_inicial; i -= 5) {
            pwm_set_gpio_level(PINO_SERVO, i);
            sleep_ms(10);  // Atraso de 10ms para suavidade
        }
    }

    return 0;
}