#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const int ledVerde = 7;
const int ledAmarelo = 6;
const int ledVermelho = 5;
const int buzzer = 4;
const int LDR_pino = A0;

// Definindo o DHT22 no pino A1
const int DHT_pino = A1; 
#define DHTTYPE DHT22
DHT dht(DHT_pino, DHTTYPE);

unsigned long tempoAnterior = 0;
const long intervalo = 5000;

int pos = 0;
unsigned long tempoAnteriorCarrossel = 0;

unsigned long tempoAnteriorLinha2 = 0;
const long intervaloLinha2 = 3000;
int telaAtualLinha2 = 0;

unsigned long tempoAnteriorMedia = 0;
const long intervaloMedia = 1000;

// ===== CONTROLE DO BUZZER INTERMITENTE =====
unsigned long tempoAnteriorBuzzer = 0;
const long intervaloBuzzer = 300; // tempo entre liga/desliga do buzzer
bool estadoBuzzer = false;
bool buzzerIntermitente = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== MÉDIA DAS ÚLTIMAS 5 MEDIDAS =====
const int NUM_MEDIDAS = 5;

float historicoUmidade[NUM_MEDIDAS];
float historicoTemperatura[NUM_MEDIDAS];
int historicoLDR[NUM_MEDIDAS];

int indiceMedida = 0;
int totalMedidas = 0;

// Variáveis que armazenam as MÉDIAS
float umidade_DHT = 0;
float temperatura_DHT = 0;
int leitura_LDR = 0;

byte gota[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110,
  B00100
};

byte termometro[8] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

// Agora esta função controla apenas os LEDs
void EstadoLeds(bool verde, bool amarelo, bool vermelho) {
  digitalWrite(ledVerde, verde);
  digitalWrite(ledAmarelo, amarelo);
  digitalWrite(ledVermelho, vermelho);
}

// Função que faz o buzzer ficar intermitente
void AtualizarBuzzer() {
  unsigned long tempoAtual = millis();

  if (buzzerIntermitente) {
    if (tempoAtual - tempoAnteriorBuzzer >= intervaloBuzzer) {
      tempoAnteriorBuzzer = tempoAtual;

      estadoBuzzer = !estadoBuzzer;
      digitalWrite(buzzer, estadoBuzzer ? HIGH : LOW);
    }
  } 
  else {
    estadoBuzzer = false;
    digitalWrite(buzzer, LOW);
  }
}

void EscreverLCD(String tx1) {
  const int intervalo_carrossel = 300;
  unsigned long tempoAtualCarrossel = millis();
  
  if (tempoAtualCarrossel - tempoAnteriorCarrossel >= intervalo_carrossel) {
    lcd.setCursor(0, 0);
    
    tempoAnteriorCarrossel = tempoAtualCarrossel;
    
    lcd.print(tx1.substring(pos, pos + 16));
    pos++;
    
    if (pos > tx1.length() - 16) {
      pos = 0;
    }
  }
}

// Função para calcular as médias das últimas medidas
void calcularMedias() {
  float somaUmidade = 0;
  float somaTemperatura = 0;
  int somaLDR = 0;

  for (int i = 0; i < totalMedidas; i++) {
    somaUmidade += historicoUmidade[i];
    somaTemperatura += historicoTemperatura[i];
    somaLDR += historicoLDR[i];
  }

  umidade_DHT = somaUmidade / totalMedidas;
  temperatura_DHT = somaTemperatura / totalMedidas;
  leitura_LDR = somaLDR / totalMedidas;
}

void AtualizarAlertas() {
  if (leitura_LDR >= 800 || umidade_DHT < 40 || umidade_DHT > 70) {
    Serial.println("Estado: PERIGO CRITICO");

    EstadoLeds(false, false, true);     // vermelho
    buzzerIntermitente = true;          // buzzer piscando/tocando intermitente
  }
  else if (leitura_LDR >= 500 || temperatura_DHT < 10 || temperatura_DHT > 15) {
    Serial.println("Estado: ALERTA");

    EstadoLeds(false, true, false);     // amarelo

    // Buzzer só toca no alerta se for problema de temperatura
    if (temperatura_DHT < 10 || temperatura_DHT > 15) {
      buzzerIntermitente = true;
    } else {
      buzzerIntermitente = false;
    }
  } 
  else {
    Serial.println("Estado: OK");

    EstadoLeds(true, false, false);     // verde
    buzzerIntermitente = false;         // buzzer desligado
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(LDR_pino, INPUT);
  
  dht.begin();
  
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);
  
  lcd.init();
  lcd.backlight();
  
  EscreverLCD("Vinharia Agnello      ");

  lcd.createChar(1, gota);
  lcd.createChar(2, termometro);
}

