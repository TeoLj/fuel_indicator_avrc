#include <avr/io.h>
#include <util/delay.h>

#define LENGTH 100

int POTI = A1;
unsigned long startTime = 0;
volatile int RingPuffer[LENGTH];
float average_val();


int main()
{

  init();
  Serial.begin(9600);
  DDRC |= (1 << PORTC1);

  // AD Einstellungen löschen
  ADMUX = 0;
  ADCSRA = 0;

  // ADC aktiviern
  ADCSRA |= (1 << ADEN);

  //Referenzspannung interne 5V
  ADMUX |= (1 << REFS0);


  // Auto trigger enablen
  ADCSRA |= (1 << ADATE);

  // Kanal0 des MUX
  ADMUX &= ~(1 << MUX0);

  //Taktteiler auf 128
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  //ADC Interrupt einschalten und starten
  ADCSRA |= (1 << ADIE) | (1 << ADSC);

  //Fortlaufender Betriebsmodus (schon by default eingestellt, kann auch weggelassen werden)
  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

  while (1)
  {
    if ((millis() - startTime) > 1000)
    {
      Serial.println("average value: " + String(average_val()));
                     startTime = millis();
    }
  }
}

float average_val()
{
  float average = 0;
  for (int i = 0; i < LENGTH; i++)
  {
    average += RingPuffer[i];
  }

  return average/ LENGTH;
}



ISR (ADC_vect)   //Ringpuffer
{
  static uint16_t i = 0;
  RingPuffer[i] = ADC;
  i++;
  if (i == LENGTH)i = 0;

}





