/***************************************************************************************************
 * File:         $URL: https://svn.bolitho.us/Library/Trunk/Src/IO/SerialPort.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1631 $
 * Last Updated: $Date: 2008-09-15 07:52:23 -0700 (Mon, 15 Sep 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/




#include "CodeLibrary.hpp"

#include "IO/SerialPort.hpp"

using namespace Bolitho;
using namespace Bolitho::IO;

//-----------------------------------------------------------------------------------------------------------------------------------------------------
ImplementRuntimeType(Bolitho::IO,SerialPort,Stream);
ImplementRuntimeType(Bolitho::IO,SerialPortException,Exception);
//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_WINDOWS)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SerialPort::SerialPort(FILEHANDLE hPort)
{
  m_hPort = hPort;
  m_PortInfo.DCBlength = sizeof(m_PortInfo);

  GetCommState(m_hPort, &m_PortInfo);

  m_PortInfo.ByteSize = 8;
  SetCommState(m_hPort, &m_PortInfo);

  SetupComm(m_hPort, 1024, 1024);
  PurgeComm(m_hPort, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<SerialPort> SerialPort::TryOpen(LPCTSTR DeviceName)
{
  HANDLE hPort = CreateFile(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
  if (hPort == INVALID_HANDLE_VALUE)
    Ptr<SerialPort>::Null;
  return new SerialPort(hPort);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<SerialPort> SerialPort::Open(LPCTSTR DeviceName)
{
  HANDLE hPort = CreateFile(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
  if (hPort == INVALID_HANDLE_VALUE)
    throw SerialPortException(0, SystemException::GetError());
  return new SerialPort(hPort);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Flush()
{
  FlushFileBuffers(m_hPort);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Close()
{
  CloseHandle(m_hPort);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T SerialPort::Write(LPCVOID pData, SIZE_T N)
{
  SIZE_T TotalBytesWritten = 0;

  while (TotalBytesWritten < N)
  {
    DWORD BytesWritten = 0;
    if (!WriteFile(m_hPort, (LPCBYTE)pData + TotalBytesWritten, (DWORD)(N - TotalBytesWritten), &BytesWritten, 0))
      SystemException::ThrowError(this);
    TotalBytesWritten += BytesWritten;
  }

  return TotalBytesWritten;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T SerialPort::Read(LPVOID pData, SIZE_T N)
{
  /* Tries to read N bytes into pData.  May timeout and not return all data */
  BOOL Timeout = FALSE;

  SIZE_T TotalReadBytes = 0;
  while (TotalReadBytes < N)
  {
    DWORD ReadBytes = 0;
    if (!ReadFile(m_hPort, (LPBYTE)pData + TotalReadBytes, (DWORD)(N - TotalReadBytes), &ReadBytes, 0))
      SystemException::ThrowError(this);
    
    if (ReadBytes == 0)
    {
      if (Timeout)
        return TotalReadBytes;
      else
        Timeout = TRUE;
    }

    TotalReadBytes += ReadBytes;
  }

  return TotalReadBytes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SerialPort::EndOfStream()
{
  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetBaudRate() CONST
{
  return m_PortInfo.BaudRate;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetParity() CONST
{
  return m_PortInfo.Parity;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetStopBits() CONST
{
  return m_PortInfo.StopBits;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetBaudRate(DWORD BaudRate)
{
  m_PortInfo.BaudRate = BaudRate;
  SetCommState(m_hPort, &m_PortInfo);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetParity(DWORD Parity)
{
  m_PortInfo.Parity = (BYTE)Parity;
  SetCommState(m_hPort, &m_PortInfo);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetStopBits(DWORD StopBits)
{
  m_PortInfo.StopBits = (BYTE)StopBits;
  SetCommState(m_hPort, &m_PortInfo);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetTimeout(DWORD ReadTimeout, DWORD WriteTimeout)
{
  COMMTIMEOUTS Timeouts;
  GetCommTimeouts(m_hPort, &Timeouts);
  Timeouts.ReadTotalTimeoutConstant = ReadTimeout;
  Timeouts.WriteTotalTimeoutConstant = WriteTimeout;
  SetCommTimeouts(m_hPort, &Timeouts);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Purge()
{
  PurgeComm(m_hPort, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------------
#if defined(PLATFORM_POSIX)
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SerialPort::SerialPort(FILEHANDLE hPort)
{
  m_hPort = hPort;


  if (tcgetattr(m_hPort, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError());

  m_PortInfo.c_oflag &= ~OPOST;
  m_PortInfo.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  m_PortInfo.c_iflag &= ~(INLCR | IGNCR | ICRNL);
#if !defined(PLATFORM_FREEBSD) 
  m_PortInfo.c_line = 0;
#endif
  m_PortInfo.c_cflag |= CREAD | CLOCAL;
  m_PortInfo.c_cc[VMIN] = 0;
  m_PortInfo.c_cc[VTIME] = 10;
  
  if (tcflush(m_hPort, TCIOFLUSH) < 0)
    throw SerialPortException(this, SystemException::GetError());

  if (tcsetattr(m_hPort, TCSANOW, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError());

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ptr<SerialPort> SerialPort::Open(LPCTSTR DeviceName)
{
  FILEHANDLE hFile = open(DeviceName, O_RDWR | O_NOCTTY);
  if (hFile == -1)
    throw SerialPortException(0, SystemException::GetError(0));
  fcntl(hFile, F_SETFL, 0);

  return new SerialPort(hFile);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Flush()
{
  if (tcflush(m_hPort, TCIOFLUSH) < 0)
    throw SerialPortException(this, SystemException::GetError());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Close()
{
  if (m_hPort == 0)
    return;

  if (close(m_hPort) < 0)
    throw SerialPortException(this, SystemException::GetError());
  m_hPort = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T SerialPort::Write(LPCVOID pData, SIZE_T N)
{
  SIZE_T TotalBytesWritten = 0;

  while (TotalBytesWritten < N)
  {
    int Result = write(m_hPort, (LPCBYTE)pData + TotalBytesWritten, (DWORD)(N - TotalBytesWritten));
    if (Result < 0)  
      throw SerialPortException(this, SystemException::GetError(this));
    else
      TotalBytesWritten += Result;
  }

  return TotalBytesWritten;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
SIZE_T SerialPort::Read(LPVOID pData, SIZE_T N)
{
  SIZE_T TotalReadBytes = 0;
  while (TotalReadBytes < N)
  {
    int Result = read(m_hPort, (LPBYTE)pData + TotalReadBytes, N - TotalReadBytes);
    if (Result < 0)
      throw SerialPortException(this, SystemException::GetError(this));
    else if (Result > 0)
      TotalReadBytes += Result;
    else if (Result == 0)
      break;
  }

  return TotalReadBytes;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL SerialPort::EndOfStream()
{
  return FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetBaudRate() CONST
{
  speed_t BaudRate = cfgetospeed(&m_PortInfo);

  switch(BaudRate)
  {
  case B1200:
    return 1200;
  case B2400:
    return 2400;
  case B4800:
    return 4800;
  case B9600:
    return 9600;
  case B19200:
    return 19200;
  case B38400:
    return 38400;
  case B57600:
    return 57600;
  case B115200:
    return 115200;
  case B230400:
    return 230400;
  };
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetParity() CONST
{
  if (m_PortInfo.c_cflag & PARENB)
    return (m_PortInfo.c_cflag & PARODD) ? Parity::ODD : Parity::EVEN;
  return Parity::NONE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
DWORD SerialPort::GetStopBits() CONST
{
  return (m_PortInfo.c_cflag & CSTOPB) ? StopBits::TWO : StopBits::ONE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetBaudRate(DWORD BaudRate)
{
  speed_t Baud = 0;
  switch(BaudRate)
  {
  case 1200:
    Baud = B1200; break;
  case 2400:
    Baud = B2400; break;
  case 4800:
    Baud = B4800; break;
  case 9600:
    Baud = B9600; break;
  case 19200:
    Baud = B19200; break;
  case 38400:
    Baud = B38400; break;
  case 57600:
    Baud = B57600; break;
  case 115200:
    Baud = B115200; break;
  case 230400:
    Baud = B230400; break;
  };

  if ((cfsetispeed(&m_PortInfo,Baud) < 0) || (cfsetospeed(&m_PortInfo,Baud) < 0))
    throw SerialPortException(this, SystemException::GetError(this));

  if (tcsetattr(m_hPort, TCSANOW, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError(this));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetParity(DWORD Parity)
{
  switch(Parity)
  {
  case Parity::EVEN:
    m_PortInfo.c_cflag |= PARENB;
    m_PortInfo.c_cflag &= ~PARODD;
    m_PortInfo.c_iflag |= INPCK;
    break;
  case Parity::ODD:
    m_PortInfo.c_cflag |= (PARENB | PARODD);
    m_PortInfo.c_iflag |= INPCK;
    break;
  case Parity::NONE:
    m_PortInfo.c_cflag &= ~(PARENB);
    m_PortInfo.c_iflag |= IGNPAR;
    break;
  default:
    throw InvalidArgumentException(this, "Invalid or Unsupported Parity");
  };
  
  if (tcsetattr(m_hPort, TCSANOW, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetStopBits(DWORD StopBits)
{
  switch(StopBits)
  {
  case StopBits::ONE:
    m_PortInfo.c_cflag &= ~(CSTOPB) ;
    break;
  case StopBits::TWO:
    m_PortInfo.c_cflag |= CSTOPB ;
    break;
  default:
    throw InvalidArgumentException(this, "Invalid or Unsupported StopBits");
  };
  
  if (tcsetattr(m_hPort, TCSANOW, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::SetTimeout(DWORD ReadTimeout, DWORD WriteTimeout)
{
  if (ReadTimeout > 25000)
    m_PortInfo.c_cc[VTIME] = 255;
  else
    m_PortInfo.c_cc[VTIME] = (cc_t)((ReadTimeout / 100) + 1);

  if (tcsetattr(m_hPort, TCSANOW, &m_PortInfo) < 0)
    throw SerialPortException(this, SystemException::GetError());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------
VOID SerialPort::Purge()
{
  while (TRUE)
    {
      BYTE B;
      int Result = read(m_hPort, &B, 1);
      if (Result < 0)
	throw SerialPortException(this, SystemException::GetError(this));
      if (Result == 0)
	break;
    }
}
#endif
//-----------------------------------------------------------------------------------------------------------------------------------------------------
