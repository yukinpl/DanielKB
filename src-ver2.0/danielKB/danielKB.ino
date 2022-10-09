#include "Keyboard.h"

#define DEBOUNCE_PASS_DELAY ( 20 )

uint8_t const outCnt = 12 ;
uint8_t const  inCnt =  8 ;
uint8_t const keyCnt = outCnt * inCnt ;

uint8_t const keymap[ keyCnt * 2 ] = {

/* 
 *  39 = SINGLE QUOTE
 *  92 = BACK SLASH
 *  
 * 128 = L CTRL
 * 129 = L SHIFT
 * 130 = L ALT
 * 131 = L GUI ( WIN / CMD )
 * 132 = R CTRL
 * 133 = R SHIFT
 * 134 = R ALT
 * 135 = R GUI ( WIN / CMD )
 * 
 * 176 = ENTER
 * 177 = ESC
 * 178 = BACK SPACE
 * 179 = TAB
 * 
 * 193 = CAPS_LOCK
 * 194 = F1
 * 195 = F2
 * 196 = F3
 * 197 = F4
 * 198 = F5
 * 199 = F6
 * 200 = F7
 * 201 = F8
 * 202 = F9
 * 203 = F10
 * 204 = F11
 * 205 = F12
 * 206 = PRINT SCREEN
 * 
 * 209 = INSERT
 * 212 = DELETE
 * 
 * 215 = RIGHT
 * 216 = LEFT
 * 217 = DOWN
 * 218 = UP
 */
 
/*       1     2     3     4     5     6     7     8  */
/* A */ '1' , '2' , 'q' , 'w' , 'a' , 's' , 'z' , 'x' ,
/* B */ '3' , '4' , 'e' , 'r' , 'd' , 'f' , 'c' , 'v' ,
/* C */ '5' , '6' , 't' , 'y' , 'g' , 'h' , 'b' , 'n' ,
/* D */ '7' , '8' , 'u' , 'i' , 'j' , 'k' , 'm' , ',' ,
/* E */ '9' , '0' , 'o' , 'p' , 'l' , ';' , '.' , '/' ,
/* F */ '-' , '=' ,  92 , '`' , '[' , ']' ,  39 ,  0  ,
/* G */ 177 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  , /* FN */
/* H */ 179 , 178 , 176 , ' ' , ' ' , ' ' , ' ' ,  0  , 
/* I */ 128 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* J */ 129 , 133 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* K */  0  ,  0  ,  0  ,  0  ,  0  ,  0  , 130 , 134 ,
/* L */  0  ,  0  ,  0  ,  0  ,  0  ,  0  , 131 , 135 ,

/*       1     2     3     4     5     6     7     8  */
/* A */ 194 , 195 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* B */ 196 , 197 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* C */ 198 , 199 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* D */ 200 , 201 ,  0  , 209 ,  0  ,  0  ,  0  ,  0  ,
/* E */ 202 , 203 ,  0  , 206 ,  0  , 216 ,  0  , 217 ,
/* F */ 204 , 205 ,  0  , 212 , 218 ,  0  , 215 ,  0  ,
/* G */  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* H */  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* I */ 193 ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* J */  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* K */  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,
/* L */  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  

} ;

bool currPressed[ keyCnt ] ;
bool prevPressed[ keyCnt ] ;

uint32_t currTime ;
uint32_t prevTime[ keyCnt ] ;

bool ink[ inCnt ] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } ;

typedef struct
{
  bool fk ;
  bool rk ;
} KEYS ;


KEYS distinguishInput( int value )
{
  KEYS keys ;
    /**/ if( 60 <= value )
  {
    keys.fk = true ;
    keys.rk = true ;
  }
  else if( 50 <= value && 60 >= value )
  {
    keys.fk = true  ;
    keys.rk = false ;
  }
  else if( 10 <= value && 20 >= value ) 
  {
    keys.fk = false ;
    keys.rk = true  ;
  }
  else  
  {
    keys.fk = false ;
    keys.rk = false ;
  }

  return keys ;
}

bool IsFnPressed()
{
  return currPressed[ 55 ] ;
}

