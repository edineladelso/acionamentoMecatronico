// --- Sistema de comando estrela-triângulo com botão de emergência ---
// Autor: Edinel Marinho
// Data: 2025

// Pinos de entrada
const int botaoStart = 2;
const int botaoStop  = 3;
const int botaoEmerg = 4;   // Botão de emergência (NC)

// Pinos de saída
const int K1 = 8;  // Principal
const int K3 = 9;  // Estrela
const int K2 = 10; // Triângulo

// Tempos (ajustáveis)
const unsigned long tempoPartida = 5000;  // 5 s
const unsigned long tempoMorto   = 500;   // 0.5 s

// Estados do sistema
enum EstadoMotor {DESLIGADO, ESTRELA, TEMPO_MORTO, TRIANGULO, EMERGENCIA};
EstadoMotor estado = DESLIGADO;

// Controlo de tempo
unsigned long tempoInicio = 0;

void setup() {
  pinMode(botaoStart, INPUT_PULLUP);
  pinMode(botaoStop, INPUT_PULLUP);
  pinMode(botaoEmerg, INPUT_PULLUP);  // NC → HIGH normal, LOW = emergência

  pinMode(K1, OUTPUT);
  pinMode(K2, OUTPUT);
  pinMode(K3, OUTPUT);

  desligarTudo();
}

void loop() {
  // Leitura dos botões
  bool startPressionado = (digitalRead(botaoStart) == LOW);
  bool stopPressionado  = (digitalRead(botaoStop) == LOW);
  bool emergenciaAtiva  = (digitalRead(botaoEmerg) == LOW); // Botão pressionado → emergência

  // --- Prioridade total do botão de emergência ---
  if (emergenciaAtiva) {
    estado = EMERGENCIA;
    desligarTudo();
  }

  switch (estado) {
    case DESLIGADO:
      if (startPressionado && !emergenciaAtiva) {
        ligarEstrela();
      }
      break;

    case ESTRELA:
      if (millis() - tempoInicio >= tempoPartida) {
        iniciarTempoMorto();
      }
      break;

    case TEMPO_MORTO:
      if (millis() - tempoInicio >= tempoMorto) {
        ligarTriangulo();
      }
      break;

    case TRIANGULO:
      // Motor em regime, aguarda Stop ou emergência
      break;

    case EMERGENCIA:
      // Espera até o botão ser rearmado manualmente
      if (!emergenciaAtiva) {
        estado = DESLIGADO;  // Libera o sistema após rearme
      }
      break;
  }

  // Paragem normal
  if (stopPressionado && estado != EMERGENCIA) {
    desligarTudo();
  }
}

// ---- Funções auxiliares ----
void ligarEstrela() {
  digitalWrite(K1, HIGH);
  digitalWrite(K3, HIGH);
  digitalWrite(K2, LOW);
  tempoInicio = millis();
  estado = ESTRELA;
}

void iniciarTempoMorto() {
  digitalWrite(K3, LOW);
  tempoInicio = millis();
  estado = TEMPO_MORTO;
}

void ligarTriangulo() {
  digitalWrite(K2, HIGH);
  estado = TRIANGULO;
}

void desligarTudo() {
  digitalWrite(K1, LOW);
  digitalWrite(K2, LOW);
  digitalWrite(K3, LOW);
}
