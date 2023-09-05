 #include <EEPROM.h>
String Allkey = "";
String StartKey = "";
String GStatus = "";
String RStatus = "";
String YStatus = "";
String GInt = "";
String RInt = "";
String YInt = "";

int GreenPin = 11;
int RedPin = 10;
int YellowPin = 12;

/***********1.Running**************/
int interval_Running_Green = 500;
int interval_Running_Red = 500;
int interval_Running_Yellow = 500;
String greenRunningMode = "ON";
String redRunningMode = "ON";
String yellowRunningMode = "ON";


/*************2.Idle**************/
int interval_Idle_Green = 500;
int interval_Idle_Red = 500;
int interval_Idle_Yellow = 500;
String greenIdleMode = "ON";
String redIdleMode = "ON";
String yellowIdleMode = "ON";

/*************3.Error**************/
int interval_Error_Green = 500;
int interval_Error_Red = 500;
int interval_Error_Yellow = 500;
String greenErrorMode = "ON";
String redErrorMode = "ON";
String yellowErrorMode = "ON";


/************4.Material**********/
int interval_Material_Green = 500;
int interval_Material_Red = 500;
int interval_Material_Yellow = 500;
String greenMaterialMode = "ON";
String redMaterialMode = "ON";
String yellowMaterialMode = "ON";

/************0.OFF**********/
int interval_OFF_Green = 500;
int interval_OFF_Red = 500;
int interval_OFF_Yellow = 500;
String greenOFFMode = "ON";
String redOFFMode = "ON";
String yellowOFFMode = "ON";

/***0.OFF***/
int selectFunction = 0;

bool greenON = false;
bool yellowON = false;
bool redON = false;

unsigned long Oldt1 = 0;
unsigned long Oldt2 = 0;
unsigned long Oldt3 = 0;


