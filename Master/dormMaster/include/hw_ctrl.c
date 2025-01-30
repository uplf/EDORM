#include "hw_ctrl.h"


#ifdef CTRL_BAR


#endif

#ifdef  CTRL_HID


USBHIDVendor Vendor;

const int buttonPin = 0;
int previousButtonState = HIGH;

static void vendorEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    digitalWrite(14,HIGH);
  if (event_base == ARDUINO_USB_HID_VENDOR_EVENTS) {
    arduino_usb_hid_vendor_event_data_t *data = (arduino_usb_hid_vendor_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_HID_VENDOR_GET_FEATURE_EVENT: Serial.printf("HID VENDOR GET FEATURE: len:%u\n", data->len); break;
      case ARDUINO_USB_HID_VENDOR_SET_FEATURE_EVENT:
        trial.printf("HID VENDOR SET FEATURE: len:%u\n", data->len);
        for (uint16_t i = 0; i < data->len; i++) {
          trial.printf("0x%02X ", *(data->buffer));
        }
        trial.println();
        break;
      case ARDUINO_USB_HID_VENDOR_OUTPUT_EVENT:
        trial.printf("HID VENDOR OUTPUT: len:%u\n", data->len);
        for(uint16_t i=0; i<data->len; i++){
           trial.write(Vendor.read());
         }
        break;

      default: break;
    }
  }
}

void hid_vector_setup(){
    Vendor.onEvent(vendorEventCallback);
  Vendor.begin();
  USB.begin();
}


#endif

