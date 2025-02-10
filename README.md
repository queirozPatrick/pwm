# Controle de Servomotor por PWM

Este projeto foi desenvolvido como parte do curso EmbarcaTech, promovido pelo CEPEDI, SOFTEX e HwIT - Hardware Innovation Technologies, para consolidar conhecimentos sobre modulação por largura de pulso (PWM) no microcontrolador RP2040, utilizando a placa de desenvolvimento BitDogLab e o simulador Wokwi.

## 📝 Enunciado do Projeto
O objetivo desta atividade é projetar um sistema utilizando o módulo PWM do microcontrolador RP2040 para controlar o ângulo de um servomotor padrão. A simulação é realizada no ambiente Wokwi, sem necessidade de circuitos externos. Além disso, um LED RGB é utilizado para representar visualmente o movimento do servomotor.

## 🔧 Componentes Utilizados
- Raspberry Pi Pico W (simulado no Wokwi)
- Servomotor padrão (simulado no Wokwi)
- LED RGB (conectado à GPIO 12)
- Pico SDK para desenvolvimento em C

## 📌 Requisitos Implementados
1. **Configuração do PWM**
   - A frequência do PWM foi ajustada para aproximadamente **50Hz** (período de 20ms) na GPIO 22. ✅
   
2. **Posicionamento do Servo em 180°**
   - Definição do ciclo ativo do PWM para **2400µs** (duty cycle de 0,12%).
   - Aguardo de **5 segundos** nesta posição. ✅
   
3. **Posicionamento do Servo em 90°**
   - Definição do ciclo ativo do PWM para **1470µs** (duty cycle de 0,0735%).
   - Aguardo de **5 segundos** nesta posição. ✅
   
4. **Posicionamento do Servo em 0°**
   - Definição do ciclo ativo do PWM para **500µs** (duty cycle de 0,025%).
   - Aguardo de **5 segundos** nesta posição. ✅
   
5. **Movimentação periódica do servo de 0° a 180°**
   - Movimentação suave com incremento/decremento de **±5µs**.
   - Atraso de **10ms** entre cada ajuste para suavidade no movimento. ✅
   
6. **Experimento com LED RGB (GPIO 12)**
   - O brilho do LED azul varia proporcionalmente ao ângulo do servo.
   - Quando o servo está em **0°**, o LED está **apagado**.
   - Quando o servo está em **90°**, o LED está **meio brilho**.
   - Quando o servo está em **180°**, o LED está **no brilho máximo**. ✅

## 🎥 Demonstração do Projeto
- Simulação no Wokwi: [Clique aqui](https://wokwi.com/projects/422362142211338241)
- Vídeo do projeto: (A ser incluído)_

## 🛠️ Configuração e Execução do Código
### Requisitos
- Ambiente de desenvolvimento com **Pico SDK** configurado.
- Compilador **ARM GCC**.
- Ferramenta **BitDogLab** para testes práticos.
- Simulador **Wokwi** para validação.

### Compilação e Execução
```sh
mkdir build
cd build
cmake ..
make
./programa_pico
```

## 🚀 Conclusão
Este projeto permitiu explorar o controle de servomotores utilizando PWM, aplicando conceitos fundamentais de tempo real e mapeamento de sinais PWM para componentes físicos. Além disso, a relação entre o ângulo do servo e a intensidade do LED RGB reforçou a compreensão sobre conversão e escalonamento de sinais analógicos e digitais.

---
📌 **Autor:** Patrick
📌 **Curso:** EmbarcaTech - CEPEDI, SOFTEX, HwIT

