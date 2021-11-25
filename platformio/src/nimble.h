#include <Arduino.h>
#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

void commands(String rxString);
void bluetooth_Connect();
void bluetooth_Disconnect();

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t txValue = 0;

class BluetoothCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        String rxString = rxValue.c_str();
        commands(rxString);
      }
    }
};

class BluetoothConnectionCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      bluetooth_Connect();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      bluetooth_Disconnect();
    }
};

void setupBLE() {
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BluetoothConnectionCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      //BLECharacteristic::PROPERTY_NOTIFY
                      NIMBLE_PROPERTY::NOTIFY
                    );

  //pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         //BLECharacteristic::PROPERTY_WRITE
                                         NIMBLE_PROPERTY::WRITE
                                       );

  pCharacteristic->setCallbacks(new BluetoothCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}