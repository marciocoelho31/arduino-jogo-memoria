#define LED_VERDE 2
#define LED_AMARELO 3
#define LED_VERMELHO 4
#define LED_AZUL 5

#define BOTAO_VERDE 10
#define BOTAO_AMARELO 11
#define BOTAO_VERMELHO 12
#define BOTAO_AZUL 13

#define BUZZER 8

#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define CINQUENTA_MILISEGUNDOS 50

#define TAMANHO_SEQUENCIA 4

#define INDEFINIDO -1

enum Estados {
  PRONTO_PARA_PROXIMA_RODADA,   // 0
  USUARIO_RESPONDENDO,          // 1
  JOGO_FINALIZADO_COM_SUCESSO,  // 2
  JOGO_FINALIZADO_COM_FALHA     // 3
};

int sequenciaLuzes[TAMANHO_SEQUENCIA];

int rodada = 0;
int ledsRespondidos = 0;

void setup() {
  Serial.begin(9600);
  iniciaPortas();
  iniciaJogo();
}

void iniciaJogo() {
  int jogo = analogRead(0);
  randomSeed(jogo);
  Serial.println(jogo);

  for (int i = 0; i < TAMANHO_SEQUENCIA; i++){
    sequenciaLuzes[i] = sorteiaCor();  
  }
}

int sorteiaCor() {
  return random(LED_VERDE, LED_AZUL + 1); // 2 a 5 (6)
}

void iniciaPortas() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
}

void loop() {
  switch (estadoAtual()) {
    case PRONTO_PARA_PROXIMA_RODADA:
      Serial.println("Pronto para proxima rodada");
      preparaNovaRodada();
      break;
    case USUARIO_RESPONDENDO:
      Serial.println("Usuario respondendo");
      processaRespostaDoUsuario();
      break;
    case JOGO_FINALIZADO_COM_SUCESSO:
      Serial.println("Jogo finalizado com sucesso");
      jogoFinalizadoComSucesso();
      break;
    case JOGO_FINALIZADO_COM_FALHA:
      Serial.println("Jogo finalizado com falha");
      jogoFinalizadoComFalha();
      break;
  }
}

void preparaNovaRodada() {
  rodada++;
  ledsRespondidos = 0;
  if (rodada <= TAMANHO_SEQUENCIA) {
    piscaLedsDaRodada();
  }
}

void processaRespostaDoUsuario() {
  int resposta = checaRespostaJogador();

  if (resposta == INDEFINIDO){
    return;
  }

  if (resposta == sequenciaLuzes[ledsRespondidos]) {
    ledsRespondidos++;
  } else {
    Serial.println("Resposta errada");
    rodada = TAMANHO_SEQUENCIA + 2;   // para cair na condição do estado de jogo finalizado com falha
  }
}

int estadoAtual() {
  if (rodada <= TAMANHO_SEQUENCIA){
    if (ledsRespondidos == rodada){
      return PRONTO_PARA_PROXIMA_RODADA;
    } else {
      return USUARIO_RESPONDENDO;
    }
  } else if (rodada == TAMANHO_SEQUENCIA + 1) {
    return JOGO_FINALIZADO_COM_SUCESSO;
  } else {
    return JOGO_FINALIZADO_COM_FALHA;
  }
}

void piscaLedsDaRodada() {
  for(int indice = 0; indice < rodada; indice++) {
    piscaLed(sequenciaLuzes[indice]);
  }
}

int checaRespostaJogador() {
  if (digitalRead(BOTAO_VERDE) == LOW) {
    return piscaLed(LED_VERDE);
  }
  if (digitalRead(BOTAO_AMARELO) == LOW) {
    return piscaLed(LED_AMARELO);
  }
  if (digitalRead(BOTAO_VERMELHO) == LOW) {
    return piscaLed(LED_VERMELHO);
  }
  if (digitalRead(BOTAO_AZUL) == LOW) {
    return piscaLed(LED_AZUL);
  }
  return INDEFINIDO;
}

void jogoFinalizadoComSucesso() {

  digitalWrite(LED_VERDE, HIGH);
  delay(CINQUENTA_MILISEGUNDOS);
  digitalWrite(LED_VERDE, LOW);
  delay(CINQUENTA_MILISEGUNDOS);
}

void jogoFinalizadoComFalha() {
  tocaSom(300);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(CINQUENTA_MILISEGUNDOS);
  digitalWrite(LED_VERMELHO, LOW);
  delay(CINQUENTA_MILISEGUNDOS);
}

int piscaLed(int portaLed) {
  
  verificaSomDoLed(portaLed);
  
  digitalWrite(portaLed, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(portaLed, LOW);
  delay(MEIO_SEGUNDO);
  
  return portaLed;
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100);
}

void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2500);
      break;
  }
}