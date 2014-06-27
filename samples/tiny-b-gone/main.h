// What pins do what
#define LED PB5
#define IRLED PD3

// Two regions!
#define US 0
#define EU 1

#define EU_CODES
#define NA_CODES

// Lets us calculate the size of the NA/EU databases
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)));

// Makes the codes more readable. the OCRA is actually
// programmed in terms of 'periods' not 'freqs' - that
// is, the inverse!
#define freq_to_timerval(x) (F_CPU / 8 / x - 1)
// Shortcut to insert single, non-optimized-out nop
#define NOP __asm__ __volatile__ ("nop")
// Tweak this if neccessary to change timing
#define DELAY_CNT 25

#define PULSE_CODE 0

// The structure of compressed code entries
typedef struct {
  uint8_t timer_val;
  uint8_t numpairs;
  uint8_t bitcompression;
  uint16_t const *times;
  uint8_t codes[];
} IrCode, *PIrCode;

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code );
void flashslowLEDx( uint8_t num_blinks );
void quickflashLEDx( uint8_t x );
void tvbgone_sleep( void );
void delay_ten_us(uint16_t us);
void quickflashLED( void );
