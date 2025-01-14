// Definições de pinos para 8 pessoas
const int pulsePins[8] = {A0, A2, A4, A6, A8, A10, A12, A14};  // Pinos dos Pulse Sensors
const int gsrPins[8] = {A1, A3, A5, A7, A9, A11, A13, A15};    // Pinos para leitura dos sensores GSR

long lastBeats[8] = {0};              // Timestamps dos últimos batimentos detectados para cada sensor
float beatsPerMinute[8] = {0};        // BPM calculado para cada sensor
int activeChannels[8] = {0};          // Indica se o canal está ativo (1 = ativo, 0 = inativo)

// Definição de faixas
const int pulseMinThreshold = 500;    // Limite mínimo para considerar uma leitura válida do sensor de pulso
const int pulseMaxThreshold = 700;    // Limite máximo para considerar uma leitura válida do sensor de pulso
const int gsrMinThreshold = 450;      // Faixa válida de GSR
const int gsrMaxThreshold = 700;

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoramento de oito pessoas iniciado!");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // Leitura do valor de pulso
    int pulseValue = analogRead(pulsePins[i]);

    // Verifica se o canal está ativo com base no sensor de pulso
    if (pulseValue >= pulseMinThreshold && pulseValue <= pulseMaxThreshold) {
      activeChannels[i] = 1;  // Marca o canal como ativo

      // Calcular BPM apenas para canais ativos
      if (millis() - lastBeats[i] > 300) { // Intervalo mínimo de 300ms
        long delta = millis() - lastBeats[i];
        lastBeats[i] = millis();

        beatsPerMinute[i] = 60.0 / (delta / 1000.0);
        beatsPerMinute[i] = constrain(beatsPerMinute[i], 50, 150);  // Limita o BPM a valores realistas
      }

      // Leitura do valor GSR
      int gsrValue = analogRead(gsrPins[i]);

      // Verifica se o GSR está na faixa esperada
      if (gsrValue >= gsrMinThreshold && gsrValue <= gsrMaxThreshold) {
        // Exibe os dados apenas para canais válidos
        Serial.print("Pessoa ");
        Serial.print(i + 1);
        Serial.print(" - BPM: ");
        Serial.print(beatsPerMinute[i]);
        Serial.print(", GSR: ");
        Serial.println(gsrValue);
      } else {
        // GSR fora da faixa esperada
        Serial.print("Pessoa ");
        Serial.print(i + 1);
        Serial.println(" - GSR fora da faixa esperada.");
      }
    } else {
      activeChannels[i] = 0;  // Marca o canal como inativo
    }
  }

  delay(20);  // Pequena pausa para evitar sobrecarga de processamento
}
