@echo off
setlocal EnableDelayedExpansion

REM ��������
set devenvcmd=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\devenv.com


REM ����Ľ������
set slnFile1=CxImgLibVC2010.sln


"%devenvcmd%" "%slnFile1%" /Clean "Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Release|Win32"

"%devenvcmd%" "%slnFile1%" /Clean "Unicode Debug|Win32"  
"%devenvcmd%" "%slnFile1%" /Build "Unicode Debug|Win32"  

"%devenvcmd%" "%slnFile1%" /Clean "Unicode Release|Win32"
"%devenvcmd%" "%slnFile1%" /Build "Unicode Release|Win32"

pause