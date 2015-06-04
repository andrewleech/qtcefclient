@echo off
set SolutionDir=%1
set Configuration=%2
set Platform=%3
if "%SolutionDir%"=="" (
  echo Missing SolutionDir
  goto END
)
if "%Configuration%"=="" (
  echo Missing Configuration
  goto END
)
echo SolutionDir=%SolutionDir%
echo Configuration=%Configuration%

echo %Platform% | FIND /I "x64">Nul && (set ARCH=win64) || (set ARCH=win32)
echo ARCH=%ARCH%

set OutDir=%SolutionDir%%Configuration%\%Platform%\
echo OutDir=%OutDir%
mkdir %OutDir%
::echo d|xcopy %SolutionDir%%Configuration% %OutDir% /e /d /exclude:%SolutionDir%exclude.list
::echo n|copy /-y %SolutionDir%\cef_binary_%ARCH%\Resources\cef.pak %OutDir%
echo nnnnnnnnnn|xcopy /-y %SolutionDir%\cef_binary_%ARCH%\Resources\* %OutDir%
echo n|copy /-y %SolutionDir%\cef_binary_%ARCH%\Resources\devtools_resources.pak %OutDir%
echo n|copy /-y %SolutionDir%\cef_binary_%ARCH%\%Configuration%\*.dll %OutDir%
echo n|copy /-y %SolutionDir%\cef_binary_%ARCH%\%Configuration%\*.exe %OutDir%
mkdir %OutDir%locales
echo n|copy /-y %SolutionDir%\cef_binary_%ARCH%\Resources\locales\en-US.pak %OutDir%\locales

pushd %QT_HOME%\bin

echo n|copy /-y icuin53.dll %OutDir%
echo n|copy /-y icuuc53.dll %OutDir%
echo n|copy /-y icudt53.dll %OutDir%
mkdir %OutDir%platforms

if "%Configuration%"=="Debug" (
  echo n|copy /-y Qt5Cored.dll  %OutDir%
  echo n|copy /-y Qt5Widgetsd.dll  %OutDir%
  echo n|copy /-y Qt5Guid.dll %OutDir%
  echo n|copy /-y ..\plugins\platforms\qwindowsd.dll %OutDir%\platforms\
  popd
  goto END
)
echo n|copy /-y Qt5Core.dll  %OutDir%
echo n|copy /-y Qt5Widgets.dll  %OutDir%
echo n|copy /-y Qt5Gui.dll %OutDir%
echo n|copy /-y ..\plugins\platforms\qwindows.dll %OutDir%\platforms\
popd

:END
rem @pause>nul