:: https://github.com/appjs/appjs/wiki/Building-Appjs
pushd %~dp0
pushd ..\..
set PATH=%CD%;%CD%\node-4.2.1-x86;%CD%\..;%CD%\..\cmake\bin;%PATH%
popd

:: get msys out of the path first then...

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86
::call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" amd64

cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..\qcef3_2272
ninja qt_cefclient

popd