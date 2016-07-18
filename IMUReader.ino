#include <SPI.h>

int CSB1 = A2;
int CSB3 = A0;
int CSB2 = A1;

unsigned int x_lsb = 0x02;
unsigned int y_lsb = 0x04;
unsigned int z_lsb = 0x06;

const byte READ = 0b10000000;     // SCP1000's read command

void setup() {
  pinMode(CSB1, OUTPUT);
  pinMode(CSB2, OUTPUT);
  pinMode(CSB3, OUTPUT);

  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);

  digitalWrite(CSB1, LOW);
  digitalWrite(CSB2, HIGH);
  digitalWrite(CSB3, HIGH);
}

int readCoord(unsigned int coor) {
  int aux;

  digitalWrite(CSB1, LOW);
  SPI.transfer(0x80 + coor); // prepare for read operation register coor where are LSB.
  aux = SPI.transfer(0x00); // reads LSB (4 bits)
  digitalWrite(CSB1, HIGH);

  //check if acceleration value has been updated since last it has been read out last
  if (aux & 1) {
    aux = aux >> 4; 

    digitalWrite(CSB1, LOW);
    SPI.transfer(0x80 + coor + 1); // MSB is on the next register
    aux += (SPI.transfer(0x00) << 4); // sums MSB and LSB
	
	// fix the aux that is in two complements format
    if (aux & 0x800)
      aux &= ~(0x800);
    digitalWrite(CSB1, HIGH);
    
    return aux;
  }
  else
    return 0;
}

void loop() {
  int x, y, z;
  x = readCoord(x_lsb);
  y = readCoord(y_lsb);
  z = readCoord(z_lsb);

  if ((x != 0) && (y != 0) && (z != 0)) {

    Serial.print("x:");
    Serial.print(x);
    Serial.print(" y:");
    Serial.print(y);
    Serial.print(" z:");
    Serial.println(z);

  }

  delay(250);
}

