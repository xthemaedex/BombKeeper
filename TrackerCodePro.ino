/**
 * @author Anthony Sarisin s2379554
 * This sketch controls the prototype of the tracking device from our projct Smart Products group 16\
 * It uses 3 LED's, a vibration motor and a potentiometer. The middle LED is always on when the arduino is connected
 * When the data on the potentiometer is rasied, the left LED will light up and the vibration motor will vibrate at a certain tempo.
 * When the data on the potentiometer is rasied more, the right LED will light up and the vibration motor will vibrate at a higher tempo.
 */

int mvm = 5; // micro vibration motor is connected with pin number 5 which is the pwm pin. 
int vresistor = A1; // the potentiometer is connected to pin A1 of the arduino
int data = 0; 

void setup() {
  
  pinMode(mvm, OUTPUT); // vibration motor
  pinMode(vresistor, INPUT); // potentiometer  
  pinMode(2, OUTPUT);  // right LED
  pinMode(3, OUTPUT);  // middle LED
  pinMode(4, OUTPUT);  // left LED
 
}
 
void loop() {
  // put your main code here, to run repeatedly:
data = analogRead(vresistor); 
data = map(data, 0, 1023, 0,255);   

if (data<1100){ 
  digitalWrite(3, HIGH);  // the middle LED is always lit 
}

//when the data reaches 460, the left LED turns on and the motor will vibrate in a 250-750 tempo
data = analogRead(vresistor); 
if (data>460 && data<850){
  digitalWrite(4, HIGH);
  analogWrite(mvm, 255);  
  delay(250);
  analogWrite(mvm, 0);
  delay(750);
}

//when the data reaches 850, the right LED turns on and the motor will vibrate in a 250-250 tempo
data = analogRead(vresistor); 
if (data>850 && data<1100){
  digitalWrite(2, HIGH);
  analogWrite(mvm, 255);
  delay(250);
  analogWrite(mvm, 0);
  delay(250);
}

else{
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
    

}



}
