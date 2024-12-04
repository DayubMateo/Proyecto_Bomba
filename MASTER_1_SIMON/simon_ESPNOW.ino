
void lightLedAndPlayTone(byte ledIndex) {
  Serial.print("indice: ");
  Serial.println(ledIndex);
  digitalWrite(ledPins[ledIndex], HIGH);
  Serial.print("pin: ");
  Serial.println(digitalRead(ledPins[ledIndex]));
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
}

void playSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);
    delay(100);
  }
}

byte readButtons() {
  while (1==1) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        Serial.print("se presiono: ");
        Serial.println(buttonPin);
        return i;        
      }
    }
    delay(1);
  }
}

bool checkUserSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton;
    if(FRK){
      if(gameSequence[i] == 0)
      expectedButton = 1;
      if(gameSequence[i] == 1)
      expectedButton = 2;
      if(gameSequence[i] == 2)
      expectedButton = 3;
      if(gameSequence[i] == 3)
      expectedButton = 0;
  } else{
    if(gameSequence[i] == 0)
      expectedButton = 2;
      if(gameSequence[i] == 1)
      expectedButton = 3;
      if(gameSequence[i] == 2)
      expectedButton = 0;
      if(gameSequence[i] == 3)
      expectedButton = 1;
  }                     
    byte actualButton = readButtons();                           
    lightLedAndPlayTone(actualButton);                           
    if (expectedButton != actualButton) {
      return false;
    }
  }

  return true;
}

void Perdiste (){
  myData.lose=true;
  Serial.println("perdiste");
  for(int i = 0 ; i < 4 ; i++)
  digitalWrite(ledPins[i], LOW);
}

void Ganaste(){
  Serial.println("ganaste");
  for(int i = 0 ; i < 4 ; i++)
  digitalWrite(ledPins[i], HIGH);
  myData.win=true;
}

void play_simon() {
  Serial.println("se accedio a simon");
  while(myData.win!=true && myData.lose!=true){
    randomSeed(millis());
    while(myData.win!=true && myData.lose!=true){
       // put your main code here, to run repeatedly:
      gameSequence[gameIndex] = random(4);                     //PONE UN LED A LA SECUENCIA
      gameIndex++;
      if (gameIndex >= MAX_GAME_LENGTH) {
        //poner variable booleana de ganar en true
         Ganaste();
      }
      if(myData.win!=true && myData.lose!=true){
        playSequence();                                             //MUESTRA LA SECUENCIA QUE SE HAYA GENERADO HASTA EL MOMENTO
        if (!checkUserSequence()) {                                 //COMPARA EL BOTON APRETADO CON EL BOTON ESPERADO
          //poner variable booleana de ganar en false
          Perdiste();
        }
      }
      delay(300);
    }
  }
}
