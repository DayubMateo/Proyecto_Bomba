#include <esp_now.h>
#include <WiFi.h>

#define MAX_GAME_LENGTH 6

const uint8_t ledPins[] = { 17, 5, 18, 19 }; //simon dice
const uint8_t buttonPins[] = { 35, 34, 39, 36 }; //simon dice

uint8_t gameSequence[MAX_GAME_LENGTH] = { 0, 0, 0, 0, 0, 0 };
uint8_t gameIndex = 0;

bool FRK = true;

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xe4, 0x65, 0xb8, 0x71, 0x9e, 0x78};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int id; // must be unique for each sender board
  bool ready;  // indica si el juego esta preparado para jugarse
  bool win;    // indica si se gano el juego
  bool lose;   // indica si se perdio el juego
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 /* Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
*/}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  for (byte i = 0; i < 4; i++) {
  pinMode(ledPins[i], OUTPUT);
  digitalWrite(ledPins[i], LOW);
  pinMode(buttonPins[i], INPUT_PULLDOWN);
  } 

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  myData.ready = true;
  myData.win = false;
  myData.lose = false;
}
 
void loop() {
  // Set values to send
  myData.id = 0;

  if (myData.win!=true && myData.lose!=true){
    play_simon();
  }
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);  //este delay es para ver cada cuanto se envia la data, nosotros vamos a borrarlo pero por ahora lo dejo para saber que hace
}

void ReReset(){
  myData.ready = true;
  myData.win = false;
  myData.lose = false;
}
