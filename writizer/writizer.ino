/*
  Writizer writes stuff to serial for chkey to read!
 */
void setup() {
  pinMode(13, OUTPUT);
   pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
   pinMode(6, INPUT_PULLUP);
  Serial.begin(9600);

}
 int a=0,b=0,c=0;
  int last_a=0,last_b=0,last_c=0;
void loop() {
 
  a = !digitalRead(4);
  digitalWrite(13, a); // debug
  b = !digitalRead(5);
  c = !digitalRead(6);
  if (a!=last_a)  Serial.print(a==HIGH ? 'A' : 'D');
  if (b!=last_b) Serial.print(b==HIGH ? 'B' : 'E');
  if (c!=last_c) Serial.print(c==HIGH ? 'C' : 'F');
  
 
  delay(1);              // wait for a milisecond
  
  last_a=a;last_b=b,last_c=c;
}
