#include <avr/io.h>
#include <util/delay.h>

#define LENGTH 100  
   
int POTI = A1;
unsigned long startTime = 0;
volatile int RingPuffer[LENGTH];
float Mittelwert_bilden();


// falls die Arraylänge klein ist, dann kann die Mittelwertbildung auch innerhalb 
// der ISR erfolgen und die Genauigkeit ist höher ,da keine Messergebnisse verloren gehen.

// Wenn die Arraylänge sehr groß ist, dann ist es besser die Mittelwertbildung außerhalb erfolgen zu lassen,
// weil die ISR ansonsten zu lange dauern würde. Die Genauigkeit des Messergebnisses wird nicht beeinträchtigt.


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
      Serial.println("Mittelwert: " + String(Mittelwert_bilden()); 
      startTime = millis();
    }
  }
}

float Mittelwert_bilden()
{
  float mittelwert = 0;
  for (int i = 0; i < LENGTH; i++)
  {
    mittelwert += RingPuffer[i];
  }

  return mittelwert / LENGTH;
}



ISR (ADC_vect)   //Ringpuffer:= neuester Eintrag an anderer Stelle im Kreis 
{
    //*for (int i = LENGTH - 1; i > 0 ; i--)
    {
      RingPuffer[i] = RingPuffer[i - 1]; //Shiften der Elemente
    }

    RingPuffer[0] = ADC;  // neues Element an die erste Stelle */


static uint16_t i=0;
RingPuffer[i]=ADC;
i++;
if(i==LENGTH)i=0;


}





