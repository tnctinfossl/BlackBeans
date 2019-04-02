@REM This script allows a 3rd party IDE to use cyelftool.exe to perform
@REM the post processing that is necessary to update the *.elf file
@REM generated by the linker into a complete memory image to use in
@REM programming the PSoC.
@REM NOTE: This script is auto generated. Do not modify.
set buildDir=%PWD%
chdir /d ..\.\Export
@IF %errorlevel% NEQ 0 EXIT /b %errorlevel%
cyelftool.exe -C "%~1" --flash_row_size 256 --flash_size 262144 --flash_offset 0x00000000 
@IF %errorlevel% NEQ 0 EXIT /b %errorlevel%
