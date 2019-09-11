
/*
    digitalRead(0) 
        1 - Nix 
        0 - Button ist gedrückt 

 */
bool interrupted = false;
void setup(){

    Serial.begin(9600);
    pinMode(0,INPUT);
    pinMode(7,OUTPUT);
    attachInterrupt(digitalPinToInterrupt(0), interrupt, CHANGE);
}

void loop(){

    if(!interrupted){
        digitalWrite(8,HIGH);
        delay(250);
        digitalWrite(8,LOW);
        delay(250);
    }

}

void interrupt(){
    Serial.print("Interrupting here");
    interrupted = false;
}