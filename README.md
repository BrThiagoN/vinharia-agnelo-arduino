# 🍷 Vinheria Agnello — Sistema de Monitoramento Inteligente

> **Edge Computing & Computer Systems — Checkpoint 02**
> Curso: Engenharia de Software | FIAP
> Turma: **1ESPV**

-----

## 👥 Integrantes do Grupo

|Nome       |RM |
|-----------|---|
|Thiago Gomes|RM:569436|
|Gabriel Henrique|RM:572636|
|Matheus de Amorim|RM:572435|
|Eduardo Frois|RM:574103|

-----

## 📋 Descrição do Projeto

A **Vinheria Agnello** é uma loja tradicional que busca modernizar sua operação com um sistema de monitoramento ambiental para garantir a qualidade dos seus vinhos.

Este projeto implementa um sistema embarcado com **Arduino** capaz de monitorar em tempo real a **luminosidade**, **temperatura** e **umidade** do ambiente, exibindo os dados em um **display LCD** e acionando alertas visuais e sonoros conforme as condições do ambiente.

O sistema foi desenvolvido em duas etapas:

- **CP1:** Monitoramento de luminosidade com LEDs e Buzzer
- **CP2 (atual):** Adição do sensor DHT11 para temperatura e umidade, com exibição no display LCD

-----

## ⚙️ Funcionalidades

### 💡 Luminosidade (LDR)

|Condição       |LED             |Buzzer              |Display               |
|---------------|----------------|--------------------|----------------------|
|Ambiente escuro|🟢 Verde aceso   |—                   |—                     |
|Meia luz       |🟡 Amarelo aceso |—                   |“Ambiente a meia luz” |
|Muito claro    |🔴 Vermelho aceso|Ligado continuamente|“Ambiente muito CLARO”|

### 🌡️ Temperatura (DHT11)

|Condição         |LED            |Buzzer              |Display                        |
|-----------------|---------------|--------------------|-------------------------------|
|Entre 10°C e 15°C|—              |—                   |“Temperatura OK / Temp. = X.XC”|
|Acima de 15°C    |🟡 Amarelo aceso|Ligado continuamente|“Temp. ALTA / Temp. = X.XC”    |
|Abaixo de 10°C   |🟡 Amarelo aceso|Ligado continuamente|“Temp. BAIXA / Temp. = X.XC”   |

### 💧 Umidade (DHT11)

|Condição       |LED             |Buzzer              |Display                       |
|---------------|----------------|--------------------|------------------------------|
|Entre 50% e 70%|—               |—                   |“Umidade OK / Umidade = X%”   |
|Acima de 70%   |🔴 Vermelho aceso|Ligado continuamente|“Umidade ALTA / Umidade = X%” |
|Abaixo de 50%  |🔴 Vermelho aceso|Ligado continuamente|“Umidade BAIXA / Umidade = X%”|


> 📊 Os valores exibidos são a **média de pelo menos 5 leituras** dos sensores, atualizados a cada **5 segundos**.

-----

## 🧰 Componentes Utilizados

- Arduino Uno
- Sensor LDR (fotoresistor)
- Sensor DHT11 (temperatura e umidade)
- Display LCD 16x2 (paralelo ou I2C)
- LED Verde
- LED Amarelo
- LED Vermelho
- Buzzer passivo
- Resistores diversos
- Protoboard e jumpers

-----

## 📦 Dependências / Bibliotecas

Instale as seguintes bibliotecas na IDE do Arduino:

- [`DHT sensor library`](https://github.com/adafruit/DHT-sensor-library) — Adafruit
- [`LiquidCrystal`](https://www.arduino.cc/reference/en/libraries/liquidcrystal/) — padrão Arduino (LCD paralelo)
- ou [`LiquidCrystal_I2C`](https://github.com/johnrickman/LiquidCrystal_I2C) — caso use comunicação I2C

### Como instalar:

1. Abra a Arduino IDE
1. Vá em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas…**
1. Pesquise pelo nome da biblioteca e clique em **Instalar**

-----

## 🔌 Como Reproduzir o Projeto

### Simulação Online

Acesse a simulação no Tinkercad / Wokwi pelo link abaixo:

🔗 **https://wokwi.com/projects/464730814628509697(#)**

### Executar fisicamente:

1. Monte o circuito conforme o diagrama disponível na pasta `/circuito`
1. Instale as bibliotecas necessárias (ver seção acima)
1. Abra o arquivo `codigo.ino` na Arduino IDE
1. Selecione a porta correta em **Ferramentas > Porta**
1. Clique em **Upload**

-----

## 📁 Estrutura do Repositório

```
📦 vinheria-agnello-cp2
├── 📄 README.md
├── 📂 codigo/
│   └── codigo.ino
├── 📂 circuito/
│   └── diagrama.png
└── 📂 docs/
    └── (imagens e materiais de apoio)
```

-----

## 📹 Vídeo de Apresentação

🎥 **https://youtu.be/iwwfQ3McPgI(#)** 

-----

## 📅 Informações da Entrega

|Item           |Detalhe                          |
|---------------|---------------------------------|
|Disciplina     |Edge Computing & Computer Systems|
|Checkpoint     |02                               |
|Turma          |1ESPV                            |
|Data de entrega|22/05/2026                       |
|Professor      |Prof. Lucas D. Augusto           |

-----

<p align="center">
  Desenvolvido com ❤️ para a FIAP — 2026
</p>
