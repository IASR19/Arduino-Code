// Definições de pinos para 8 pessoas
const int pulsePins[8] = {A0, A2, A4, A6, A8, A10, A12, A14};  // Pinos dos Pulse Sensors
const int gsrPins[8] = {A1, A3, A5, A7, A9, A11, A13, A15};    // Pinos para leitura dos sensores GSR

long lastBeats[8] = {0};                // Timestamps dos últimos batimentos detectados
float beatsPerMinute[8] = {0};          // BPM calculado para cada sensor
int lastPulseValues[8] = {0};           // Últimos valores para verificar variação
bool sensorActive[8] = {false};         // Indica se o sensor está ativo
const int INACTIVITY_TIMEOUT = 2000;    // Tempo limite para marcar sensor como inativo (ms)
const int BPM_MIN = 50;                 // Limite mínimo de BPM para valores realistas
const int BPM_MAX = 120;                // Limite máximo de BPM para valores realistas
const int GSR_MIN = 450;                // Limite mínimo para GSR
const int GSR_MAX = 700;                // Limite máximo para GSR

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoramento de oito pessoas iniciado!");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // Leitura do valor de pulso
    int pulseValue = analogRead(pulsePins[i]);

    // Detecta picos para calcular BPM
    if (pulseValue > 550 && millis() - lastBeats[i] > 300) {  // Threshold e intervalo mínimo
      long delta = millis() - lastBeats[i];
      lastBeats[i] = millis();

      float bpm = 60.0 / (delta / 1000.0);  // Calcula BPM
      if (bpm >= BPM_MIN && bpm <= BPM_MAX) {  // Verifica se o BPM está dentro da faixa realista
        beatsPerMinute[i] = bpm;
        sensorActive[i] = true;  // Marca o sensor como ativo
        lastPulseValues[i] = pulseValue;  // Atualiza o último valor
      } else {
        sensorActive[i] = false;  // Marca como inativo se o BPM estiver fora da faixa
      }
    } else if (millis() - lastBeats[i] > INACTIVITY_TIMEOUT) {
      sensorActive[i] = false;  // Marca como inativo se não houver atividade por muito tempo
      beatsPerMinute[i] = 0;    // Zera o BPM para sensores inativos
    }

    // Apenas exibe dados de sensores ativos
    if (sensorActive[i]) {
      // Leitura do valor GSR
      int gsrValue = analogRead(gsrPins[i]);

      // Verifica se o GSR está dentro da faixa válida
      if (gsrValue >= GSR_MIN && gsrValue <= GSR_MAX) {
        // Exibe os dados para cada pessoa ativa
        Serial.print("Pessoa ");
        Serial.print(i + 1);
        Serial.print(" - BPM: ");
        Serial.print(beatsPerMinute[i]);
        Serial.print(", GSR: ");
        Serial.println(gsrValue);
      } else {
        sensorActive[i] = false;  // Marca o canal como inativo se o GSR estiver fora da faixa
      }
    }
  }

  delay(1000);  // Pequena pausa para evitar sobrecarga de processamento
}
