// Definições de pinos para 8 pessoas
const int pulsePins[8] = {A0, A2, A4, A6, A8, A10, A12, A14};  // Pinos dos Pulse Sensors
const int gsrPins[8] = {A1, A3, A5, A7, A9, A11, A13, A15};    // Pinos para leitura dos sensores GSR

long lastBeats[8] = {0};                // Timestamps dos últimos batimentos detectados
float beatsPerMinute[8] = {0};          // BPM calculado para cada sensor
float bpmHistory[8][5] = {0};           // Histórico para média móvel de BPM
int bpmIndex[8] = {0};                  // Índices do histórico para cada sensor
bool sensorActive[8] = {false};         // Indica se o sensor está ativo
int thresholds[8] = {550};              // Threshold inicial para detecção de picos
int gsrValues[8] = {0};                 // Últimos valores lidos do GSR
int pulseValues[8] = {0};               // Últimos valores lidos do pulso
long lastActivity[8] = {0};             // Tempo da última atividade de cada sensor
const int INACTIVITY_TIMEOUT = 2000;    // Tempo limite para marcar sensor como inativo (ms)
const int BPM_MIN = 50;                 // Limite mínimo de BPM para valores realistas
const int BPM_MAX = 120;                // Limite máximo de BPM para valores realistas

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoramento de oito pessoas iniciado!");
}

void loop() {
  for (int i = 0; i < 8; i++) {
    // Leitura do GSR
    int gsrValue = analogRead(gsrPins[i]);

    // Verifica se o GSR está ativo (entre 400 e 700)
    if (gsrValue >= 400 && gsrValue <= 700) {
      gsrValues[i] = gsrValue;  // Atualiza o valor do GSR
    } else {
      sensorActive[i] = false;  // Marca como inativo se o GSR estiver fora da faixa
      beatsPerMinute[i] = 0;    // Zera BPM para sensores inativos
      continue;                 // Ignora processamento adicional
    }

    // Leitura do sensor de pulso
    int pulseValue = analogRead(pulsePins[i]);

    // Detecta picos com base no threshold e intervalo de tempo
    if (pulseValue > thresholds[i] && millis() - lastBeats[i] > 300) {  // Intervalo mínimo de 300ms
      long delta = millis() - lastBeats[i];
      lastBeats[i] = millis();

      float bpm = 60.0 / (delta / 1000.0);  // Calcula BPM
      if (bpm >= BPM_MIN && bpm <= BPM_MAX) {  // Verifica se o BPM está na faixa realista
        // Marca o sensor como ativo
        sensorActive[i] = true;
        lastActivity[i] = millis();  // Atualiza o tempo da última atividade

        // Armazena o BPM no histórico
        bpmHistory[i][bpmIndex[i]] = bpm;
        bpmIndex[i] = (bpmIndex[i] + 1) % 5;  // Atualiza índice circular

        // Calcula a média móvel do BPM
        float avgBPM = 0;
        for (int j = 0; j < 5; j++) {
          avgBPM += bpmHistory[i][j];
        }
        beatsPerMinute[i] = avgBPM / 5;  // Atualiza o BPM médio
      }
    }

    // Marca o sensor como inativo se não houver atividade por um tempo
    if (millis() - lastActivity[i] > INACTIVITY_TIMEOUT) {
      sensorActive[i] = false;
      beatsPerMinute[i] = 0;  // Zera BPM para sensores inativos
    }

    // Exibe os dados somente para sensores ativos
    if (sensorActive[i]) {
      Serial.print("Pessoa ");
      Serial.print(i + 1);
      Serial.print(" - BPM: ");
      Serial.print(beatsPerMinute[i]);
      Serial.print(", GSR: ");
      Serial.println(gsrValues[i]);
    }
  }

  delay(1000);  // Pequena pausa para evitar sobrecarga
}
