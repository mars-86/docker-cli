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

ECHO ====================
ECHO Compilation complete
ECHO ====================
PAUSE
