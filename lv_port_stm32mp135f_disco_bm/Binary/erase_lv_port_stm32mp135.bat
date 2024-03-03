:: STM32CubeProgrammer Utility flash script

@ECHO ON
@setlocal
::COLOR 0B

:: Current Directory
@SET CUR_DIR=%CD%

:: TSV filename
@SET TSV_FILE="FlashLayout_lv_port_stm32mp135_deleteall.tsv"
@IF NOT EXIST %TSV_FILE% @SET TSV_FILE="flashlayout\\%TSV_FILE%"

:: USB Port ID
@SET USB_ID=USB1

@SET STM32_PROGRAMMER_PATH="%ProgramFiles(x86)%\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
@IF NOT EXIST %STM32_PROGRAMMER_PATH% @SET STM32_PROGRAMMER_PATH="%ProgramW6432%\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
@IF NOT EXIST %STM32_PROGRAMMER_PATH% @SET STM32_PROGRAMMER_PATH="%ProgramFiles%\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"
@IF NOT EXIST %STM32_PROGRAMMER_PATH% @ECHO STM32CubeProgrammer is not installed, please download and install it from https://www.st.com/en/development-tools/stm32cubeprog.html !! && GOTO goError

TITLE STM32CubeProgrammer Utility for %CHIP_NAME%%MAIN_BOARD%

:: Add STM32CubeProgrammer to the PATH
@SET PATH=%STM32_PROGRAMMER_PATH%;%PATH%

@ECHO.
@ECHO ===================================================
@ECHO Erasing with %TSV_FILE%
@ECHO ===================================================
@ECHO. 
STM32_Programmer_CLI.exe -c port=%USB_ID% -d %TSV_FILE% -detach -vb 1
@IF NOT ERRORLEVEL 0 (
  @GOTO goError
)

@GOTO goOut

:goError
@SET RETERROR=%ERRORLEVEL%
@COLOR 0C
@ECHO.
@ECHO Failure Reason Given is %RETERROR%
@PAUSE
@COLOR 07
@EXIT /b %RETERROR%

:goOut
PAUSE