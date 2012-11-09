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

void synthInit(void);

ISR(TIMER1_CAPT_vect){
   // PORTB |= 0x20;
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
  
  synthInit();
  
  sei();
}

void loop() {
  if(response>=2) {
    //wait = 0;
    delayMicroseconds(72);
    if(wait>=6) {
      wait=0;
      dindex=0;
    }
    response=0;
    synthInit();
    PORTB &= ~0x10;
  }
}


void synthInit(void) {
  TCCR1A = (1<<WGM11)|(1<<WGM10)|(1<<COM1B0)|(1<<COM1B1);
  TCCR1B = (1<<WGM12)|(1<<WGM13)|(0<<CS10)|(0<<ICES1);
  TCNT1 = 0;
  OCR1A = 1000;
  OCR1B = 0;
  TIMSK1 = (1<<TOIE1)|(1<<OCIE1B);
  TCCR1B |= (1<<CS10);
}