int i = 0;
bool debug = false;
String Statestatus = "";
void setup() 
{

  Serial.begin(115200);
  delay(1);
  ReadPinEEPROM();

  Serial.println("**********************************  Tower Light Control  ***************************************");
  Serial.println("*  ComPort := COM12                                                                            *");
  Serial.println("*  Function Tower Light := :MATERIAL, RUNNING, IDLE, ERROR, OFF                                *");
  Serial.println("*  Config := :MC:ON:ON:ON:500:500:500, MaterialConfig:SG,SR,SY,intG,intR,intY                  *");
  Serial.println("*  ModeConfig := :(MC,RC,IC,EC):(StatusG):(StatusR):(StatusY):(interG):(interR):(interY)       *");
  Serial.print("*  Default Pin Green := ");
  Serial.print(GreenPin);
  Serial.print(",Red := ");
  Serial.print(RedPin);
  Serial.print(",Yellow := ");
  Serial.print(YellowPin);
  Serial.println("; ChangePin := :CP:GPin:RPin:YPin; Value 0-13 *");
  Serial.println("************************************************************************************************\n\n");

  LoadConfig();
  pinMode(4,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  //attachInterrupt(0, set_state, RISING); //pin2
  attachInterrupt(0, set_state, FALLING); //pin2
  
  attachInterrupt(1, reset_state, HIGH); //pin3

  resetIO(); 
  selectFunction = 0;
}

void set_state()
{
  digitalWrite(4,HIGH);
  Serial.println("Interrupt : SET");
}
void reset_state()
{
  digitalWrite(4,LOW);
  Serial.println("Interrupt : RESET");
}

void serialEvent() 
{ 
  while(Serial.available()>0)
  {
      char A = (char)Serial.read();
      Allkey +=A; 

      if(A != ':'&& A!='\n')
      {
        switch(i)
        {
          case 1:
            StartKey += A;
            break;
          case 2:
            GStatus += A;
            break;
          case 3:
            RStatus += A;
            break;
          case 4:
            YStatus += A;
            break;
          case 5:
            GInt += A;
            break;
          case 6:
            RInt += A;
            break;
          case 7:
            YInt += A;
            break;
        }
      }
      
      if(A == ':')
      {
        i++;
      }
      if(A=='\n')
      {        
        Serial.print("COMMAND := "+Allkey);
        if(StartKey == "DEBUG")
        {
          debug = !debug;
          if(debug)Serial.println("Seleted DEBUG MODE.");
        }
        else if(StartKey == "RUNNING")
        {
          selectFunction = 1;
          resetIO();
          if(debug)Serial.println("Seleted "+StartKey+".");
        }
        else if(StartKey == "IDLE")
        {
          resetIO();
          selectFunction = 2;
          if(debug)Serial.println("Seleted "+StartKey+".");
        }
        else if(StartKey == "ERROR")
        {
          resetIO();
          selectFunction = 3;
          if(debug)Serial.println("Seleted "+StartKey+".");
        }
        else if(StartKey == "MATERIAL")
        {
          resetIO();
          selectFunction = 4;
          if(debug)Serial.println("Seleted "+StartKey+".");
        }
        else if(StartKey == "OFF")
        {
          resetIO();
          selectFunction = 0;
          if(debug)Serial.println("Seleted "+StartKey+".");
        }
        else if(StartKey == "RC")
        {
          interval_Running_Green = GInt.toInt();
          interval_Running_Red = RInt.toInt();
          interval_Running_Yellow = YInt.toInt();
          greenRunningMode = GStatus;
          redRunningMode = RStatus;
          yellowRunningMode = YStatus;
          
          SaveConfigStatusEEPROM(GStatus,10);
          SaveConfigStatusEEPROM(RStatus,11);
          SaveConfigStatusEEPROM(YStatus,12);
          SaveConfigIntervalEEPROM(interval_Running_Green,13);
          SaveConfigIntervalEEPROM(interval_Running_Red,14);
          SaveConfigIntervalEEPROM(interval_Running_Yellow,15);
          if(debug)Serial.println("Config RUNNING Complete. GMode := "+GStatus+" ,RMode := "+RStatus+" ,YMode := "+YStatus+" ,GIntval := "+GInt+" ,RIntval := "+RInt+" ,YIntval := "+YInt);
        }
        else if(StartKey == "IC")
        {
          interval_Idle_Green = GInt.toInt();
          interval_Idle_Red = RInt.toInt();
          interval_Idle_Yellow = YInt.toInt();
          greenIdleMode = GStatus;
          redIdleMode = RStatus;
          yellowIdleMode = YStatus;
          SaveConfigStatusEEPROM(GStatus,20);
          SaveConfigStatusEEPROM(RStatus,21);
          SaveConfigStatusEEPROM(YStatus,22);
          SaveConfigIntervalEEPROM(interval_Idle_Green,23);
          SaveConfigIntervalEEPROM(interval_Idle_Red,24);
          SaveConfigIntervalEEPROM(interval_Idle_Yellow,25);
          if(debug)Serial.println("Config IDLE Complete. GMode := "+GStatus+" ,RMode := "+RStatus+" ,YMode := "+YStatus+" ,GIntval := "+GInt+" ,RIntval := "+RInt+" ,YIntval := "+YInt);
        }
        else if(StartKey == "EC")
        {
          interval_Error_Green = GInt.toInt();
          interval_Error_Red = RInt.toInt();
          interval_Error_Yellow = YInt.toInt();
          greenErrorMode = GStatus;
          redErrorMode = RStatus;
          yellowErrorMode = YStatus;
          SaveConfigStatusEEPROM(GStatus,30);
          SaveConfigStatusEEPROM(RStatus,31);
          SaveConfigStatusEEPROM(YStatus,32);
          SaveConfigIntervalEEPROM(interval_Error_Green,33);
          SaveConfigIntervalEEPROM(interval_Error_Red,34);
          SaveConfigIntervalEEPROM(interval_Error_Yellow,35);
          if(debug)Serial.println("Config ERROR Complete. GMode := "+GStatus+" ,RMode := "+RStatus+" ,YMode := "+YStatus+" ,GIntval := "+GInt+" ,RIntval := "+RInt+" ,YIntval := "+YInt);
        }
        else if(StartKey == "MC")
        {
          interval_Material_Green = GInt.toInt();
          interval_Material_Red = RInt.toInt();
          interval_Material_Yellow = YInt.toInt();
          greenMaterialMode = GStatus;
          redMaterialMode = RStatus;
          yellowMaterialMode = YStatus;
          SaveConfigStatusEEPROM(GStatus,40);
          SaveConfigStatusEEPROM(RStatus,41);
          SaveConfigStatusEEPROM(YStatus,42);
          SaveConfigIntervalEEPROM(interval_Material_Green,43);
          SaveConfigIntervalEEPROM(interval_Material_Red,44);
          SaveConfigIntervalEEPROM(interval_Material_Yellow,45);
          if(debug)Serial.println("Config MATERIAL Complete. GMode := "+GStatus+" ,RMode := "+RStatus+" ,YMode := "+YStatus+" ,GIntval := "+GInt+" ,RIntval := "+RInt+" ,YIntval := "+YInt);
        }
        else if(StartKey == "OC")
        {
          interval_OFF_Green = GInt.toInt();
          interval_OFF_Red = RInt.toInt();
          interval_OFF_Yellow = YInt.toInt();
          greenOFFMode = GStatus;
          redOFFMode = RStatus;
          yellowOFFMode = YStatus;
          SaveConfigStatusEEPROM(GStatus,50);
          SaveConfigStatusEEPROM(RStatus,51);
          SaveConfigStatusEEPROM(YStatus,52);
          SaveConfigIntervalEEPROM(interval_OFF_Green,53);
          SaveConfigIntervalEEPROM(interval_OFF_Red,54);
          SaveConfigIntervalEEPROM(interval_OFF_Yellow,55);
          if(debug)Serial.println("Config MATERIAL Complete. GMode := "+GStatus+" ,RMode := "+RStatus+" ,YMode := "+YStatus+" ,GIntval := "+GInt+" ,RIntval := "+RInt+" ,YIntval := "+YInt);
        }
        else if(StartKey == "CP")
        {
          int nGP = GStatus.toInt();
          int nRP = RStatus.toInt();
          int nYP = YStatus.toInt();
          if(nGP >= 0 && nGP <= 13 && nRP >= 0 && nRP <= 13 && nYP >= 0 && nYP <= 13 && (nGP != nRP && nGP != nYP && nRP != nYP))
          {
            resetIO();
            GreenPin = nGP;
            RedPin = nRP;
            YellowPin = nYP;
            WritePinEEPROM();
            resetIO();
            if(debug)Serial.println("Change Pin Complete GPin = "+GStatus+",RPin = "+RStatus+",YPin = "+YStatus+".");
          }
        }
        
       
        StartKey = "";
        Allkey = "";
        StartKey = "";
        GStatus = "";
        RStatus = "";
        YStatus = "";
        GInt = "";
        RInt = "";
        YInt = "";
        i=0;
      }
  }
  Serial.flush();
}

void loop() 
{
 
 unsigned long currenttime = millis();
 if(Oldt1>currenttime || Oldt2>currenttime || Oldt3>currenttime)
 {
    Oldt1 = currenttime;
    Oldt2 = currenttime;
    Oldt3 = currenttime;  
 }

 switch(selectFunction)
 {
    case 0:
      if(debug)
      {
        if(Statestatus != "off") Serial.println("off");
        Statestatus = "off";
      }
      Control(greenOFFMode,yellowOFFMode,redOFFMode,interval_OFF_Green,interval_OFF_Yellow,interval_OFF_Red,currenttime);
      break;
    case 1:
      if(debug)
      {
        if(Statestatus != "Runninng") Serial.println("Runninng");
        Statestatus = "Runninng";
      }
      Control(greenRunningMode,yellowRunningMode,redRunningMode,interval_Running_Green,interval_Running_Yellow,interval_Running_Red,currenttime);
      break;
    case 2:
      if(debug)
      {
        if(Statestatus != "Idle") Serial.println("Idle");
        Statestatus = "Idle";
      }
      Control(greenIdleMode,yellowIdleMode,redIdleMode,interval_Idle_Green,interval_Idle_Yellow,interval_Idle_Red,currenttime);
      break;
    case 3:
      if(debug)
      {
        if(Statestatus != "Error") Serial.println("Error");
        Statestatus = "Error";
      }
      Control(greenErrorMode,yellowErrorMode,redErrorMode,interval_Error_Green,interval_Error_Yellow,interval_Error_Red,currenttime);
      break;
    case 4:
      if(debug)
      {
        if(Statestatus != "Material") Serial.println("Material");
        Statestatus = "Material";
      }
      Control(greenMaterialMode,yellowMaterialMode,redMaterialMode,interval_Material_Green,interval_Material_Yellow,interval_Material_Red,currenttime);
      break;
 }

}

void Control(String greenMode,String yellowMode,String redMode,int gInterval,int yInterval,int rInterval,unsigned long currenttime)
{
  /***green***/
  if(greenMode == "ON")
  {
    if(!greenON)
    {
      digitalWrite(GreenPin,!HIGH);
      greenON = true;
      if(debug)Serial.println("Green ON");
    }
    
  }
  else if(greenMode == "OFF")
  {
    if(greenON)
    {
      digitalWrite(GreenPin,!LOW);
      greenON = false;
      if(debug)Serial.println("Green OFF");
    }
  }
  else
  {
    if((currenttime-Oldt1)>=gInterval)
    {
      Oldt1 = currenttime;
      if(greenON)
      {
        digitalWrite(GreenPin,!LOW);
        greenON = false;
        if(debug)Serial.println("Green OFF");
      }
      else
      {
        digitalWrite(GreenPin,!HIGH);
        greenON = true;
        if(debug)Serial.println("Green ON");
      }
    }
  }

  /***yellow***/
  if(yellowMode == "ON")
  {
    if(!yellowON)
    {
      digitalWrite(YellowPin,!HIGH);
      yellowON = true;
      if(debug)Serial.println("Yellow ON");
    }
  }
  else if(yellowMode == "OFF")
  {
    if(yellowON)
    {
      digitalWrite(YellowPin,!LOW);
      yellowON = false;
      if(debug)Serial.println("Yellow OFF");
    }
  }
  else
  {
    if((currenttime-Oldt2)>=yInterval)
    {
      Oldt2 = currenttime;
      if(yellowON)
      {
        digitalWrite(YellowPin,!LOW);
        yellowON = false;
        if(debug)Serial.println("Yellow OFF");
      }
      else
      {
         digitalWrite(YellowPin,!HIGH);
         yellowON = true;
         if(debug)Serial.println("Yellow ON");
      }
    }
  }


/**red**/
  if(redMode == "ON")
  {
    if(!redON)
    {
      digitalWrite(RedPin,!HIGH);
      redON = true;
      if(debug)Serial.println("Red ON");
    }
  }
  else if(redMode == "OFF")
  {
    if(redON)
    {
      digitalWrite(RedPin,!LOW);
      redON = false;
      if(debug)Serial.println("Red OFF");
    }
  }
  else
  {
    if((currenttime-Oldt3)>=rInterval)
    {
      Oldt3 = currenttime;
      if(redON)
      {
        digitalWrite(RedPin,!LOW);
        redON = false;
        if(debug)Serial.println("Red OFF");
      }
      else
      {
         digitalWrite(RedPin,!HIGH);
         redON = true;
         if(debug)Serial.println("Red ON");
      }
    }
  }

  
}

void Off()
{
  if(!yellowON)
  {
    digitalWrite(YellowPin,!HIGH);
    yellowON = true;
  }
}

void resetIO()
{
  pinMode(GreenPin,OUTPUT);
  pinMode(RedPin,OUTPUT);
  pinMode(YellowPin,OUTPUT);
  digitalWrite(GreenPin,!LOW);
  digitalWrite(RedPin,!LOW);
  digitalWrite(YellowPin,!LOW);
  greenON = false;
  redON = false;
  yellowON = false;
}
void ReadPinEEPROM()
{
  GreenPin = EEPROM.read(0);
  RedPin = EEPROM.read(1);
  YellowPin = EEPROM.read(2);
  if(GreenPin == RedPin || GreenPin == YellowPin || RedPin == YellowPin || GreenPin > 13 || GreenPin < 0 || RedPin > 13 || RedPin < 0 || YellowPin > 13 || YellowPin < 0)
  {
    for (int i = 0 ; i <= 2 ; i++) 
    {
        EEPROM.write(i, 0);
    }
    GreenPin = 11;
    RedPin = 10;
    YellowPin = 12;
    EEPROM.write(0, GreenPin);
    EEPROM.write(1, RedPin);
    EEPROM.write(2, YellowPin);
    GreenPin = EEPROM.read(0);
    RedPin = EEPROM.read(1);
    YellowPin = EEPROM.read(2);
   
  }
}
void WritePinEEPROM()
{
    if(EEPROM.read(0) != GreenPin)
    {
      EEPROM.write(0, GreenPin);
    }
    if(EEPROM.read(1) != RedPin)
    {
      EEPROM.write(1, RedPin);
    }
    if(EEPROM.read(2) != YellowPin)
    {
      EEPROM.write(2, YellowPin);
    }
    GreenPin = EEPROM.read(0);
    RedPin = EEPROM.read(1);
    YellowPin = EEPROM.read(2);
}


void LoadConfig()
{
  ReadConfigStatusEEPROM();
  ReadConfigIntervalEEPROM();
}







void ReadConfigStatusEEPROM()
{
  //running
  greenRunningMode = ReadConfigStatusEEPROM(10);
  redRunningMode = ReadConfigStatusEEPROM(11);
  yellowRunningMode = ReadConfigStatusEEPROM(12);

  //idle
  greenIdleMode = ReadConfigStatusEEPROM(20);
  redIdleMode = ReadConfigStatusEEPROM(21);
  yellowIdleMode = ReadConfigStatusEEPROM(22);

  //error
  greenErrorMode = ReadConfigStatusEEPROM(30);
  redErrorMode = ReadConfigStatusEEPROM(31);
  yellowErrorMode = ReadConfigStatusEEPROM(32);

  //material
  greenMaterialMode = ReadConfigStatusEEPROM(40);
  redMaterialMode = ReadConfigStatusEEPROM(41);
  yellowMaterialMode = ReadConfigStatusEEPROM(42);

  //off
  greenOFFMode = ReadConfigStatusEEPROM(50);
  redOFFMode = ReadConfigStatusEEPROM(51);
  yellowOFFMode = ReadConfigStatusEEPROM(52);
}

void ReadConfigIntervalEEPROM()
{
  //running
  interval_Running_Green = ReadConfigIntervalEEPROM(13);
  interval_Running_Red = ReadConfigIntervalEEPROM(14);
  interval_Running_Yellow = ReadConfigIntervalEEPROM(15);

  //idle
  interval_Idle_Green = ReadConfigIntervalEEPROM(23);
  interval_Idle_Red = ReadConfigIntervalEEPROM(24);
  interval_Idle_Yellow = ReadConfigIntervalEEPROM(25);

  //error
  interval_Error_Green = ReadConfigIntervalEEPROM(33);
  interval_Error_Red = ReadConfigIntervalEEPROM(34);
  interval_Error_Yellow =  ReadConfigIntervalEEPROM(35);

  //material
  interval_Material_Green = ReadConfigIntervalEEPROM(43);
  interval_Material_Red = ReadConfigIntervalEEPROM(44);
  interval_Material_Yellow = ReadConfigIntervalEEPROM(45);

  //off
  interval_OFF_Green = ReadConfigIntervalEEPROM(53);
  interval_OFF_Red = ReadConfigIntervalEEPROM(54);
  interval_OFF_Yellow = ReadConfigIntervalEEPROM(55);
}


String ReadConfigStatusEEPROM(int address)
{
  /* "ON"    Val 99
   * "OFF"   Val 98
   * "Blink" Val 97
   * Interval = RealVal/10
   * 
   * RUNNING  ADD 10-15
   * IDLE     ADD 20-25
   * ERROR    ADD 30-35
   * MATERIAL ADD 40-45
  
  */
  String statusLamp = "ON";
  int a = EEPROM.read(address); //Gstatus
  //Serial.println(a);
  switch(EEPROM.read(address))
  {
    case 99:
      statusLamp = "ON";
    break;
    case 98:
      statusLamp = "OFF";
    break;
    case 97:
      statusLamp = "BLINK";
    break;
    default:
      statusLamp = "ON";
      EEPROM.write(address, 99);
    break;
  }
  if(debug)
  {
    Serial.print(statusLamp+" , ");
    Serial.println(address);
  }
  return statusLamp;
}

int ReadConfigIntervalEEPROM(int address)
{
  int val = 500;
  val = EEPROM.read(address)*10;
  if(debug)
  {
    Serial.print(val);
    Serial.print(" , ");
    Serial.println(address);
  }
  return val;
}

void SaveConfigStatusEEPROM(String Val,int address)
{
  if(Val == "ON")
  {
    if(EEPROM.read(address) != 99)
    {
      EEPROM.write(address, 99);
    }
  }
  else if(Val == "OFF")
  {
    if(EEPROM.read(address) != 98)
    {
      EEPROM.write(address, 98);
    }
  }
  else
  {
    if(EEPROM.read(address) != 97)
    {
      EEPROM.write(address, 97);
    }
  }
}

void SaveConfigIntervalEEPROM(int Val,int address)
{
  int a = EEPROM.read(address);
  if((int)(Val/10) != a)
  {
    EEPROM.write(address, (int)(Val/10));
  }
}
