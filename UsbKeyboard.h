/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 */
#ifndef __UsbKeyboard_h__
#define __UsbKeyboard_h__
//#define _OSCCAL_
/*
	Please Set USB_CFG_HAVE_MEASURE_FRAME_LENGTH to 1 in usbconfig.h
	
#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   1

*/	

#include <string.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>   //Eeprom read
#include <avr/interrupt.h>  // for sei() 
#include <util/delay.h>     // for _delay_ms() 

#include <avr/pgmspace.h>   // required by usbdrv.h 

#ifdef __cplusplus
extern "C"
{
#endif

#include "utility/usbdrv.h"

#ifdef __cplusplus
}
#endif

typedef uint8_t byte;

typedef struct 
{
	uint8_t report_id;
    uint8_t modifiers;
	uint8_t keycode; //TODO should be array
	//uint8_t keycode[6];
} keyboard_data_t;

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
 *
 * //http://www.microchip.com/forums/FindPost/878780 
 */
 
#define KEYCODE_LENGTH 1
#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_CONSUMER 2

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = // USB report descriptor
{
	//LENGTH=40
	0x05, 0x01,                    //USAGE_PAGE (Generic Desktop)
	0x09, 0x06,                    // USAGE (Keyboard)
	0xa1, 0x01,                    //COLLECTION (Application)
	0x85, REPORT_ID_KEYBOARD,    				   //   Report ID=1
	//Modifiers
	0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
	0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
	0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,                    //   REPORT_SIZE (1)
	0x95, 0x08,                    //   REPORT_COUNT (8)
	0x81, 0x02,                    //   INPUT (Data,Var,Abs)
	//Key byte
	0x75, 0x08,                    //   REPORT_SIZE(8)
	0x95, KEYCODE_LENGTH,          //   REPORT_COUNT(KEYCODE_LENGTH) //TODO If we are using array we will set it to the length of that array 6
	0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	0x26, 0x86, 0x00,              //   LOGICAL_MAXIMUM (134)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
	0x81, 0x00,                    //   INPUT (Data,Ary,Abs)	
	0xc0,                           //END_COLLECTION
	
    /* consumer */
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, REPORT_ID_CONSUMER,      //   REPORT_ID (2)
    0x15, 0x01,                    //   LOGICAL_MINIMUM (0x1)
    0x26, 0x9c, 0x02,              //   LOGICAL_MAXIMUM (0x29c)
    0x19, 0x01,                    //   USAGE_MINIMUM (0x1)
    0x2a, 0x9c, 0x02,              //   USAGE_MAXIMUM (0x29c)
    0x75, 0x10,                    //   REPORT_SIZE (16)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x00,                    //   INPUT (Data,Array,Abs)
    0xc0                          // END_COLLECTION	
};

/* 
 * Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 *	or http://www.freebsddiary.org/APC/usb_hid_usages.php 
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

#define KEY_PAD_SLASH	84
#define KEY_PAD_ASTERIX	85
#define KEY_PAD_MINUS	86
#define KEY_PAD_PLUS	87
#define KEY_PAD_ENTER	88
#define KEY_PAD_1	89
#define KEY_PAD_2	90
#define KEY_PAD_3	91
#define KEY_PAD_4	92
#define KEY_PAD_5	93
#define KEY_PAD_6	94
#define KEY_PAD_7	95
#define KEY_PAD_8	96
#define KEY_PAD_9	97
#define KEY_PAD_0	98
#define KEY_PAD_PERIOD	99

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

/*
*  USB_Hut1_12v2 Page 75 chapter 15
*/

#define CONSUMER_CHANNEL_INCREMENT 0x9C
#define CONSUMER_CHANNEL_DECREMENT 0x9D

#define CONSUMER_PLAY 0xB0
#define CONSUMER_PAUSE 0xB1
#define CONSUMER_STOP 0xB1
#define CONSUMER_PLAY_PAUSE 0xE2
#define CONSUMER_MUTE 0xE2

#define CONSUMER_VOLUME_INCREMENT 0xE9
#define CONSUMER_VOLUME_DECREMENT 0xEA

#define KEY_ARROW_LEFT KEY_LEFT //deprecated
 
