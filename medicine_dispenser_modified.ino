#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#define interruptPin 2 //Pin we are going to use to wake up the Arduino
#define wholenote 1666
#define PUSH_BUTTON 8
#include <DS3232RTC.h>  //RTC Library https://github.com/JChristensen/DS3232RTC
#define buzzer 11
int melody[] = {
  
  REST, 2, NOTE_D4, 4,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_C5, -2, 
  NOTE_A4, -2,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_F4, 2, NOTE_GS4, 4,
  NOTE_D4, -1, 
  NOTE_D4, 4,

  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4, //10
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_F5, 2, NOTE_E5, 4,
  NOTE_DS5, 2, NOTE_B4, 4,
  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
  NOTE_CS4, 2, NOTE_B4, 4,
  NOTE_G4, -1, 
};


//**********************************************************************************************************************************************
#define MAX_MEDICINES 5
#define MAX_ALARMS    5
int no_of_alarms = 5;
int no_of_meds = 5;
int all_alarms[MAX_ALARMS][3]={0}; //allows a maximum of 5 alarms in hr/mn/dd format

// 5 alarms + no of days left to eat medicine
// alarm0, alarm1, alarm2, alarm3, alarm4, #of meds left
int all_meds[MAX_MEDICINES][MAX_ALARMS + 1] = 
    {{1,1,1,1,1,3},\
     {1,1,1,1,1,2},\
     {1,0,1,0,1,1},\
     {1,0,1,0,1,2},\
     {1,1,1,1,1,4}\
    }; 
const char* med_names[MAX_MEDICINES] = {"Paracetamol", "Zico", "Azithral", "Diuril", "Ocimum Sanctum"};
    
int current_alarm= 0;    // alarm that is set or is being set
int LEDs[MAX_MEDICINES]={3,4,5,6,7};


void set_next_alarm();
void Going_To_Sleep();
void wakeUp();
void indication();
void delete_alarm( int ); // you have to pass actual alarm number 0-5
void startup_code();
int any_meds_left();

void setup() {
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);  
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);  //nonsleep mode
  
  pinMode(interruptPin,INPUT_PULLUP);//Set pin d2 to input using the buildin pullup resistor
  for (int i=0; i < no_of_meds; i++ ) 
     pinMode(LEDs[i], OUTPUT);
     
  RTC.begin();
  startup_code();
}

