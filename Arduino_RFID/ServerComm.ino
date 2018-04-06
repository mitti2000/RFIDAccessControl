// send to Server
void sendToServer(String function, String data){
  String message = function + "_" + data;
  Serial.println(message);
}

// wait for Server Response
String waitForMessage(String function){
  String response;
  unsigned long now = millis();
  do{
    /*
    if(millis() - 5000 < now){
      return "fail";
    }
    */
    response = Serial.readString();    
  }
  while(response.length() == 0);
  if(response.startsWith(function + "_")){    
    int separator = response.indexOf(function + "_") + function.length() + 1;
    return response.substring(separator);
  }

  return "bullshit";
}


