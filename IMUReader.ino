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

	digitalWrite(CSB1, HIGH);
	digitalWrite(CSB2, HIGH);
	digitalWrite(CSB3, HIGH);
}

int readAcc(unsigned int coor) {
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
		if (aux & 0x800)
			aux &= ~(0x800);
		digitalWrite(CSB1, HIGH);

		return aux;
	}
	else
		return 0;
}

int readGyr(unsigned int coor) {
	int lsb;
	int msb;
	int aux;

	digitalWrite(CSB2, LOW);
	SPI.transfer(0x80 + coor);
	lsb = SPI.transfer(0x00);
	digitalWrite(CSB2, HIGH);

	digitalWrite(CSB2, LOW);
	SPI.transfer(0x80 + coor + 1);
	msb = SPI.transfer(0x00);
	digitalWrite(CSB2, HIGH);
	
	aux = msb;

	if (lsb & 0x80) {
		lsb = ~lsb;
		lsb &= 0x7F;
		lsb++;
	}

	if (msb & 0x80) {
		msb = ~msb;
		msb &= 0x7F;
		msb++;
	}

	if (aux & 0x80) {
		aux = (msb << 8) + lsb;
		aux = ~aux;
		aux++;
	}
	else {
		aux = (msb << 8) + lsb;
	}

	Serial.print(" msb:");
	Serial.println(msb);
	Serial.print(" lsb:");
	Serial.println(lsb);

	return aux;
}

int readMag(unsigned int coor) {
	int aux;

	digitalWrite(CSB3, LOW);
	SPI.transfer(0xC0 + coor);
	aux = (SPI.transfer(0x00) >> 3);
	digitalWrite(CSB3, HIGH);

	digitalWrite(CSB3, LOW);
	SPI.transfer(0xC0 + coor + 1);
	aux += (SPI.transfer(0x00) << 5);
	digitalWrite(CSB3, HIGH);

	return aux;
}

void loop() {
	int x, y, z;
	//  x = readAcc(x_lsb);
	//  y = readAcc(y_lsb);
	//  z = readAcc(z_lsb);
	//
	//  if ((x != 0) && (y != 0) && (z != 0)) {
	//
	//    Serial.print("Acc x:");
	//    Serial.print(x);
	//    Serial.print(" y:");
	//    Serial.print(y);
	//    Serial.print(" z:");
	//    Serial.println(z);
	//
	//    x = readGyr(x_lsb);
	//    y = readGyr(y_lsb);
	//    z = readGyr(z_lsb);
	//
	//    Serial.print("Gyr x:");
	//    Serial.print(x);
	//    Serial.print(" y:");
	//    Serial.print(y);
	//    Serial.print(" z:");
	//    Serial.println(z);
	//
	//    x = readMag(x_lsb);
	//    y = readMag(y_lsb);
	//    z = readMag(z_lsb);
	//
	//    Serial.print("Mag x:");
	//    Serial.print(x);
	//    Serial.print(" y:");
	//    Serial.print(y);
	//    Serial.print(" z:");
	//    Serial.println(z);
	//
	//    Serial.println();
	//  }

	delay(400);

	x = readGyr(x_lsb);
	//y = readGyr(y_lsb);
	//    z = readGyr(z_lsb);
	//
	Serial.print(" x:");
	Serial.println(x);
	//Serial.print(" y:");
	//Serial.println(y);
	//    Serial.print(" z:");
	//    Serial.println(z);

}
