
#define buffc 10
char s0inbuff[550];
char s1inbuff[buffc][550];

bool AUX = false;
bool change = false;
int mode = 0;

int curwbuffp = 0;
int currbuffp = 0;
int fbuffc = 0;

bool overflow = false;
bool bufffull = false;
bool ofs = true;

void MISR()
{
  AUX = digitalRead(PA0);
  delay(1);
  change = true;
}



int modeChange(int i)
{
  if (i == 0)
  {
    digitalWrite(PA1, 0);  // m0
    digitalWrite(PC15, 0);  // m1
    mode = 0;
    char cm[] {"#MS,M0|\r\n"};
    Serial.write(cm, sizeof(cm));
    return i;
  }
  if (i == 1)
  {
    digitalWrite(PA1, 0);
    digitalWrite(PC15, 1);
    mode = 1;
    char cm[] {"#MS,M1|\r\n"};
    Serial.write(cm, sizeof(cm));
    return i;
  }
  if (i == 2)
  {
    digitalWrite(PA1, 1);
    digitalWrite(PC15, 0);
    char cm[] {"#MS,M2|\r\n"};
    Serial.write(cm, sizeof(cm));
    mode = 2;
    return i;
  }
  if (i == 3)
  {
    digitalWrite(PA1, 1);
    digitalWrite(PC15, 1);
    char cm[] {"#MS,M3|\r\n"};
    Serial.write(cm, sizeof(cm));
    mode = 3;
    return i;
  }
  return -1;
}









