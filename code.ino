// Definições de pinos para 8 pessoas
const int pulsePins[8] = {A0, A2, A4, A6, A8, A10, A12, A14};  // Pinos dos Pulse Sensors
const int gsrPins[8] = {A1, A3, A5, A7, A9, A11, A13, A15};    // Pinos para leitura dos sensores GSR

long lastBeats[8] = {0};  // Timestamps dos últimos batimentos detectados para cada sensor
float beatsPerMinute[8] = {0};  // BPM calculado para cada sensor
int lastPulseValues[8] = {0};  // Armazena o último valor do sensor para comparação
int thresholds[8] = {20};  // Ajustável dinamicamente para cada sensor

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoramento de oito pessoas iniciado!");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // Leitura do valor GSR para cada pessoa
    int gsrValue = analogRead(gsrPins[i]);

    // Suavização do GSR (Média Móvel Simples)
    gsrValue = (gsrValue + lastPulseValues[i]) / 2;
    lastPulseValues[i] = gsrValue;

    // Processa o batimento cardíaco para cada sensor
    int pulseValue = analogRead(pulsePins[i]);

    // Detecta picos com base em variação e intervalo
    if (pulseValue - lastPulseValues[i] > thresholds[i] && millis() - lastBeats[i] > 300) {
      long delta = millis() - lastBeats[i];
      lastBeats[i] = millis();

      beatsPerMinute[i] = 60.0 / (delta / 1000.0);
      beatsPerMinute[i] = constrain(beatsPerMinute[i], 20, 255);  // Filtro de valores realistas

      // Exibe os dados para cada pessoa
      Serial.print("Pessoa ");
      Serial.print(i + 1);
      Serial.print(" - BPM: ");
      Serial.print(beatsPerMinute[i]);
      Serial.print(", GSR: ");
      Serial.println(gsrValue);
    }
  }

  delay(20);  // Pequena pausa para evitar sobrecarga de processamento
}