//ported from https://github.com/aduitsis/ardumultimedia/blob/master/HID.cpp
#define SHIFT 0x80
const uint8_t asciimap[128] =
{
	0x00,             // NUL
	0x00,             // SOH
	0x00,             // STX
	0x00,             // ETX
	0x00,             // EOT
	0x00,             // ENQ
	0x00,             // ACK  
	0x00,             // BEL
	KEY_BACKSPACE,			// BS	Backspace
	KEY_TAB,			// TAB	Tab
	KEY_ENTER,			// LF	Enter
	0x00,             // VT 
	0x00,             // FF 
	0x00,             // CR 
	0x00,             // SO 
	0x00,             // SI 
	0x00,             // DEL
	0x00,             // DC1
	0x00,             // DC2
	0x00,             // DC3
	0x00,             // DC4
	0x00,             // NAK
	0x00,             // SYN
	0x00,             // ETB
	0x00,             // CAN
	0x00,             // EM 
	0x00,             // SUB
	0x00,             // ESC
	0x00,             // FS 
	0x00,             // GS 
	0x00,             // RS 
	0x00,             // US 

	0x2c,		   //  ' '
	0x1e|SHIFT,	   // !
	0x34|SHIFT,	   // "
	0x20|SHIFT,    // #
	0x21|SHIFT,    // $
	0x22|SHIFT,    // %
	0x24|SHIFT,    // &
	0x34,          // '
	0x26|SHIFT,    // (
	0x27|SHIFT,    // )
	0x25|SHIFT,    // *
	0x2e|SHIFT,    // +
	0x36,          // ,
	0x2d,          // -
	0x37,          // .
	0x38,          // /
	0x27,          // 0
	0x1e,          // 1
	0x1f,          // 2
	0x20,          // 3
	0x21,          // 4
	0x22,          // 5
	0x23,          // 6
	0x24,          // 7
	0x25,          // 8
	0x26,          // 9
	0x33|SHIFT,      // :
	0x33,          // ;
	0x36|SHIFT,      // <
	0x2e,          // =
	0x37|SHIFT,      // >
	0x38|SHIFT,      // ?
	0x1f|SHIFT,      // @
	KEY_A|SHIFT,      // A
	KEY_B|SHIFT,      // B
	KEY_C|SHIFT,      // C
	KEY_D|SHIFT,      // D
	KEY_E|SHIFT,      // E
	KEY_F|SHIFT,      // F
	KEY_G|SHIFT,      // G
	KEY_H|SHIFT,      // H
	KEY_I|SHIFT,      // I
	KEY_J|SHIFT,      // J
	KEY_K|SHIFT,      // K
	KEY_L|SHIFT,      // L
	KEY_M|SHIFT,      // M
	KEY_N|SHIFT,      // N
	KEY_O|SHIFT,      // O
	KEY_P|SHIFT,      // P
	KEY_Q|SHIFT,      // Q
	KEY_R|SHIFT,      // R
	KEY_S|SHIFT,      // S
	KEY_T|SHIFT,      // T
	KEY_U|SHIFT,      // U
	KEY_V|SHIFT,      // V
	KEY_W|SHIFT,      // W
	KEY_X|SHIFT,      // X
	KEY_Y|SHIFT,      // Y
	KEY_Z|SHIFT,      // Z
	0x2f,          // [
	0x31,          // bslash
	0x30,          // ]
	0x23|SHIFT,    // ^
	0x2d|SHIFT,    // _
	0x35,          // `
	KEY_A,          // a
	KEY_B,          // b
	KEY_C,          // c
	KEY_D,          // d
	KEY_E,          // e
	KEY_F,          // f
	KEY_G,          // g
	KEY_H,          // h
	KEY_I,          // i
	KEY_J,          // j
	KEY_K,          // k
	KEY_L,          // l
	KEY_M,          // m
	KEY_N,          // n
	KEY_O,          // o
	KEY_P,          // p
	KEY_Q,          // q
	KEY_R,          // r
	KEY_S,          // s
	KEY_T,          // t
	KEY_U,          // u
	KEY_V,          // v
	KEY_W,          // w
	KEY_X,          // x
	KEY_Y,          // y
	KEY_Z,          // z
	0x2f|SHIFT,    // 
	0x31|SHIFT,    // |
	0x30|SHIFT,    // }
	0x35|SHIFT,    // ~
	0				// DEL
};
 
#ifdef _OSCCAL_
/* ------------------------------------------------------------------------- */
/* ------------------------ Oscillator Calibration ------------------------- */
/* ------------------------------------------------------------------------- */

/* Calibrate the RC oscillator to 8.25 MHz. The core clock of 16.5 MHz is
 * derived from the 66 MHz peripheral clock by dividing. Our timing reference
 * is the Start Of Frame signal (a single SE0 bit) available immediately after
 * a USB RESET. We first do a binary search for the OSCCAL value and then
 * optimize this value with a neighboorhod search.
 * This algorithm may also be used to calibrate the RC oscillator directly to
 * 12 MHz (no PLL involved, can therefore be used on almost ALL AVRs), but this
 * is wide outside the spec for the OSCCAL value and the required precision for
 * the 12 MHz clock! Use the RC oscillator calibrated to 12 MHz for
 * experimental purposes only!
 */
static void calibrateOscillator(void)
{
uchar       step = 128;
uchar       trialValue = 0, optimumValue;
int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

    /* do a binary search: */
    do{
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    }while(step > 0);
    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++){
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0)
            x = -x;
        if(x < optimumDev){
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}
/*
Note: This calibration algorithm may try OSCCAL values of up to 192 even if
the optimum value is far below 192. It may therefore exceed the allowed clock
frequency of the CPU in low voltage designs!
You may replace this search algorithm with any other algorithm you like if
you have additional constraints such as a maximum CPU clock.
For version 5.x RC oscillators (those with a split range of 2x128 steps, e.g.
ATTiny25, ATTiny45, ATTiny85), it may be useful to search for the optimum in
both regions.
*/