int process(char* str, int len)
{
  char* chp = NULL;
  chp = strtok(str, ",");
  if (chp == NULL) return -1;
  else
  {
    if (strcmp(chp, "R") == 0)
    {
      chp = NULL;
      chp = strtok(NULL, ",");
      if (chp == NULL) return -1;
      else
      {
        if (strcmp(chp, "O") == 0)
        {
          if (strlen(s1inbuff[currbuffp]) == 0)
          {
            char eb[] {"#MS,DE|\r\n"};
            Serial.write(eb, sizeof(eb));
            return 1;
          }
          else
          {
            char dh[] {'#', 'D', ','};
            char sh[] {'#', 'S', ','};

            if (s1inbuff[currbuffp][0] == 'D')
              Serial.write(dh, sizeof(dh));
            if (s1inbuff[currbuffp][0] == 'S')
              Serial.write(sh, sizeof(sh));
            --fbuffc;

            if (s1inbuff[currbuffp][0] == 'D')
            {
              Serial.write(&s1inbuff[currbuffp][1], strlen(s1inbuff[currbuffp]) - 1);
              Serial.write('|');
              Serial.write('\r'); Serial.write('\n'); Serial.write('\0');
            }
            if (s1inbuff[currbuffp][0] == 'S')
            {
              Serial.write(&s1inbuff[currbuffp][1], 10);
              Serial.write('|');
              Serial.write('\r'); Serial.write('\n'); Serial.write('\0');
            }

            memset(s1inbuff[currbuffp], '\0', sizeof(s1inbuff[currbuffp]));
            (++currbuffp) %= buffc;
            overflow = false;
            bufffull = false;
            return 1;
          }
        }

        if (strcmp(chp, "A") == 0)
        {
          while (1)
          {
            if (strlen(s1inbuff[currbuffp]) == 0)
            {
              char eb[] {"#MS,DE|\r\n"};
              Serial.write(eb, sizeof(eb));
              return 1;
            }
            else
            {
              char dh[] {'#', 'D', ','};
              char sh[] {'#', 'S', ','};

              if (s1inbuff[currbuffp][0] == 'D')
                Serial.write(dh, sizeof(dh));
              if (s1inbuff[currbuffp][0] == 'S')
                Serial.write(sh, sizeof(sh));
              --fbuffc;

              if (s1inbuff[currbuffp][0] == 'D')
              {
                Serial.write(&s1inbuff[currbuffp][1], strlen(s1inbuff[currbuffp]) - 1);
                Serial.write('|');
                Serial.write('\r'); Serial.write('\n'); Serial.write('\0');
              }
              if (s1inbuff[currbuffp][0] == 'S')
              {
                Serial.write(&s1inbuff[currbuffp][1], 10);
                Serial.write('|');
                Serial.write('\r'); Serial.write('\n'); Serial.write('\0');
              }

              memset(s1inbuff[currbuffp], '\0', sizeof(s1inbuff[currbuffp]));
              (++currbuffp) %= buffc;
              overflow = false;
              bufffull = false;
            }
          }
        }
      }
    }

    if (strcmp(chp, "CM") == 0)
    {
      chp = NULL;
      chp = strtok(NULL, ",");
      if (chp == NULL) return -1;
      else
      {
        if (strcmp(chp, "M0") == 0)
        {
          modeChange(0);
          return 1;
        }
        if (strcmp(chp, "M1") == 0)
        {
          modeChange(1);
          return 1;
        }
        if (strcmp(chp, "M2") == 0)
        {
          modeChange(2);
          return 1;
        }
        if (strcmp(chp, "M3") == 0)
        {
          modeChange(3);
          return 1;
        }
      }
      return -1;
    }

    if (strcmp(chp, "CS") == 0)
    {
      chp = NULL;
      chp = strtok(NULL, ",");
      if (chp == NULL) return -1;
      AUX = digitalRead(PA0);
      if (AUX == 0)
      {
        char al[] {"#MS,AL|\r\n"};
        Serial.write(al, sizeof(al));
        return 1;
      }
      if (mode == 3)
      {
        Serial1.write(chp, strlen(chp)); ///
        return 1;
      }
      else
      {
        char wm[] {"#MS,WM|\r\n"};
        Serial.write(wm, sizeof(wm));
        return 1;
      }
    }

    if (strcmp(chp, "G") == 0)
    {
      chp = NULL;
      chp = strtok(NULL, ",");
      if (chp == NULL) return -1;
      else
      {
        if (strcmp(chp, "AUX") == 0)
        {
          AUX = digitalRead(PA0);
          if (AUX == 0)
          {
            char as[] {"#MS,AL|\r\n"};
            Serial.write(as, sizeof(as));
          }
          if (AUX == 1)
          {
            char as[] {"#MS,AH|\r\n"};
            Serial.write(as, sizeof(as));
          }
          return 1;
        }
        if (strcmp(chp, "MD") == 0)
        {
          modeChange(mode);
          return 1;
        }
        if (strcmp(chp, "B") == 0)
        {
          char bc[20] {};
          sprintf(bc, "#MS,B%d|\r\n", fbuffc); ///!!!
          Serial.write(bc, sizeof(bc));
          return 1;
        }
      }
    }
  }
  return -1;
}
















void setup() {
  memset(s1inbuff[0], '\0', sizeof(s1inbuff[0]));
  memset(s1inbuff[1], '\0', sizeof(s1inbuff[1]));
  memset(s1inbuff[2], '\0', sizeof(s1inbuff[2]));
  memset(s1inbuff[3], '\0', sizeof(s1inbuff[3]));
  memset(s1inbuff[4], '\0', sizeof(s1inbuff[4]));
  memset(s1inbuff[5], '\0', sizeof(s1inbuff[5]));
  memset(s1inbuff[6], '\0', sizeof(s1inbuff[6]));
  memset(s1inbuff[7], '\0', sizeof(s1inbuff[7]));
  memset(s1inbuff[8], '\0', sizeof(s1inbuff[8]));
  memset(s1inbuff[9], '\0', sizeof(s1inbuff[9]));

  Serial.begin(500000, SERIAL_8N1);
  Serial1.begin(9600, SERIAL_8N1);
  Serial.setTimeout(200);
  Serial1.setTimeout(200);



  pinMode(PA0, INPUT); // aux
  pinMode(PA1, OUTPUT); // aux
  pinMode(PC15, OUTPUT); // aux

  while (!Serial);
  char on[] {"#MS,O|\r\n"};
  Serial.write(on, sizeof(on));
  while (1)
  {
    //  wdt_reset();
    if (Serial.available() > 0)
    {
      char ch = 'X';
      Serial.readBytes(&ch, 1);
      if (ch == 'S')
      {
        ch = 'X';
        Serial.readBytes(&ch, 1);
        if (ch == 'S')
          break;
      }
    }
  }
  char strt[] {"#MS,S|\r\n"};
  Serial.write(strt, sizeof(strt));
  modeChange(0);

  attachInterrupt(digitalPinToInterrupt(PA0), MISR, CHANGE);
  AUX = digitalRead(PA0);
}














