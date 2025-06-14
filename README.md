## Advanced WiFi enabled USB 'rubber ducky'

more info at [www.aledthomas.com](https://www.aledthomas.com/)

Designed for ESP32 (specifically WEMOS S2 but should work with any board with USB HID support)

### Usage
Use in AP mode: set your preffered SSID/Password. Connect to the access point and browse to
http://192.168.4.1 (the default ESP32 address).

or in WiFi client mode (uncomment the "#define WIFICLIENT" line and edit the SSID and password values), 
the  IP address will be assigned by DHCP so you can find it on the network, it will also be output over 
serial (connect while plugged in to your own computer first and make a note of it).

Once you browse to the the WebUI page you will find a dropdown list of all the scripts saved on the device
(the app will look for a script named "default.txt" and load it if it's found on startup). Choose one
and click 'load', the currently loaded script will be shown in the box below. Scripts can also be deleted
from the list if they are no longer needed. Click 'run' to run the loaded script, or 'edit' to open the
editor page where you can add or remove lines and values in the script can be changed as needed. Click
'new' to add a new script.

From the editor screen you can save your edited/new script, you will be prompted for a filename before saving.


### Supported script commands
Supports scripts written for [duckyscript v1](https://web.archive.org/web/20220606164510/https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript) with a few added extra commands, this does not include
many of the more advanced scripting features added in the latest duckyscript v3 although REM/STRING
blocks will be understood. Also supports many of the extended commands used in scripts written for 
the 'badUSB' feature on the flipper zero.
extra features not in duckyscript v1-
- mouse control (flipper style)
- HOLD and RELEASE multiple keys including modifier keys (flipper style)
- DEFAULTSTRINGDELAY and STRINGDELAY to control typing speed (flipper style)
- random mouse moves :)
- random keystrokes

### Building
This sketch uses littleFS for onboard storage, before using please use the [littleFS uploader plugin](https://github.com/earlephilhower/arduino-littlefs-upload)
in arduino IDE to prepare the storage and upload and scripts (put them in the data/ subdirectory).
The compiled sketch should fit into <1MB so for a chip with 4MB flash you can set the partitions to 
"No OTA (1MB APP/3MB SPIFFS)" and have plenty of room for a ton of scripts.

### TODO coming soon
- jitter (randomize typing speed)
- loops
- exfil of data via SD card mounted as mass storage
- infil of binaries etc. via SD card mounted as mass storage
- exfil of data on SD card via WiFi connection and webUI


### Adding/changing commands
Commands can be added in the file "commands.h", uncomment the line "#define CALCULATE_HASHES" then
compile and upload the sketch and the ESP32 will generate a new "hashes.h" file for you and output it 
back over the serial monitor. You can just copy and paste it in place of the old one.
