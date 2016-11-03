/* clang-format off */
/*********************************************************************
*              SEGGER MICROCONTROLLER GmbH & Co. KG                  *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 2014-2014 SEGGER Microcontroller GmbH & Co. KG          *
*                                                                    *
*       Internet: www.segger.com Support: support@segger.com         *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File    : SEGGER_RTT_printf.c
Date    : 17 Dec 2014
Purpose : Replacement for printf to write formatted data via RTT
---------------------------END-OF-HEADER------------------------------
*/
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#ifndef SEGGER_RTT_PRINTF_BUFFER_SIZE
  #define SEGGER_RTT_PRINTF_BUFFER_SIZE (64)
#endif

#include <stdlib.h>
#include <stdarg.h>


#define FORMAT_FLAG_LEFT_JUSTIFY   (1 << 0)
#define FORMAT_FLAG_PAD_ZERO       (1 << 1)
#define FORMAT_FLAG_PRINT_SIGN     (1 << 2)
#define FORMAT_FLAG_ALTERNATE      (1 << 3)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  char* pBuffer;
  int   BufferSize;
  int   Cnt;

  int   ReturnValue;

  unsigned RTTBufferIndex;
} SEGGER_RTT_PRINTF_DESC;

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _StoreChar
*/
static void _StoreChar(SEGGER_RTT_PRINTF_DESC * p, char c) {
  int Cnt;

  Cnt = p->Cnt;
  if ((Cnt + 1) <= p->BufferSize) {
    *(p->pBuffer + Cnt) = c;
    p->Cnt = Cnt + 1;
    p->ReturnValue++;
  }
  //
  // Write part of string, when the buffer is full
  //
  if (p->Cnt == p->BufferSize) {
    if (SEGGER_RTT_Write(p->RTTBufferIndex, p->pBuffer, p->Cnt) != p->Cnt) {
      p->ReturnValue = -1;
    } else {
      p->Cnt = 0;
    }
  }
}

