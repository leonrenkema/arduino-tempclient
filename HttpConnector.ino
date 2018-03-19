
void sendMessage(EthernetClient client, String temperaturePart) {
  client.println("POST /api/webhook/temperature HTTP/1.1");
  client.println("Host: solar.leonrenkema.nl");
  client.println("User-Agent: Arduino");
  client.println("Connection: close");
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + String(temperaturePart.length()));
  client.println();
  client.println(temperaturePart);  
}

