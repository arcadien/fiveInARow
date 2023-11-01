dir %1

avrdude\bin\avrdude  ^
    -patmega328p ^
    -carduino    ^
    -b57600     ^
    -Pcom11      ^
    -e           ^
    -C avrdude\etc\avrdude.conf ^
    -v -v        ^
    -D "-Uflash:w:%1:i"