/*********************************************************************
*
*       _PrintUnsigned
*/
static void _PrintUnsigned(SEGGER_RTT_PRINTF_DESC * pBufferDesc, unsigned v, unsigned Base, int NumDigits, unsigned FieldWidth, unsigned FormatFlags) {
  static const char _aV2C[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  unsigned Div;
  unsigned Digit = 1;
  unsigned Number;
  unsigned Width;
  char c;

  Number = v;

  //
  // Get actual field width
  //
  Width = 1;
  while (Number >= Base) {
    Number = (Number / Base);
    Width++;
  }
  if ((unsigned)NumDigits > Width) {
    Width = NumDigits;
  }
  //
  // Print leading chars if necessary
  //
  if ((FormatFlags & FORMAT_FLAG_LEFT_JUSTIFY) == 0) {
    if (FieldWidth != 0) {
      if (((FormatFlags & FORMAT_FLAG_PAD_ZERO) == FORMAT_FLAG_PAD_ZERO) && (NumDigits == 0)) {
        c = '0';
      } else {
        c = ' ';
      }
      while ((FieldWidth != 0) && (Width < FieldWidth--)) {
        _StoreChar(pBufferDesc, c);
        if (pBufferDesc->ReturnValue < 0) {
          return;
        }
      }
    }
  }
  //
  // Count how many digits are required by precision
  //
  while (((v / Digit) >= Base) | (NumDigits-- > 1)) {
    Digit *= Base;
  }
  //
  // Output digits
  //
  do {
    Div = v / Digit;
    v -= Div * Digit;
    _StoreChar(pBufferDesc, _aV2C[Div]);
    if (pBufferDesc->ReturnValue < 0) {
      break;
    }
    Digit /= Base;
  } while (Digit);
  //
  // Print trailing spaces if necessary
  //
  if ((FormatFlags & FORMAT_FLAG_LEFT_JUSTIFY) == FORMAT_FLAG_LEFT_JUSTIFY) {
    if (FieldWidth != 0) {
      while ((FieldWidth != 0) && (Width < FieldWidth--)) {
        _StoreChar(pBufferDesc, ' ');
        if (pBufferDesc->ReturnValue < 0) {
          return;
        }
      }
    }
  }
}

/*********************************************************************
*
*       _PrintInt
*/
static void _PrintInt(SEGGER_RTT_PRINTF_DESC * pBufferDesc, int v, unsigned Base, unsigned NumDigits, unsigned FieldWidth, unsigned FormatFlags) {
  unsigned Width;
  unsigned Number;

  Number = (v < 0) ? -v : v;

  //
  // Get actual field width
  //
  Width = 1;
  while (Number >= Base) {
    Number = (Number / Base);
    Width++;
  }
  if (NumDigits > Width) {
    Width = NumDigits;
  }
  if ((FieldWidth > 0) && ((v < 0) || ((FormatFlags & FORMAT_FLAG_PRINT_SIGN) == FORMAT_FLAG_PRINT_SIGN))) {
    FieldWidth--;
  }

  //
  // Print leading spaces if necessary
  //
  if ((((FormatFlags & FORMAT_FLAG_PAD_ZERO) == 0) || (NumDigits != 0)) && ((FormatFlags & FORMAT_FLAG_LEFT_JUSTIFY) == 0)) {
    if (FieldWidth != 0) {
      while ((FieldWidth != 0) && (Width < FieldWidth--)) {
        _StoreChar(pBufferDesc, ' ');
        if (pBufferDesc->ReturnValue < 0) {
          return;
        }
      }
    }
  }
  //
  // Print sign if necessary
  //
  if (v < 0) {
    v = -v;
    _StoreChar(pBufferDesc, '-');
    if (pBufferDesc->ReturnValue < 0) {
      return;
    }
  } else if ((FormatFlags & FORMAT_FLAG_PRINT_SIGN) == FORMAT_FLAG_PRINT_SIGN) {
    _StoreChar(pBufferDesc, '+');
    if (pBufferDesc->ReturnValue < 0) {
      return;
    }
  }
  //
  // Print leading zeros if necessary
  //
  if (((FormatFlags & FORMAT_FLAG_PAD_ZERO) == FORMAT_FLAG_PAD_ZERO) && ((FormatFlags & FORMAT_FLAG_LEFT_JUSTIFY) == 0) && (NumDigits == 0)) {
    if (FieldWidth != 0) {
      while ((FieldWidth != 0) && (Width < FieldWidth--)) {
        _StoreChar(pBufferDesc, '0');
        if (pBufferDesc->ReturnValue < 0) {
          return;
        }
      }
    }
  }

  //
  // Print number without sign
  //
  _PrintUnsigned(pBufferDesc, v, Base, NumDigits, FieldWidth, FormatFlags);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       SEGGER_RTT_vprintf
*
*  Function description
*    Stores a formatted string in SEGGER RTT control block.
*    This data is read by the host.
*
*  Parameters
*    BufferIndex  Index of "Up"-buffer to be used. (e.g. 0 for "Terminal")
*    sFormat      Pointer to format string
*    pParamList   Pointer to the list of arguments for the format string
*
*  Return values
*    >= 0:  Number of bytes which have been stored in the "Up"-buffer.
*     < 0:  Error
*/
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList) {
  char c;
  SEGGER_RTT_PRINTF_DESC BufferDesc;
  int v;
  unsigned NumDigits;
  unsigned FormatFlags;
  unsigned FieldWidth;
  char acBuffer[SEGGER_RTT_PRINTF_BUFFER_SIZE];

  BufferDesc.pBuffer        = acBuffer;
  BufferDesc.BufferSize     = SEGGER_RTT_PRINTF_BUFFER_SIZE;
  BufferDesc.Cnt            = 0;
  BufferDesc.RTTBufferIndex = BufferIndex;
  BufferDesc.ReturnValue    = 0;

  do {
    c = *sFormat++;
    if (c == 0) {
      break;
    }
    if (c == '%') {
      //
      // Filter out flags
      //
      FormatFlags = 0;
      do {
        c = *sFormat;
        switch (c) {
        case '-': FormatFlags |= FORMAT_FLAG_LEFT_JUSTIFY; sFormat++; break;
        case '0': FormatFlags |= FORMAT_FLAG_PAD_ZERO;     sFormat++; break;
        case '+': FormatFlags |= FORMAT_FLAG_PRINT_SIGN;   sFormat++; break;
        case '#': FormatFlags |= FORMAT_FLAG_ALTERNATE;    sFormat++; break;
        default:  goto FilterFieldWidth;                   break;
        }
      } while (1);
      //
      // filter out field with
      //
FilterFieldWidth:
      FieldWidth = 0;
      do {
        c = *sFormat;
        if (c < '0' || c > '9') {
          break;
        }
        sFormat++;
        FieldWidth = FieldWidth * 10 + (c - '0');
      } while (1);

      //
      // Filter out precision (number of digits to display)
      //
      NumDigits = 0;
      c = *sFormat;
      if (c == '.') {
        sFormat++;
        do {
          c = *sFormat;
          if (c < '0' || c > '9') {
            break;
          }
          sFormat++;
          NumDigits = NumDigits * 10 + (c - '0');
        } while (1);
      }
      //
      // Filter out length modifier
      //
      c = *sFormat;
      do {
        if (c == 'l' || c == 'h') {
          c = *sFormat++;
          continue;
        }
        break;
      } while (1);
      //
      // Handle specifiers
      //
      switch (c) {
      case 'c': {
        char c0;
        v = va_arg(*pParamList, int);
        c0 = (char)v;
        _StoreChar(&BufferDesc, c0);
        break;
      }
      case 'd':
        v = va_arg(*pParamList, int);
        _PrintInt(&BufferDesc, v, 10, NumDigits, FieldWidth, FormatFlags);
        break;
      case 'u':
        v = va_arg(*pParamList, int);
        _PrintUnsigned(&BufferDesc, v, 10, NumDigits, FieldWidth, FormatFlags);
        break;
      case 'x':
      case 'X':
        v = va_arg(*pParamList, int);
        _PrintUnsigned(&BufferDesc, v, 16, NumDigits, FieldWidth, FormatFlags);
        break;
      case 's':
        {
          const char * s = va_arg(*pParamList, const char *);
          do {
            c = *s++;
            if (c == 0) {
              break;
            }
           _StoreChar(&BufferDesc, c);
          } while (BufferDesc.ReturnValue >= 0);
        }
        break;
      case 'p':
        v = va_arg(*pParamList, int);
        _PrintUnsigned(&BufferDesc, v, 16, 8, 8, 0);
        break;
      case '%':
        _StoreChar(&BufferDesc, '%');
        break;
      }
      sFormat++;
    } else {
      _StoreChar(&BufferDesc, c);
    }
  } while (BufferDesc.ReturnValue >= 0);

  if (BufferDesc.ReturnValue > 0) {
    //
    // Write remaining data, if any
    //
    if (BufferDesc.Cnt != 0) {
      SEGGER_RTT_Write(BufferIndex, acBuffer, BufferDesc.Cnt);
    }
    BufferDesc.ReturnValue += BufferDesc.Cnt;
  }
  return BufferDesc.ReturnValue;
}

/*********************************************************************
*
*       SEGGER_RTT_printf
*
*  Function description
*    Stores a formatted string in SEGGER RTT control block.
*    This data is read by the host.
*
*  Parameters
*    BufferIndex  Index of "Up"-buffer to be used. (e.g. 0 for "Terminal")
*    sFormat      Pointer to format string, followed by the arguments for conversion
*
*  Return values
*    >= 0:  Number of bytes which have been stored in the "Up"-buffer.
*     < 0:  Error
*
*  Notes
*    (1) Conversion specifications have following syntax:
*          %[flags][FieldWidth][.Precision]ConversionSpecifier
*    (2) Supported flags:
*          -: Left justify within the field width
*          +: Always print sign extension for signed conversions
*          0: Pad with 0 instead of spaces. Ignored when using '-'-flag or precision
*        Supported conversion specifiers:
*          c: Print the argument as one char
*          d: Print the argument as a signed integer
*          u: Print the argument as an unsigned integer
*          x: Print the argument as an hexadecimal integer
*          s: Print the string pointed to by the argument
*          p: Print the argument as an 8-digit hexadecimal integer. (Argument shall be a pointer to void.)
*/
int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...) {
  va_list ParamList;

  va_start(ParamList, sFormat);
  return SEGGER_RTT_vprintf(BufferIndex, sFormat, &ParamList);
}
/*************************** End of file ****************************/
