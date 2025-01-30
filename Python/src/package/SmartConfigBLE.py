from bluetooth import BLE, UUID, FLAG_WRITE
from network import WLAN, STA_IF
from time import sleep

class SmartConfigBLE(object):
    _IRQ_CENTRAL_CONNECT                        = 1
    _IRQ_CENTRAL_DISCONNECT                     = 2
    _IRQ_GATTS_WRITE                            = 3
    _IRQ_GATTS_READ_REQUEST                     = 4
    _IRQ_SCAN_RESULT                            = 5
    _IRQ_SCAN_DONE                              = 6
    _IRQ_PERIPHERAL_CONNECT                     = 7
    _IRQ_PERIPHERAL_DISCONNECT                  = 8
    _IRQ_GATTC_SERVICE_RESULT                   = 9
    _IRQ_GATTC_SERVICE_DONE                     = 10
    _IRQ_GATTC_CHARACTERISTIC_RESULT            = 11
    _IRQ_GATTC_CHARACTERISTIC_DONE              = 12
    _IRQ_GATTC_DESCRIPTOR_RESULT                = 13
    _IRQ_GATTC_DESCRIPTOR_DONE                  = 14
    _IRQ_GATTC_READ_RESULT                      = 15
    _IRQ_GATTC_READ_DONE                        = 16
    _IRQ_GATTC_WRITE_DONE                       = 17
    _IRQ_GATTC_NOTIFY                           = 18
    _IRQ_GATTC_INDICATE                         = 19
    _IRQ_GATTS_INDICATE_DONE                    = 20
    _IRQ_MTU_EXCHANGED                          = 21
    _IRQ_L2CAP_ACCEPT                           = 22
    _IRQ_L2CAP_CONNECT                          = 23
    _IRQ_L2CAP_DISCONNECT                       = 24
    _IRQ_L2CAP_RECV                             = 25
    _IRQ_L2CAP_SEND_READY                       = 26
    _IRQ_CONNECTION_UPDATE                      = 27
    _IRQ_ENCRYPTION_UPDATE                      = 28
    _IRQ_GET_SECRET                             = 29
    _IRQ_SET_SECRET                             = 30
    _GATTS_NO_ERROR                             = 0x00
    _GATTS_ERROR_READ_NOT_PERMITTED             = 0x02
    _GATTS_ERROR_WRITE_NOT_PERMITTED            = 0x03
    _GATTS_ERROR_INSUFFICIENT_AUTHENTICATION    = 0x05
    _GATTS_ERROR_INSUFFICIENT_AUTHORIZATION     = 0x08
    _GATTS_ERROR_INSUFFICIENT_ENCRYPTION        = 0x0f
    _PASSKEY_ACTION_NONE                        = 0
    _PASSKEY_ACTION_INPUT                       = 2
    _PASSKEY_ACTION_DISPLAY                     = 3
    _PASSKEY_ACTION_NUMERIC_COMPARISON          = 4
    def __init__(self, names: str, data: dict):
        self.names = names
        self.ble = BLE()
        self.ble.active(True)
        print("MAC ESP32 address is: " + self.ble.config('mac')[1].hex())
        self.ble.irq(self._BLE_IRQ)
        self.connections = set()
        self.serviceElements = []
        self.wifiConfigs = {"ssid": "", "password": ""}
        self.uuid = UUID(self._UUID("uuid", 0))
        self._BLE_RegisterElement(data)
        self._BLE_Advertisement()
        
    def _BLE_IRQ(self, event, data):
        if event == self._IRQ_CENTRAL_CONNECT:
            conn, types, addr = data
            self.connections.add(conn)
            address = ':'.join('{:02x}'.format(char) for char in addr)
            print("Connected to " + address + " is SUCCESS")
        elif event == self._IRQ_CENTRAL_DISCONNECT:
            conn, types, addr = data
            self.connections.remove(conn)
            address = ':'.join('{:02x}'.format(char) for char in addr)
            print("Disconnected from " + address + " is SUCCESS")
        elif event == self._IRQ_GATTS_WRITE:
            conn, attribute = data
            values = self.ble.gatts_read(attribute).decode('utf-8').strip()
            print("Write is SUCCESS with attibute: ", attribute, " values: ", values)
            self._handleWrite(attribute, values)
            
    def _convert(self, data: str):
        return ['{:04b}'.format(ord(char))[-4:] for char in data]
            
    def _UUID(self, data: str, position: int):
        mac_address = self.ble.config('mac')[1].hex()
        if len(data) > 4:
            data = data[0:4]
        binary = self._convert(data)
        uuid = ''.join(binary) + '{:04b}'.format(position)
        uuid += mac_address.replace(':', '')
        uuid_f = f"{uuid[:8]}-{uuid[8:12]}-{uuid[12:16]}-{uuid[16:20]}-{uuid[20:]}"
        return uuid_f
        
    def _BLE_RegisterElement(self, dictionary):
        keys = list(dictionary.keys())
        values = list(dictionary.values())
        for key, i in zip(keys, range(len(keys))):
            element = (UUID(self._UUID(str(key), values[i])), FLAG_WRITE)
            self.serviceElements.append(element)
        _SERVICE = (self.uuid, tuple(self.serviceElements))
        SERVICES = (_SERVICE, )
        (self.ssid_t, self.pass_t) = self.ble.gatts_register_services(SERVICES)[0]
            
    def connectWifi(self):
        if self.wifiConfigs["ssid"] and self.wifiConfigs["password"]:
            sta = WLAN(STA_IF)
            sta.active(True)
            sta.connect(self.wifiConfigs["ssid"], self.wifiConfigs["password"])
            while not sta.isconnected():
                sleep(0.01)
            print("Connected to ", self.wifiConfigs["ssid"], " with IP: ", sta.ifconfig()[0])
        else:
            print("Waiting for SSID and Password...")
        
    def _handleWrite(self, attribute, values):
        if attribute == self.ssid_t:
            self.wifiConfigs["ssid"] = values
            print("SSID set to ", self.wifiConfigs["ssid"])
            self.connectWifi()
        elif attribute == self.pass_t:
            self.wifiConfigs["password"] = values
            print("Password set to ", self.wifiConfigs["password"])
            self.connectWifi()
        else:
            print("Unknown characteristic written")
    
    def _BLE_Advertisement(self):
        user = bytes(self.names, 'utf-8')
        data = bytearray(b'\x02\x01\x02') + bytearray((len(user) + 1, 0x09)) + user
        self.ble.gap_advertise(100, data)