void loop()
{  
  unsigned long tempoAtual = millis();

  // O buzzer precisa ser atualizado sempre,
  // fora dos blocos de 5 segundos.
  AtualizarBuzzer();

  // === BLOCO 1: LEITURA E CÁLCULO DA MÉDIA ===
  if (tempoAtual - tempoAnteriorMedia >= intervaloMedia) {
    tempoAnteriorMedia = tempoAtual;

    int leitura_LDR_atual = 1023 - analogRead(LDR_pino);
    float umidade_atual = dht.readHumidity();
    float temperatura_atual = dht.readTemperature();

    if (isnan(umidade_atual) || isnan(temperatura_atual)) {
      Serial.println("Falha ao ler o DHT22! Medida ignorada.");
    } 
    else {
      historicoLDR[indiceMedida] = leitura_LDR_atual;
      historicoUmidade[indiceMedida] = umidade_atual;
      historicoTemperatura[indiceMedida] = temperatura_atual;

      indiceMedida++;

      if (indiceMedida >= NUM_MEDIDAS) {
        indiceMedida = 0;
      }

      if (totalMedidas < NUM_MEDIDAS) {
        totalMedidas++;
      }

      calcularMedias();
    }
  }

  // Se ainda não houver nenhuma medida válida, não atualiza nada
  if (totalMedidas == 0) {
    EscreverLCD("Aguardando leitura                ");
    return;
  }

  // === BLOCO 2: ALERTAS E LEDS A CADA 5 SEGUNDOS ===
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;

    Serial.print("Media Umidade: ");
    Serial.print(umidade_DHT);
    Serial.print("% | Media Temp: ");
    Serial.print(temperatura_DHT);
    Serial.print("°C | Media Luz: ");
    Serial.println(leitura_LDR);

    AtualizarAlertas();
  }

  // === BLOCO 3: CARROSSEL DA LINHA 1 ===
  if (leitura_LDR > 800 || umidade_DHT < 40 || umidade_DHT > 70) {
    if (leitura_LDR >= 800) {
      EscreverLCD(" Ambiente muito claro                ");
    } else {
      EscreverLCD(" Umidade Critica          ");
    }
  }
  else if (leitura_LDR > 500 || temperatura_DHT < 10 || temperatura_DHT > 15) {
    if (leitura_LDR >= 500) {
      EscreverLCD(" Ambiente a meia luz                ");
    } else {
      EscreverLCD(" Temperatura Critica          ");
    }
  } 
  else {
    EscreverLCD(" Sistema Operando OK                ");
  }

  // === BLOCO 4: CRONÔMETRO DA LINHA 2 ===
  if (tempoAtual - tempoAnteriorLinha2 >= intervaloLinha2) {
    tempoAnteriorLinha2 = tempoAtual;
    telaAtualLinha2++; 

    if (telaAtualLinha2 > 2) { 
      telaAtualLinha2 = 0; 
    }

    lcd.setCursor(0, 1);
    lcd.print("                ");
  }

  // === BLOCO 5: RENDERIZAÇÃO DA LINHA 2 ===
  if (telaAtualLinha2 == 0) {
    lcd.setCursor(0, 1);
    lcd.write(byte(1)); 
    lcd.print(" ");
    lcd.print(int(umidade_DHT)); 
    lcd.print("% "); 
      
    lcd.setCursor(9, 1);
    lcd.write(byte(2)); 
    lcd.print(" ");
    lcd.print(int(temperatura_DHT)); 
    lcd.print((char)223);
    lcd.print("C ");
  }
  else if (telaAtualLinha2 == 1) {
    lcd.setCursor(0, 1);

    if (umidade_DHT < 40) {
      lcd.print("Umidade Baixa   ");
    }
    else if (umidade_DHT > 70) {
      lcd.print("Umidade Alta    ");
    }
    else {
      lcd.print("Umidade OK      ");
    }
  }
  else if (telaAtualLinha2 == 2) {
    lcd.setCursor(0, 1);

    if (temperatura_DHT < 10) {
      lcd.print("Temp. Baixa     ");
    }
    else if (temperatura_DHT > 15) {
      lcd.print("Temp. Alta      ");
    }
    else {
      lcd.print("Temp. OK        ");
    }
  }
}