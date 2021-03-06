#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/wdt.h>
//-----Hardware Adressierung-----
//Bei falscher Funktion bitte obere Zeile auskommentieren,
//und untere Zeile freigeben
LiquidCrystal_I2C lcd(0x27,20,4);
//LiquidCrystal_I2C lcd(0x3F,20,4);

const int maxPlayers=50;
const int buttonPin[3] = {9, 10, 11};

char* scrStart[]={"^___^  Willkommen zu","<>_<>  Die Werwoelfe"," V V        vom"," -w-    Duesterwald"};
char* scrDiv[]={"Let's play a game!", " Randomization done"};
char* scrCmd[]={"Back     OK     Next","Reset   Again    New","                 New","  -     Next     +  ","Finish    View_Roles"," Back       Next", "Back    Deal    Next"};

char* roles[] = {"Werwolf", "Amor", "Baerenhueter", "Hauptmann", "Hexe", "Jaeger", "Seherin", "Wildes Kind", "Spieler", "Dorfbewohner"};  
int numbers[] = {1,0,0,0,0,0,0,0,3,0};
char* buttons[] = {"Back","OK","Next"};
int playerCards[maxPlayers];
int playerCardsAssigned[maxPlayers];
bool enButton[]={0,0,0};
bool lastGame = 0;
int finish=0;


void setup()
{

  Serial.begin(9600);
  for (int i=0; i<maxPlayers; i++)
  {
    playerCards[i]=-1;
    playerCardsAssigned[i]=-1;
  }
  delay(10000);
  Serial.print("Cards Initial: ");
  for (int i; i<maxPlayers;i++)
  {
    Serial.print(playerCards[i]);
  }
  Serial.println("");
  Serial.print("Assigned Initial: ");
  for (int i; i<maxPlayers; i++)
  {
    Serial.print(playerCardsAssigned[i]);
  }
  Serial.println("");
  
  for (int i=0; i<3; i++)
  {
    pinMode(buttonPin[i], INPUT);
  }
  
  lcd.init();
  delay(5000);
  lcd.backlight();
  WelcomeScreen();
}

void loop() {
  StartScreen();
  //catch Button
  switch (catchButton())
  {
    case 1:
      //Reset
      delay(100);
      softwareReset( WDTO_60MS);
      break;
    case 2:
      //Again
      Serial.println("Button 2 Pressed - resume Game");
      delay(100);
      break;
    case 3:
      //New
      enterNumbers();
      Serial.println("Button 3 Pressed - start new Game");
      
      delay(100);
      break;
  }
  
  //randomize roles
  randRoles();
  
  //karten austeilen
  dealCards();
  //Finish oder Rollen Anzeigen
  
  
  finish=0;
  while (finish==0)
  {
    switch (catchButton())
      {
        case 1: //Finish
        finish=1;
        break;
        case 3: //Review
        dealCards();
        break;
      }
  }
}

void dealCards()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(scrDiv[1]);
  lcd.setCursor(0,3);
  lcd.print(scrCmd[6]);
  for (int i=0; i<numbers[8]; i++)
  {
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("Player ");
    lcd.print(i+1);
    lcd.print(" of ");
    lcd.print(numbers[8]);
    lcd.setCursor(0,2);
    lcd.print("####################");
    setEnButton(1,1,1);
    switch (catchButton())
    {
      case 1: //Previous Player
      if (i>0)
      {
        i=i-2;
      }
      else
      {
        i--;
      }
      break;
      case 2: //Show Card
      lcd.setCursor(1,2);
      lcd.print(roles[playerCardsAssigned[i]]);
      delay(3000);
      break;
      case 3: //Next Player
      break;
      
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Finish this game or");
  lcd.setCursor(0,1);
  lcd.print("   review Roles?");
  lcd.setCursor(0,3);
  lcd.print(scrCmd[4]);
  setEnButton(1,0,1);
}

void randRoles()
{
  int cardsLen = 0;
  for (int i=0; i<10;i++)
  {
    if (i == 8)
    {
      i++;
    }
    for (int j=0; j<numbers[i]; j++)
    {
      playerCards[cardsLen]=i;
      cardsLen++;
    }
  }
  Serial.print("Cards: ");
  for (int i=0; i<cardsLen; i++)
  {
    Serial.print(playerCards[i]);  
  }
  Serial.println("");
  for (int i=0; i<cardsLen; i++)
  {
    int found = 0;
    while (found == 0)
    {
      int current = random(0,cardsLen);
      if (playerCards[current] != -2)
      {
        playerCardsAssigned[i] = playerCards[current];
        playerCards[current] = -2;
        found = 1;
      }
    }
    for (int i=0; i<cardsLen; i++)
    {
      Serial.print(playerCardsAssigned[i]);
    }
    Serial.println("");
  }
}

void enterNumbers()
{
  int numPlayers = 0;
  //cycle through roles
  for (int i=0; i<9; i++)
  {
    int next = 0;
    if(i == 8)
    {
      numbers[i] = numPlayers;
    }
    int min = numbers[i];
    while (next == 0)
    {
      lcd.clear();
      lcd.setCursor(1,1);
      lcd.print(roles[i]);
      lcd.setCursor(18,1);
      lcd.print(numbers[i]);
      lcd.setCursor(0,3);
      lcd.print(scrCmd[3]);
      setEnButton(1,1,1);
      switch(catchButton())
      {
        case 1:
          //Minus
          if(numbers[i]>min)
          {
            numbers[i]--;
          }
          break;
        case 2:
          //Next
          next=1;
          if(i == 8)
          {
            numbers[i+1]=numbers[i]-numPlayers;
          }
          numPlayers+=numbers[i];
          break;
        case 3:
          //Plus
          numbers[i]++;
          break;
      }
    }
  }
  lastGame=1;
}

int catchButton()
{
  int buttonPressed=0;
  int buttonState[]={0,0,0};
  int buttonLastState[]={digitalRead(buttonPin[0]),digitalRead(buttonPin[1]),digitalRead(buttonPin[2])};
  
  while (1==1)
  {
    for (int i=0; i<3; i++)
    {
      if(enButton[i])
      {
        buttonState[i] = digitalRead(buttonPin[i]);
        if ((buttonState[i] != buttonLastState[i]) && buttonState[i] == 1)
        {
          return (i+1);
        }
        buttonLastState[i]=buttonState[i];
      }
    }
    delay(50);
  }
}

void StartScreen()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(scrDiv[0]);
  lcd.setCursor(0,3);
  if(lastGame == 1)
  {
    lcd.print(scrCmd[1]);
    setEnButton(1,1,1);
  }
  else
  {
    lcd.print(scrCmd[2]);
    setEnButton(0,0,1);
  }
}

void WelcomeScreen()
{
  //Welcome Screen
  lcd.clear();
  
  for (int i=0; i<4; i++)
  {
    lcd.setCursor(0,i);
    lcd.print(scrStart[i]);
  }
  setEnButton(1,1,1);
  catchButton();
}

void setEnButton(int val1, int val2, int val3)
{
  enButton[0]=val1;
  enButton[1]=val2;
  enButton[2]=val3;  
}

void softwareReset( uint8_t prescaller) {
  // start watchdog with the provided prescaller
  wdt_enable( prescaller);
  // wait for the prescaller time to expire
  // without sending the reset signal by using
  // the wdt_reset() method
  while(1) {}
}