void startup_code()
{
  Serial.println("initialising the alarms to known values, clear alarm flags and alarm interrupt flags");  
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.squareWave(SQWAVE_NONE);
  
  for (int i=0; i < no_of_meds; i++ ) 
    digitalWrite(LEDs[i], HIGH);
  delay(1000);
  for (int i=0; i < no_of_meds; i++ ) 
    digitalWrite(LEDs[i], LOW);
  
  time_t t= RTC.get(); //create a temporary time variable so we can set the time and read the time from the RTC
  t = RTC.get();//Gets the current time of the RTC
  Serial.println("Checking the time");
  Serial.println("Current Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//Prints time stamp
  for (int i=0 ; i<MAX_ALARMS ; i++)
  {
    all_alarms[i][0]=hour(t);
    all_alarms[i][1]=minute(t) + i + 1;
    if (all_alarms[i][1] > 59) {
      all_alarms[i][1] -= 60;
      all_alarms[i][0] += 1;
      if (all_alarms[i][0] > 23) {
        all_alarms[i][0] = 0;
      }
    }
  }
  
  Serial.println("The following Alarms have been set:");
  for (int i=0 ; i< no_of_alarms ; i++) {
     Serial.println("Alarm "+ String(i+1) + " : Set at " + String(all_alarms[i][0]) + " hrs " + String(all_alarms[i][1]) + " mins");
  }
     
  set_next_alarm();

}

void set_next_alarm()
{
  RTC.setAlarm(ALM1_MATCH_HOURS, 0, all_alarms[current_alarm][1], all_alarms[current_alarm][0], 0);  //ALM1_MATCH_HOURS = 0x08 : match hours *and* minutes, seconds
  // clear the alarm flag If the alarm was triggered, returns true and resets the alarm flag in the RTC, else returns false.
  RTC.alarm(ALARM_1);
  RTC.squareWave(SQWAVE_NONE);
  RTC.alarmInterrupt(ALARM_1, true);  //enable interrupt output
  Serial.println("Upcoming Alarm " + String(current_alarm+1) + " : Set at " + String(all_alarms[current_alarm][0]) + " hrs " + String(all_alarms[current_alarm][1]) + " mins");
  
}

void Going_To_Sleep()
{
    sleep_enable();                  //Enabling sleep mode
    attachInterrupt(0, wakeUp, LOW);//attaching a interrupt to pin d2
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sleep
    digitalWrite(LED_BUILTIN,LOW);  //turning LED off
    time_t t;                       // creates temp time variable
    t=RTC.get();                    //gets current time from rtc
    Serial.println("Sleep  Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//prints time stamp on serial monitor
    delay(200); //wait 1sec for LED-->LOW  before going to sleep
    sleep_cpu();//activating sleep mode
    
    Serial.println("Just woke up!");//next line of code executed after the interrupt 
    t=RTC.get();
    Serial.println("WakeUp Time: "+String(hour(t))+":"+String(minute(t))+":"+String(second(t)));//Prints time stamp 

    //to check if any medicines are there for the current alarm
    int flag=0;
    for (int j =0 ; j< no_of_meds ; j++)
      if (all_meds[j][current_alarm]==1){
        flag=1;break;
      }
    if (flag==1)
      indication();   //buzzer + LED 
    //changing alarm number
    current_alarm++;
    if (current_alarm >= no_of_alarms)
      current_alarm=0;
    Serial.println("Alarm set for " + String(all_alarms[current_alarm][0]) + " hrs " + String(all_alarms[current_alarm][1]) + " mins" );
 
    // Set New Alarm
    set_next_alarm();
}


void wakeUp()
{
  sleep_disable();//Disable sleep mode
  detachInterrupt(0); //Removes the interrupt from pin 2;
  digitalWrite(LED_BUILTIN, HIGH);
}

void buzzer_play()
{
  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < 31 * 2 && (digitalRead(PUSH_BUTTON)) == HIGH ; thisNote = thisNote + 2) 
  {
    divider = melody[thisNote + 1];
    if (divider > 0)
      noteDuration = wholenote / divider; 
    else if (divider < 0) 
    {
      noteDuration = wholenote / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration*0.9);
    delay(noteDuration);
    noTone(buzzer);
    if(thisNote>=31*2)
      thisNote=0;
 }
} 

void lights_off()
{
  Serial.println("\nTake medicine and press button");
  delay(3000);
  int flag=1;
  while (flag = digitalRead(PUSH_BUTTON)) {
    // Serial.println(String(flag));
    flag = 0;
    digitalWrite(LED_BUILTIN, 1);
    delay(250);   
    digitalWrite(LED_BUILTIN, 0);
    delay(250);
  }
      
      
  Serial.println("Well done! All medicines have been taken.");
  for(int i=0 ; i < no_of_meds ; i++)
    if (all_meds[i][current_alarm]==1) 
    {
      digitalWrite(LEDs[i], LOW);
      // The last entry is for # of medicines left
      all_meds[i][MAX_ALARMS]--;
      Serial.println("Number of " + String(med_names[i]) + " left are: " + String(all_meds[i][MAX_ALARMS]));
      if (all_meds[i][MAX_ALARMS] == 0)
        for (int j=0 ; j<no_of_alarms ; j++)
            all_meds[i][j]=0;
     }
}

void indication()
{
  int i=0;
  Serial.println("Time to have medicine (" + String(current_alarm) + ")");
  for (i=0 ; i < no_of_meds ; i++)
    if (all_meds[i][current_alarm]==1) 
      digitalWrite(LEDs[i], HIGH);
      
  while(digitalRead(PUSH_BUTTON)==HIGH)
    buzzer_play();
    
  Serial.println("Buzzer Off");

  Serial.print("Please take the following medicines:\n");
  for (i=0 ; i < no_of_meds ; i++)
    if (all_meds[i][current_alarm]==1) {
      Serial.print(String(med_names[i]) + " (box " + String(i) + "), ");
    }
    
  lights_off();
  
}


void loop()
{
  Going_To_Sleep();
}
