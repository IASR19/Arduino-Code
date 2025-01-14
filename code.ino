// Definições de pinos para 8 pessoas
const int pulsePins[8] = {A0, A2, A4, A6, A8, A10, A12, A14};  // Pinos dos Pulse Sensors
const int gsrPins[8] = {A1, A3, A5, A7, A9, A11, A13, A15};    // Pinos para leitura dos sensores GSR

long lastBeats[8] = {0};               // Timestamps dos últimos batimentos detectados
float beatsPerMinute[8] = {0};         // BPM calculado para cada sensor
int lastPulseValues[8] = {0};          // Últimos valores do sensor de pulso
int gsrHistory[8][10] = {0};           // Histórico de GSR para média móvel
int thresholds[8] = {100};             // Threshold para detecção de picos (ajustável por sensor)

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoramento de oito pessoas iniciado!");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // 1. Leitura do GSR
    int gsrValue = analogRead(gsrPins[i]);

    // Média móvel para suavizar GSR (10 leituras)
    for (int j = 9; j > 0; j--) {
      gsrHistory[i][j] = gsrHistory[i][j - 1];
    }
    gsrHistory[i][0] = gsrValue;

    int avgGSR = 0;
    for (int j = 0; j < 10; j++) {
      avgGSR += gsrHistory[i][j];
    }
    avgGSR /= 10;

    // Filtrar valores extremos para GSR
    avgGSR = constrain(avgGSR, 400, 700);

    // 2. Processa o batimento cardíaco (ECG)
    int pulseValue = analogRead(pulsePins[i]);

    // Detecta picos (Threshold + Intervalo de Tempo)
    if (pulseValue > thresholds[i] && millis() - lastBeats[i] > 600) {  // Intervalo mínimo de 600ms
      long delta = millis() - lastBeats[i];
      lastBeats[i] = millis();

      beatsPerMinute[i] = 60.0 / (delta / 1000.0);  // Calcula BPM
      beatsPerMinute[i] = constrain(beatsPerMinute[i], 50, 90);  // Filtro de valores realistas
    }

    // 3. Exibe os dados para cada pessoa
    Serial.print("Pessoa ");
    Serial.print(i + 1);
    Serial.print(" - BPM: ");
    Serial.print(beatsPerMinute[i]);
    Serial.print(", GSR: ");
    Serial.println(avgGSR);
  }

  delay(20);  // Pequena pausa para evitar sobrecarga
}
