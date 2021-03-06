//Programa : Robô Controlado por Bluetooth que Toca Buzina e Música.
//Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[Z] |   B4
         | [ ]3.3V           MOSI/D11[H]~|   B3
         | [ ]V.ref     ___    SS/D10[H]~|   B2
    C0   | [ ]A0       / N \       D9[H]~|   B1
    C1   | [ ]A1      /  A  \      D8[L] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [ ]A4/SDA               D5[B]~|   D5
    C5   | [ ]A5/SCL               D4[H] |   D4
         | [ ]A6              INT1/D3[H]~|   D3
         | [ ]A7              INT0/D2[H] |   D2
         | [ ]5V                  GND[ ] |
    C6   | [ ]RST                 RST[ ] |   C6
         | [ ]GND   5V MOSI GND   TX1[ ] |   D0
         | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |   D1
         |          [ ] [ ] [ ]          |
         |          MISO SCK RST         |
         | NANO-V3                       |
         +-------------------------------+

         http://busyducks.com/ascii-art-arduinos
*/

#include <SoftwareSerial.h>
#include <L298N.h>

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
#define LedEstado 8

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
#define RX 5
#define TX 6
SoftwareSerial serial(RX, TX);

//======================================================
// [H] H-Bridge
// Definicoes pinos Arduino ligados a entrada da Ponte H
//======================================================
//Motor A
#define ENA 11
#define IN1 9
#define IN2 10

//Motor B
#define ENB 3
#define IN3 4
#define IN4 2

//========================
//create a motor instances
//========================
L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//melodia do MARIO THEME
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//duração de cada nota
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

//pausa depois das notas
int pausadepoisdasnotas[] = {150, 300, 300, 100, 300, 550, 575};

//constants won't change:
const long interval = 20; // interval at which to blink (milliseconds)

//variáveis globais:
int velocidade = 100; //Velocidade dos Motores

void setup()
{
  //=============================
  // Pino 4 do arduino como saída
  //=============================
  pinMode(LedEstado, OUTPUT);

  //===============================
  //Define a velocidade dos motores
  //===============================
  motorA.setSpeed(velocidade);
  motorB.setSpeed(velocidade);

  //Sets the baud for serial data transmission
  serial.begin(9600);

  //Pino 12 do arduino como saída
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  //Criando uma variável do tipo caracter
  char z;

  //Variável 'z' recebe o valor da porta Serial
  z = serial.read();

  pausa(interval);

  switch (z)
  {
    case 'F' : //Se 'F' for recebido, vai para Frente
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Move Forward");

      // Move Forward
      moveForward();

      //Acende o LED.
      digitalWrite(LedEstado, HIGH);

      break;

    case 'T' : //Se 'T' for recebido, vai para Trás
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Move Backward");

      //Move Backward
      moveBackward();

      //Acende o LED.
      digitalWrite(LedEstado, HIGH);

      break;

    case 'E' : //Se 'E' for recebido, vira para Esquerda
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Move Left");

      //Move Left
      turnLeft();

      //Acende o LED.
      digitalWrite(LedEstado, HIGH);

      break;

    case 'D' : //Se 'D' for recebido, vira para Direita
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Move Right");

      //Move Right
      turnRight();

      //Acende o LED.
      digitalWrite(LedEstado, HIGH);

      break;

    case 'P' : //Se 'P' for recebido, Pára o Movimento
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Stop the Move");

      //Move Stop
      moveStop();

      //Apaga o LED.
      digitalWrite(LedEstado, LOW);

      break;

    case 'V' : //Se 'V' for recebido, Aumenta a Velocidade
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Velocidade: ");
      serial.println(velocidade);

      //Play the Buzzer
      playBuzzer();

      //Aumenta a Velocidade
      velocidade += 10;
      if (velocidade > 250)
      {
        velocidade = 250;
      }

      //=================================
      //Redefine a velocidade dos motores
      //=================================
      motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
      motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
      break;

    case 'v' : //Se 'v' for recebido, Diminui a Velocidade
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Velocidade: ");
      serial.println(velocidade);

      //Play the Buzzer
      playBuzzer();

      //Aumenta a Velocidade
      velocidade -= 10;
      if (velocidade < 0)
      {
        velocidade = 0;
      }

      //=================================
      //Redefine a velocidade dos motores
      //=================================
      motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
      motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
      break;

    case 'M' : //Se 'M' for recebido, Toca a Música
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Play the Music");

      //Play Super Mario Theme
      playSuperMarioTheme();

      break;
  }
}

void pausa(unsigned int milisegundos)
{
  volatile unsigned long compara = 0;
  volatile int contador = 0;
  do
  {
    if (compara != millis())
    {
      contador++;
      compara = millis();
    }
  } while (contador <= milisegundos);
  return;
}

void moveStop()
{
  //Move Stop
  motorA.stop();
  motorB.stop();
}

void moveForward()
{
  // Move Forward
  motorA.forward();
  motorB.forward();
}

void moveBackward()
{
  //Move Backward
  motorA.backward();
  motorB.backward();
}

void turnRight()
{
  //Move Right
  motorA.forward();
  motorB.backward();
}

void turnLeft()
{
  //Move Left
  motorA.backward();
  motorB.forward();
}

void playBuzzer()
{
  /*
    o número 12 indica que o pino positivo do buzzer está na porta 12
    o número 300 é a frequência que será tocado
    o número 300 é a duração do som
  */
  //aqui sai o som
  tone(buzzer, 300, 300);
  pausa(500);

  //aqui sai o som
  tone(buzzer, 100, 300);
  pausa(500);

  //aqui sai o som
  tone(buzzer, 900, 300);
  pausa(500);
}

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas começando no 0 até 6 ++ incrementado
  for (int nota = 0; nota < 6; nota++)
  {
    int duracaodanota = duracaodasnotas[nota];
    tone(buzzer, melodia[nota], duracaodanota);
    pausa(pausadepoisdasnotas[nota]);
  }

  noTone(buzzer);
}