void loop() {

  if (change)
  {
    if (AUX == 0)
    {
      char af[] {"#MS,AF|\r\n"};
      Serial.write(af, sizeof(af));
    }
    if (AUX == 1)
    {
      char ar[] {"#MS,AR|\r\n"};
      Serial.write(ar, sizeof(ar));
    }
    change = false;
  }


  if (Serial.available() > 0)
  {
    char ch = ' ';
    ch = Serial.read();
    if (ch == '#')
    {
      memset(s0inbuff, 0, sizeof(s0inbuff));
      int len = Serial.readBytesUntil('|', s0inbuff, sizeof(s0inbuff) - 1);
      s0inbuff[len] = '\0';
      if (len > 0)
      {
        process(s0inbuff, strlen(s0inbuff));
      }
    }

    if (ch == '$')
    {

      char addch[3]{};

      Serial.readBytes(addch,3);
      int len = Serial.readBytesUntil('\0', s0inbuff, sizeof(s0inbuff) - 1);
      if (len != 0)
      {
        if (mode == 3 || mode == 2)
        {
          char wm[] {"#MS,WM|\r\n"};
          Serial.write(wm, sizeof(wm));
        }
        else
        {
          AUX = digitalRead(PA0);
          if (AUX == 0)
          {
            char al[] {"#MS,AL|\r\n"};
            Serial.write(al, sizeof(al));
          }
          else
          {
            Serial1.write(addch, sizeof(addch)); ///
            Serial1.write(s0inbuff, strlen(s0inbuff)); ///
            memset(s0inbuff, 0, sizeof(s0inbuff));
            char td[] {"#MS,TD|\r\n"};
            Serial.write(td, sizeof(td));
          }
        }
      }
    }

    if (ch == '@')
    {
      memset(s0inbuff, 0, sizeof(s0inbuff));
      int len = Serial.readBytes(s0inbuff, 6);
      if (len > 0)
      {
        Serial1.write(s0inbuff, 6);
        memset(s0inbuff, 0, sizeof(s0inbuff));
        char scd[] {"#MS,SCD|\r\n"};
        Serial.write(scd, sizeof(scd));
      }
    }
  }


  if (Serial1.available() > 0)
  {
    if (bufffull)
    {
      overflow = true;
      if (ofs)
      {
        char of[] {"#MS,OF|\r\n"};
        Serial.write(of, sizeof(of));
        ofs = false;
      }
    }
    else
    {
      ofs = true;
      memset(s1inbuff[curwbuffp], 0, sizeof(s1inbuff[curwbuffp]));
      char nd[] {"#MS,ND|\r\n"};

      if (mode == 3)
      {
        s1inbuff[curwbuffp][0] = 'S';
        memset(nd, 0, sizeof(nd));
        strcpy(nd, "#MS,NS|\r\n");
      }
      else
      {
        s1inbuff[curwbuffp][0] = 'D';
      }

      ++fbuffc;
      Serial1.readBytes(&s1inbuff[curwbuffp][1], sizeof(s1inbuff[curwbuffp]) - 1);
      curwbuffp = (curwbuffp + 1) % buffc;

      Serial.write(nd, sizeof(nd));

      if (strlen(s1inbuff[curwbuffp]) != 0)
      {
        bufffull = true;  /// false when
        char mf[] {"#MS,BF|\r\n"};
        Serial.write(mf, sizeof(mf));
      }
    }
  }
}
