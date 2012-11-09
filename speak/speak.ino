volatile char wait = 0;
volatile char response = 0;
volatile int index = 0;
volatile char dindex = 0;
volatile int confirm = 855;
volatile int x =0;
volatile unsigned int oca = 300;
volatile unsigned int ocb = 100;
volatile unsigned int sample=0;
volatile unsigned int dupeb[22]={
  88,347,175,0,
  88,88,433,0,
  88,347,175,0,
  88,604,0,
  88,688,0,
  171,88,430,0};
volatile unsigned int dupea[22]={
  172,430,300,0,
  258,172,600,0,
  172,430,300,0,
  264,800,0,
  177,800,0,
  258,177,600,0};
  
volatile char ploop = 0;


ISR(TIMER1_CAPT_vect){
   // PORTB |= 0x20;
    sample = ICR1-sample;
    TCCR1B ^= 0x40;  //flip the edge trigger
    response++;
   // PORTB &= ~0x20;
}

ISR(TIMER1_OVF_vect){
}

ISR(TIMER1_COMPB_vect) {
    PORTB |= 0x20;
    if(dupea[dindex]==0) {
      TCCR1A = 0;  //disable pwm, normal mode
      TCCR1B = 1<<CS10;  //disable pwm, falling edge trigger
      TIMSK1 = (1<<ICIE1);
      wait++;
      PORTB |=0x10;
    } else {
      OCR1A = dupea[dindex];
      OCR1B = dupeb[dindex];
    }
    dindex++;
    PORTB &=~ 0x20;
}

void setup() {
  DDRB |= 0x34;    //led and oc1b
  DDRB &= ~0x01; //icp
  PORTB |= 0x24;
  PORTB &= ~0x20;
  
  TCCR1A = (1<<WGM11)|(1<<WGM10)|(1<<COM1B0)|(1<<COM1B1);
		 //oc1b toggle on compare match, fast PWM mode
  TCCR1B = (1<<WGM12)|(1<<WGM13)|(0<<CS10)|(0<<ICES1); 
		//CTC and no prescaler, falling edge(0x40)
  TCCR1C = 0;
  TCNT1 = 0;
  
  OCR1A = 1000;
  OCR1B=0;
  //TIMSK1 = 0x21;
  TIMSK1 = 0x01|(0<<OCIE1A)|(1<<OCIE1B); //overflow and compare interrupts
  TCCR1B |= 1<<CS10;
  
  sei();
}

void loop() {
  if(response>=2) {
    //wait = 0;
    delayMicroseconds(72);
    if(wait>=6) {
      delay(1000);
      wait=0;
      dindex=0;
    }
    TCCR1A = (1<<WGM11)|(1<<WGM10)|(1<<COM1B0)|(1<<COM1B1);
    TCCR1B = (1<<WGM12)|(1<<WGM13);
    TCNT1 = 0;
    OCR1A = 1000;
    OCR1B = 10;
    TIMSK1 = (1<<TOIE1)|(1<<OCIE1B);
    response=0;
    TCCR1B |= (1<<CS10);
    PORTB &= ~0x10;
  }
  
  //if(dindex>=8){dindex=0;}
  
  if(ploop==1) {
    Serial.begin(115200);
    for(int y=0;y<x;y++) {
      //Serial.println(sample[y]);
    }
    index=0;
    ploop=0;
    TIMSK1 |= 0x21;
  }
}
