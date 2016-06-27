/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 */
#ifndef __UsbKeyboard_h__
#define __UsbKeyboard_h__

#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */

#ifdef __cplusplus
extern "C"
{
#endif

#include "utility/usbdrv.h"

#ifdef __cplusplus
}
#endif

typedef uint8_t byte;

typedef struct kbd_data
{
	//uint8_t report_id;
    uint8_t modifiers;
	uint8_t key;
    //int8_t x;
    //int8_t y;
} kbd_data_t;

#define KEYS_COUNT 1 // Minimum of 1, we will increase it in the future

static uchar    idleRate;           // in 4 ms units

/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 * You need to modify USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH in usbconfig.h if you changed it.
 */

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
  0x95, KEYS_COUNT-1,           //   REPORT_COUNT (simultaneous keystrokes)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
  0xc0                           // END_COLLECTION
};



/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */

#define KEY_CTRL	0x01
#define KEY_SHIFT	0x02
#define KEY_ALT		0x04
#define KEY_GUI		0x08
#define KEY_LEFT_CTRL	0x01
#define KEY_LEFT_SHIFT	0x02
#define KEY_LEFT_ALT	0x04
#define KEY_LEFT_GUI	0x08
#define KEY_RIGHT_CTRL	0x10
#define KEY_RIGHT_SHIFT	0x20
#define KEY_RIGHT_ALT	0x40
#define KEY_RIGHT_GUI	0x80

#define KEY_A		4
#define KEY_B		5
#define KEY_C		6
#define KEY_D		7
#define KEY_E		8
#define KEY_F		9
#define KEY_G		10
#define KEY_H		11
#define KEY_I		12
#define KEY_J		13
#define KEY_K		14
#define KEY_L		15
#define KEY_M		16
#define KEY_N		17
#define KEY_O		18
#define KEY_P		19
#define KEY_Q		20
#define KEY_R		21
#define KEY_S		22
#define KEY_T		23
#define KEY_U		24
#define KEY_V		25
#define KEY_W		26
#define KEY_X		27
#define KEY_Y		28
#define KEY_Z		29
#define KEY_1		30
#define KEY_2		31
#define KEY_3		32
#define KEY_4		33
#define KEY_5		34
#define KEY_6		35
#define KEY_7		36
#define KEY_8		37
#define KEY_9		38
#define KEY_0		39

#define KEY_ENTER	40	
#define KEY_ESC		41
#define KEY_BACKSPACE	42	
#define KEY_TAB		43
#define KEY_SPACE	44	
#define KEY_CAPS_LOCK	57

#define KEY_MINUS	45
#define KEY_EQUAL	46
#define KEY_LEFT_BRACE	47
#define KEY_RIGHT_BRACE	48
#define KEY_BACKSLASH	49
#define KEY_NUMBER	50
#define KEY_SEMICOLON	51
#define KEY_QUOTE	52
#define KEY_TILDE	53
#define KEY_COMMA	54
#define KEY_PERIOD	55
#define KEY_SLASH	56

#define KEY_F1		58
#define KEY_F2		59
#define KEY_F3		60
#define KEY_F4		61
#define KEY_F5		62
#define KEY_F6		63
#define KEY_F7		64
#define KEY_F8		65
#define KEY_F9		66
#define KEY_F10		67
#define KEY_F11		68
#define KEY_F12		69

#define KEY_PRINTSCREEN	70		
#define KEY_SCROLL_LOCK	71		
#define KEY_PAUSE	72
#define KEY_INSERT	73	
#define KEY_HOME	74
#define KEY_PAGE_UP	75
#define KEY_DELETE	76
#define KEY_END		77
#define KEY_PAGE_DOWN	78	
#define KEY_RIGHT	79
#define KEY_LEFT	80
#define KEY_DOWN	81
#define KEY_UP		82

#define KEY_NUM_LOCK	83

#define KEYPAD_SLASH	84	
#define KEYPAD_ASTERIX	85	
#define KEYPAD_MINUS	86	
#define KEYPAD_PLUS	87	
#define KEYPAD_ENTER	88	
#define KEYPAD_1	89
#define KEYPAD_2	90
#define KEYPAD_3	91	
#define KEYPAD_4	92	
#define KEYPAD_5	93	
#define KEYPAD_6	94	
#define KEYPAD_7	95	
#define KEYPAD_8	96	
#define KEYPAD_9	97	
#define KEYPAD_0	98		
#define KEYPAD_PERIOD	99	

