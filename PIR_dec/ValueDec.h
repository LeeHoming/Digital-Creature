//Every step of detection
//Each step is 1 second
int micStep = 1000;

//How many mi-second in a second will trigger
//Higher number lower sensitive
int micStepTri = 30;

//Every Big step as sum of micro step
//Second as unit
int FactorStep = 20;
int NowSecond = 0;
float currentRound = 0;

int TriNum = 0;
int TriSec = 0;
//Thershold value def
//Unit: second

int T_s = 14;
int T_a = 9;
int T_b = 6;
int T_c = 1;

//index of publish
int N_s = 4, N_a = 3, N_b = 2, N_c = 1, N_n = 0;


void ResultPrinter(int n) {
  Serial.print("Send: ");
  Serial.println(n);
}

void TherCount(int n) {
  if (n > T_s) {
    //publish S
    if (! pub.publish(N_s)) {
      Serial.println(F("Failed_S"));
    } else {
      Serial.println(F("OK! S update"));
    }

  } else if (n > T_a) {
    //publish A
    if (! pub.publish(N_a)) {
      Serial.println(F("Failed A"));
    } else {
      Serial.println(F("OK! A update"));
    }

  } else if (n > T_b) {
    //publish B
    if (! pub.publish(N_b)) {
      Serial.println(F("Failed B"));
    } else {
      Serial.println(F("OK! B update"));
    }

  } else if (n > T_c) {
    //publish C
    if (! pub.publish(N_c)) {
      Serial.println(F("Failed C"));
    } else {
      Serial.println(F("OK! C update"));
    }
  } else {
    //publish N
    if (! pub.publish(N_n)) {
      Serial.println(F("Failed N"));
    } else {
      Serial.println(F("OK! N update"));
    }
  }
}



void MicroStepCounter() {
  //in each second
  //detect the ratio of sound in this second
  if (millis() < (NowSecond + 1) * micStep) {
    //accumulate the ratio till 1000
    if (millis()) {
      TriNum += val;
    }
  } else {
    //Define this second
    if (TriNum >= micStepTri) {
      TriSec ++;
    }

    //Debug
    Serial.print(NowSecond);
    Serial.print(" / ");
    Serial.println(TriNum);

    //accumulate second
    NowSecond ++;

    //clear ratio
    TriNum = 0;

  }

}


void SecondCounter() {
  MicroStepCounter();
  if (NowSecond < (currentRound + 1) * FactorStep) {
    //    oriNum += val;
  } else {
    currentRound ++;
    TherCount(TriSec);
    //Serial.println(oriNum);
    TriSec = 0;
  }
}
