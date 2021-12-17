//modified the total length for subscription
int SubValue[120];
int size0, size1, size2, size3, size4;

//when receive a new value, add to the Main list - SubValue
//call once when countNum
void addValue(int n) {
  for (byte i = 0; i < (sizeof(SubValue) / sizeof(SubValue[0])); i++) {
    if (i == (sizeof(SubValue) / sizeof(SubValue[0])) - 1) {
      //add new num at the end of array
      SubValue[i] = n;
    }
    else {
      //move number forward
      SubValue[i] = SubValue [i + 1];
    }
  }
}

//print the Main array - don't call often
void printValue() {
  Serial.print('{');
  for (byte i = 0; i < (sizeof(SubValue) / sizeof(SubValue[0])); i++) {
    Serial.print(SubValue[i]);
    Serial.print(',');
  }
  Serial.println('}');
}

//count the amount of each level
//call once a new value sub
void countNum(int n) {
  if (n == 0) {
    
    size0 ++;
  } else if (n == 1) {
    
    addValue(n);
    size1 ++;
    
  } else if (n == 2) {
    
    addValue(n);
    size2 ++;
    
  } else if (n == 3) {
    
    addValue(n);
    size3 ++;
    
  } else if (n == 4) {
    
    addValue(n);
    size4 ++;
    
  }
}

//clean main array to 0
//call once when decode done
void CleanArray() {
  
  for (byte i = 0; i < (sizeof(SubValue) / sizeof(SubValue[0])); i++) {
    SubValue[i] = 0;
  }

  //clean counter
  size0 = 0;
  size1 = 0;
  size2 = 0;
  size3 = 0;
  size4 = 0;
}
