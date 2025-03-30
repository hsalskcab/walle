#include "main.h"
#include "lcd.h"

uint8_t buf=0;
extern I2C_HandleTypeDef hi2c1;
void printIIC(uint8_t args){
	buf=args;
	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t)(0x4E),&buf,1,1000);
}
inline void write(uint8_t value) {
	send(value, RS);
}

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

uint8_t _Addr;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _numlines;
uint8_t _cols;
uint8_t _rows;
uint8_t _backlightval;

void setParams(uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows)
{
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _backlightval = LCD_NOBACKLIGHT;
}

void init(){
	init_priv();
}

void init_priv()
{
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	begin(_cols, _rows, 0);
}

void begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
	if (lines > 1) {
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	HAL_Delay(50);

	// Now we pull both RS and R/W low to begin commands
	//_backlightval=LCD_BACKLIGHT;
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	HAL_Delay(1000);

  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	  // we start in 8bit mode, try to set 4 bit mode
   write4bits(0x03 << 4);
   usDelay(4500); // wait min 4.1ms

   // second try
   write4bits(0x03 << 4);
   usDelay(4500); // wait min 4.1ms

   // third go!
   write4bits(0x03 << 4);
   usDelay(150);

   // finally, set to 4-bit interface
   write4bits(0x02 << 4);


	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	home();

}

/********** high level commands, for the user! */
void clear(){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	usDelay(2000);  // this command takes a long time!
}

void home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	usDelay(2000);  // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcdPrint(char c[]){ ////////////////////////////////////////////////////////////////this one!
	uint8_t i=0;
	while(c[i]!=0){
		send((uint8_t) c[i],1);
		i++;
	}
}

void lcdPrint_itos(int64_t n){
	if (n==0) send(48,1);
	else{
		if (n<0){
				lcdPrint("-");
				n=-n;
			}
	uint8_t c[20];
	int i=0;
	while(n!=0){
		c[i]=((n%10)+48);
		n/=10;
		i++;
	}
	for(i--;i!=-1;i--){
		send((uint8_t)c[i],1);
	}
	}
}


// Turn the display on/off (quickly)
void noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
	//location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void noBacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}



/*********** mid level commands, for sending data/cmds */

inline void command(uint8_t value) {
	send(value, 0);
}


/************ low level data pushing commands **********/

// write either command or data
void send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
       write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

void expanderWrite(uint8_t _data){
	printIIC((int)(_data) | _backlightval);
}

void pulseEnable(uint8_t _data){
	expanderWrite(_data | EN);	// En high
	usDelay(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~EN);	// En low
	usDelay(40);		// commands need > 37us to settle
}

