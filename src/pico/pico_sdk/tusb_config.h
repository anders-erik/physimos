#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Device mode on USB controller port 0 (CFG_TUSB_MCU / CFG_TUSB_OS are
// provided by the pico-sdk tinyusb_device / tinyusb_common targets).
#define CFG_TUSB_RHPORT0_MODE   (OPT_MODE_DEVICE)

#define CFG_TUD_ENDPOINT0_SIZE  64

//------------- CLASS -------------//
// We only expose a single HID (keyboard) interface.
#define CFG_TUD_HID             1
#define CFG_TUD_CDC             0
#define CFG_TUD_MSC             0
#define CFG_TUD_MIDI            0
#define CFG_TUD_VENDOR          0

// HID buffer size, should be sufficient to hold report ID (if any) + data
#define CFG_TUD_HID_EP_BUFSIZE  16

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
