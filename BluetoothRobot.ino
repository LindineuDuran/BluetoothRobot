//Programa : Robô Controlado por Bluetooth que Toca Buzina e Música.
//Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[H]~|   B3
         | [ ]V.ref     ___    SS/D10[H]~|   B2
    C0   | [ ]A0       / N \       D9[H]~|   B1
    C1   | [ ]A1      /  A  \      D8[H] |   B0
    C2   | [ ]A2      \  N  /      D7[S] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [D]A4/SDA               D5[B]~|   D5
    C5   | [D]A5/SCL               D4[ ] |   D4
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

//melodia do MARIO THEME
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//duração de cada nota
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

/* Defini??o de um objeto SoftwareSerial.
   Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
   Isto evita o erro do Avrdude
*/
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

//Buzzer conectado ao pino 12
int buzzer = 12;

void setup()
{
  //Define os pinos como saida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Sets the baud for serial data transmission
  serial.begin(9600);

  //Pino 12 do arduino como sa?da
  pinMode(buzzer, OUTPUT);
}

void loop()
{
  //Criando uma vari?vel do tipo caracter
  char z;

  //Vari?vel 'z' recebe o valor da porta Serial
  z = serial.read();

  switch (z)
  {
    case 'F' : //Se 'F' for recebido, vai para Frente
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Move Forward");

      // Move Forward
      moveForward();

      break;

    case 'T' : //Se 'T' for recebido, vai para Tr?s
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Move Backward");

      //Move Backward
      moveBackward();

      break;

    case 'E' : //Se 'E' for recebido, vira para Esquerda
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Move Left");

      //Move Left
      turnLeft();

      break;

    case 'D' : // 'D' for recebido acende led amarelo, vira para Direita
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Move Right");

      //Move Right
      turnRight();

      break;

    case 'P' : //Se 'P' for recebido, P?ra o Movimento
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Stop the Move");

      //Move Stop
      moveStop();

      break;

    case 'B' : //Se 'B' for recebido, Toca a Buzina
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Play the Buzzer");

      //Play the Buzzer
      playBuzzer();

      break;

    case 'M' : //Se 'M' for recebido, Toca a M?sica
      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      serial.print("Play the Music");

      //Play Super Mario Theme
      playSuperMarioTheme();

      break;

      //    default : //Se 'P' for recebido, P?ra o Movimento
      //      //Mensagem ser? enviada para o m?dulo HC-06 e da? para o Android.
      //      serial.print("Stop the Move");
      //
      //      //Move Stop
      //      moveStop();
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
    o n?mero 12 indica que o pino positivo do buzzer est? na porta 10
    o n?mero 300 ? a frequ?ncia que ser? tocado
    o n?mero 300 ? a dura??o do som
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
  //for para tocar as 156 notas come?ando no 0 ate 156 ++ incrementado
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
