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
    C1   | [ ]A1      /  A  \      D8[H] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [D]A4/SDA               D5[B]~|   D5
    C5   | [D]A5/SCL               D4[L] |   D4
         | [ ]A6              INT1/D3[ ]~|   D3
         | [ ]A7              INT0/D2[ ] |   D2
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

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
int LedEstado = 4;

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
SoftwareSerial serial(5, 6);

//=============
// [H] H-Bridge
//=============
//Definicoes pinos Arduino ligados a entrada da Ponte H
//Motor A
int IN1 = 11;
int IN2 = 10;

//Motor B
int IN3 = 9;
int IN4 = 8;

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//melodia do MARIO THEME
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//duração de cada nota
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

void setup()
{
  //=============================
  // Pino 4 do arduino como saída
  //=============================
  pinMode(LedEstado, OUTPUT);

  //Define os pinos como saída
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

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

    case 'B' : //Se 'B' for recebido, Toca a Buzina
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Play the Buzzer");

      //Play the Buzzer
      playBuzzer();

      break;

    case 'M' : //Se 'M' for recebido, Toca a Música
      //Mensagem será enviada para o módulo HC-06 e daí para o Android.
      serial.print("Play the Music");

      //Play Super Mario Theme
      playSuperMarioTheme();

      break;
  }

  delay(300);
}

void moveStop()
{
  //Move Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward()
{
  // Move Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward()
{
  //Move Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight()
{
  //Move Right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  //Move Left
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
  delay(500);

  //aqui sai o som
  tone(buzzer, 100, 300);
  delay(500);

  //aqui sai o som
  tone(buzzer, 900, 300);
  delay(500);
}

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas começando no 0 até 6 ++ incrementado
  for (int nota = 0; nota < 6; nota++)
  {
    int duracaodanota = duracaodasnotas[nota];
    tone(buzzer, melodia[nota], duracaodanota);

    //pausa depois das notas
    int pausadepoisdasnotas[] = {150, 300, 300, 100, 300, 550, 575};
    delay(pausadepoisdasnotas[nota]);
  }

  noTone(buzzer);
}