void getIn()
{
  int a0 = analogRead( A0 ) ;
  int a1 = analogRead( A1 ) ;
  int a2 = analogRead( A2 ) ;
  int a3 = analogRead( A3 ) ;

  KEYS keys ;

  keys = distinguishInput( a0 ) ;
  ink[ 0 ] = keys.rk ;
  ink[ 1 ] = keys.fk ;

  keys = distinguishInput( a1 ) ;
  ink[ 2 ] = keys.rk ;
  ink[ 3 ] = keys.fk ;

  keys = distinguishInput( a2 ) ;
  ink[ 4 ] = keys.rk ;
  ink[ 5 ] = keys.fk ;

  keys = distinguishInput( a3 ) ;
  ink[ 6 ] = keys.rk ;
  ink[ 7 ] = keys.fk ;
}

void setOut( int si , bool val )
{
  int8_t arr[ 12 ] = {
  /* 1    2    3    4    5    6    7    8    9    10    11    12  index */
    12 , 11 , 10 ,  9 ,  8 ,  7 ,  6 ,  5 ,  4 ,  13 ,  22 ,  23
  } ;

  digitalWrite( arr[ si ] , val ) ;
}


void setup()
{
  // put your setup code here, to run once:
  pinMode( A0  , INPUT ) ; // 8 , 7
  pinMode( A1  , INPUT ) ; // 6 , 5
  pinMode( A2  , INPUT ) ; // 4 , 3
  pinMode( A3  , INPUT ) ; // 2 , 1

  pinMode( 12 , OUTPUT ) ; // L(11)
  pinMode( 11 , OUTPUT ) ; // K(10)
  pinMode( 10 , OUTPUT ) ; // J( 9)
  pinMode(  9 , OUTPUT ) ; // I( 8)
  pinMode(  8 , OUTPUT ) ; // H( 7)
  pinMode(  7 , OUTPUT ) ; // G( 6)
  pinMode(  6 , OUTPUT ) ; // F( 5)
  pinMode(  5 , OUTPUT ) ; // E( 4)
  pinMode(  4 , OUTPUT ) ; // D( 3)
  pinMode( 13 , OUTPUT ) ; // C( 2)
  pinMode( 22 , OUTPUT ) ; // B( 1)
  pinMode( 23 , OUTPUT ) ; // A( 0)

  for( int8_t pos = 0 ; pos < outCnt ; ++pos )
  {
    setOut( pos , LOW ) ;
  }
    
  for( int8_t pos = 0 ; pos < keyCnt ; ++pos )
  {
    prevPressed[ pos ] = false ;
    currPressed[ pos ] = false ;
    prevTime[ pos ] = 0 ;
  }

  Keyboard.begin() ;
}

void loop()
{
  // put your main code here, to run repeatedly:

  for( int8_t out = 0 ; out < outCnt ; ++out )
  {
    setOut( out , HIGH ) ;
    getIn() ;
    
    for( int8_t in = 0 ; in < inCnt ; ++in )
    {
      currPressed[ out * inCnt + in ] = ink[ in ] ;
    }

    setOut( out , LOW  ) ;
  }

  currTime = millis() ; 
  bool fn = IsFnPressed() ;

  for( uint8_t pos = 0 ; pos < keyCnt ; ++pos )
  {
    bool back = false ;
    if( prevPressed[ pos ] == currPressed[ pos ] )
    {
      back = true ;
    }
    else if( currTime - prevTime[ pos ] < DEBOUNCE_PASS_DELAY )
    {
      back = true ;
    }

    prevPressed[ pos ] = currPressed[ pos ] ;

    if( back )
    {
      continue ;
    }

    prevTime[ pos ] = currTime ;

    uint8_t key = ( fn == true ) ? ( keymap[ keyCnt + pos ] ) : ( keymap[ pos ] ) ;

    if( 0 != key )
    {
      if( true == currPressed[ pos ] )
      {
        Keyboard.press( key ) ;
      }
      else
      {
        Keyboard.release( key ) ;
      }
    }
  }
}
