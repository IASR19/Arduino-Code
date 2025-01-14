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

    // Remapeia o GSR para a faixa de 450 a 750
    int remappedGSR = map(gsrValue, 0, 1023, 450, 750);
    remappedGSR = constrain(remappedGSR, 450, 750);  // Garante que fique na faixa

    // Processa o batimento cardíaco para cada sensor
    int pulseValue = analogRead(pulsePins[i]);

    // Remapeia o valor do ADC para BPM (50 a 150)
    float remappedBPM = map(pulseValue, 0, 1023, 50, 100);
    remappedBPM = constrain(remappedBPM, 50, 150);  // Garante que o valor fique na faixa

    // Detecta picos com base em variação e intervalo
    if (pulseValue - lastPulseValues[i] > thresholds[i] && millis() - lastBeats[i] > 300) {
      long delta = millis() - lastBeats[i];
      lastBeats[i] = millis();

      beatsPerMinute[i] = remappedBPM;  // Usa o valor remapeado para BPM

      // Exibe os dados para cada pessoa
      Serial.print("Pessoa ");
      Serial.print(i + 1);
      Serial.print(" - BPM: ");
      Serial.print(beatsPerMinute[i]);
      Serial.print(", GSR: ");
      Serial.println(remappedGSR);
    }
  }

  delay(20);  // Pequena pausa para evitar sobrecarga de processamento
}
