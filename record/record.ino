volatile char mode = 0;
volatile int index = 0;
volatile int x =0;
volatile unsigned int sample[60];
volatile char ploop = 0;
char reset=16;


ISR(TIMER1_CAPT_vect){
    PORTB |= 0x10;
    sample[index] = ICR1;
    TCCR1B ^= 0x40;  //flip the edge trigger
    index++;
    PORTB ^= 0x20;
    PORTB &=~0x10;
}

ISR(TIMER1_OVF_vect){
  if(index>18){
    x=index;
    TIMSK1 &= ~0x21;
    ploop = 1;
  } 
}

void setup() {
  DDRB |= 0x34;    //led and oc1b
  DDRB &= ~0x01; //icp
  PORTB |= 0x04;
  
  TCCR1A = (0<<WGM11)|(0<<WGM10)|(1<<COM1A0);
		 //oc1a toggle on compare match, CTC mode
  TCCR1B = (0<<WGM12)|(0<<WGM13)|(0<<CS10)|(0<<ICES1); 
		//CTC and no prescaler, falling edge(0x40)
  TCCR1C = 0;
  TCNT1 = 0;
  OCR1A = 0xffff;
  TIMSK1 = (1<<TOIE1)|(1<<ICIE1); //|(0<<OCIE1B); overflow and compare interrupts
  TCCR1B |= (1<<CS10); //start clock
  
  sei();
}

void loop() {
  if(index==reset) {
    TCNT1=0;
    reset--;
  }
  
  if(ploop==1) {
    Serial.begin(115200);
    for(int y=1;y<x;y++) {
      Serial.println(sample[y]-sample[y-1]);
    }
    index=0;
    ploop=0;
    TIMSK1 |= 0x21;
    reset=16;
    Serial.println(x);
    Serial.println("/n");
  }
}
