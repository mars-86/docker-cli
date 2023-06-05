@ECHO off

cd setup
make install
cd ..

cd installer
make install
cd ..

cd daemon
make install
cd ..

cd cli
make install
cd ..

cd gui
make install
cd ..

cd init
make install
cd ..

echo @ECHO off > install.bat
echo cd .\setup\bin >> install.bat
echo .\setup >> install.bat
echo cd ..\.. >> install.bat

ECHO ====================
ECHO Compilation complete
ECHO ====================
PAUSE