void usbEventResetReady(void)
{
    calibrateOscillator();
    eeprom_write_byte(0, OSCCAL);   /* store the calibrated value in EEPROM */
}
#endif _OSCCAL_

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (usbRequest_t *)((void *)data);	
	
	//static uchar dataBuffer[4];  /* buffer must stay valid when usbFunctionSetup returns */

	if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
		/* class request type */

		if(rq->bRequest == USBRQ_HID_GET_REPORT)
		{
			/* wValue: ReportType (highbyte), ReportID (lowbyte) */
			//usbMsgPtr = (uchar *)&UsbKeyboard.keyboard_data;
			/* we only have one report type, so don't look at wValue */ 
			// TODO: Ensure it's okay not to return anything here?
			//return sizeof(reportBuffer);
			return 0;
			
		} else if(rq->bRequest == USBRQ_HID_GET_IDLE){
			usbMsgPtr = &idleRate;
			return 1;
		} else if(rq->bRequest == USBRQ_HID_SET_IDLE){
			idleRate = rq->wValue.bytes[1];
		}
	} else {
	  /* no vendor specific requests implemented */
	}
	return 0;
}

//static keyboard_data_t reportData;

class UsbKeyboardDevice {	
  public:  
	UsbKeyboardDevice() 
	{
		USBOUT = 0; // TODO: Only for USB pins?
		USBDDR |= ~USBMASK;
		//wdt_enable(WDTO_1S);

#ifdef _OSCCAL_		
		uchar   calibrationValue;
		//Clock calibration setup
		calibrationValue = eeprom_read_byte(0); /* calibration value from last time */
		if(calibrationValue != 0xff){
			OSCCAL = calibrationValue;
		}		
#endif _OSCCAL_

		//MCUSR &= ~(1 << WDRF);
		//wdt_disable(); 	
//		int i = 0;
		
		cli();
		usbDeviceDisconnect();
		//
		usbDeviceConnect();
		usbInit();
		sei();
		
		int i = 100;
		while(--i) {
			_delay_ms(1);
			usbPoll();
		}

		// TODO: Remove the next two lines once we fix
		//       missing first keystroke bug properly.
		keyboard_data_t report = {
			.report_id = REPORT_ID_KEYBOARD,
			.modifiers = 0,
			.keycode = 0
		};

		usbSetInterrupt((uchar *)&report, sizeof(report));
	}

	void delay(){
		_delay_ms(1);	 
	}

	void update() {
		usbPoll();
	}

	void wait(){		
		while (!usbInterruptIsReady()) {
			//_delay_us(1);	 
			usbPoll();
		}
	}

	//Send key code down without sending up/release it, you need to send 0 to release it or use SendKeyStroke
	void sendKey(byte keyStroke, byte modifiers) 
	{
		wait();
		// Note: We wait until we can send keystroke
		//       so we know the previous keystroke was
		//       sent.

		keyboard_data_t report = {
			.report_id = REPORT_ID_KEYBOARD,
			.modifiers = modifiers,
			.keycode = keyStroke
		};

		usbSetInterrupt((uchar *)&report, sizeof(report));		
	}  
	
	//Stroke send key down and up
	void sendKeyStroke(byte keycode) {
		sendKeyStroke(keycode, 0);
	}

	void sendKeyStroke(byte keycode, byte modifiers) 
	{
		sendKey(keycode, modifiers);
		sendKey(0, 0);
		wait();		
	}  

	typedef struct {
		uint8_t  report_id;
		uint16_t data;
	} __attribute__ ((packed)) report_consumer_t;

	void sendConsumerStroke(uint16_t data) 
	{
		wait();

		report_consumer_t report = {
			.report_id = REPORT_ID_CONSUMER,
			.data = data
		};	    

		usbSetInterrupt((uchar *)&report, sizeof(report));

		wait();

		report = {
			.report_id = REPORT_ID_CONSUMER,
			.data = data
		};	    

		usbSetInterrupt((uchar *)&report, sizeof(report));		
		
		wait();
	}
	
	void sendAscii(uchar c)
	{
		if (c < sizeof(asciimap)) {
			byte keycode = asciimap[c];
			byte modifiers = 0;
			if (keycode & 0x80) { 
				modifiers |= 0x02;
				keycode &= 0x7F;
			}		
			sendKeyStroke(keycode, modifiers);
			//sendKeyStroke(KEY_6, 0);
		} else {
		}
	}
	
	void sendString(char str[])
	{
		unsigned int i = 0;
		while (i < strlen(str))
		{
			sendAscii(str[i]);			
			delay();
			i++;
		}		
	}
}; //UsbKeyboardDevice Class


UsbKeyboardDevice UsbKeyboard = UsbKeyboardDevice();

#endif // __UsbKeyboard_h__