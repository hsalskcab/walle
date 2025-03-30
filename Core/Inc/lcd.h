void printIIC(uint8_t args);
inline void write(uint8_t value);
void setParams(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
void init();
void init_priv();
void begin(uint8_t cols, uint8_t lines, uint8_t dotsize);
void clear();
void home();
void setCursor(uint8_t col, uint8_t row);
void lcdPrint(char c[]);
void lcdPrint_itos(int64_t n);
void noDisplay();
void display();
void noCursor();
void cursor();
void noBlink();
void blink();
void scrollDisplayLeft(void);
void scrollDisplayRight(void);
void leftToRight(void);
void rightToLeft(void);
void autoscroll(void);
void noAutoscroll(void);
void createChar(uint8_t location, uint8_t charmap[]);
void noBacklight(void);
void backlight(void);
void send(uint8_t value, uint8_t mode);
void write4bits(uint8_t value);
void expanderWrite(uint8_t _data);
void pulseEnable(uint8_t _data);

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define EN 0b00000100  // Enable bit
#define RW 0b00000010  // Read/Write bit
#define RS 0b00000001  // Register select bit