#define KEY_KEYBOARD_NON_US_BACKSLASH    100
#define KEY_APPLICATION    101
#define KEY_POWER    102
#define KEY_EQUAL    103
#define KEY_F13    104
#define KEY_F14   105
#define KEY_F15    106
#define KEY_F16    107
#define KEY_F17    108
#define KEY_F18    109
#define KEY_F19    110
#define KEY_F20    111
#define KEY_F21    112
#define KEY_F22    113
#define KEY_F23    114
#define KEY_F24    115
#define KEY_EXECUTE    116
#define KEY_HELP    117
#define KEY_MENU    118
#define KEY_SELECT    119
#define KEY_STOP    120
#define KEY_AGAIN    121
#define KEY_UNDO    122
#define KEY_CUT    123
#define KEY_COPY    124
#define KEY_PASTE    125
#define KEY_FIND    126
#define KEY_MUTE    127
#define KEY_VOLUME_UP    128
#define KEY_VOLUME_DOWN     129
#define KEY_LOCKING_CAPS LOCK    130
#define KEY_LOCKING_NUM LOCK    131
#define KEY_LOCKING_SCROLL_LOCK    132
#define KEY_COMMA    133
#define KEY_EQUAL_SIGN    134

#define KEY_ARROW_LEFT KEY_LEFT //deprecated
 
class UsbKeyboardDevice {
  protected:
	union {
		uchar report;
		kbd_data data;
	} reportBuffer;    // buffer for HID reports

  public:
  UsbKeyboardDevice() {
	USBOUT = 0; // TODO: Only for USB pins?
    USBDDR |= ~USBMASK;

    cli();
    usbDeviceDisconnect();
    //_delay_ms(75);	
    usbDeviceConnect();

    usbInit();

    sei();

    // TODO: Remove the next two lines once we fix
    //       missing first keystroke bug properly.
    memset(&reportBuffer.data, 0, sizeof(reportBuffer.data));
    usbSetInterrupt(&reportBuffer.report, sizeof(reportBuffer.data));
  }

  void delay(){
	_delay_ms(75);	
  }
  void update() {
    usbPoll();
  }

  void sendKeyStroke(byte keyStroke) {
    sendKeyStroke(keyStroke, 0);
  }

  void sendKeyStroke(byte keyStroke, byte modifiers) {

    while (!usbInterruptIsReady()) {
        // Note: We wait until we can send keystroke
        //       so we know the previous keystroke was
        //       sent.
    }

    memset(&reportBuffer.data, 0, sizeof(reportBuffer.data));

    reportBuffer.data.modifiers = modifiers;
    reportBuffer.data.key = keyStroke;

    usbSetInterrupt(&reportBuffer.report, sizeof(reportBuffer.data));

    while (!usbInterruptIsReady()) {
      // Note: We wait until we can send keystroke
      //       so we know the previous keystroke was
      //       sent.
    }

    // This stops endlessly repeating keystrokes:
    memset(&reportBuffer.data, 0, sizeof(reportBuffer.data));
    usbSetInterrupt(&reportBuffer.report, sizeof(reportBuffer.data));
  }
};

UsbKeyboardDevice UsbKeyboard = UsbKeyboardDevice();

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t *)((void *)data);

    //usbMsgPtr = (uchar *)UsbKeyboard.reportBuffer; 
	
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
      /* class request type */

      if(rq->bRequest == USBRQ_HID_GET_REPORT){
	/* wValue: ReportType (highbyte), ReportID (lowbyte) */

	/* we only have one report type, so don't look at wValue */
        // TODO: Ensure it's okay not to return anything here?
		return 0;

      } else if(rq->bRequest == USBRQ_HID_GET_IDLE)
	{
	//            usbMsgPtr = &idleRate;
	//            return 1;
		return 0;
      } else if(rq->bRequest == USBRQ_HID_SET_IDLE){
			idleRate = rq->wValue.bytes[1];
      }
    } else {
      /* no vendor specific requests implemented */
    }
    return 0;
  }

#endif // __UsbKeyboard_h__
