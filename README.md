# Talk² Arduino Library #
This is the official Arduino Library for the Talk² Boards, like Whisper Node. Please note that the official repository is hosted at Bitbucket (https://bitbucket.org/talk2/talk2-library) but we also maintain a clone of the same repository at the GitHub (https://github.com/talk2wisen/Talk2Library) for the Arduino IDE requirements.

## Installation ##
This library is supported on the recent version of Arduino IDE >= 1.6.2. You can install it directly from the Arduino IDE by opening the menu "Sketch" -> "Include Library" -> "Manage Libraries...". Search for "Talk2", select the most recent version and click "Install":

### Step 1 ###
![ArduinoIDE_LibManager.png](https://bitbucket.org/repo/9Lg6Rx/images/1267641627-ArduinoIDE_LibManager.png)

### Step 2 ###
![ArduinoIDE_InstallLib.png](https://bitbucket.org/repo/9Lg6Rx/images/2848618934-ArduinoIDE_InstallLib.png)

You also can manually install this library by creating a folder named "libraries" inside your "Sketches" folder and copying all content there. You'll need to restart the Arduino IDE to use the Library.

## Requirements ##

### Low-Power ###
The Talk² Library requires the Low-Power third-party library. The library can be installed using the Arduino IDE Library manager, just search for "power management". The library is also available at https://github.com/rocketscream/Low-Power and a recent copy has been included inside the "extras" directory of the Talk² Library.

### RadioHead ###
The Talk² Library requires the RadioHead third-party library. The latest version can be downloaded from http://www.airspayce.com/mikem/arduino/RadioHead/. The documentation is very comprehensive and can be accessed on the same Website. Additionally a recent copy of the RadioHead library is also available inside the "extras" directory on the Talk² Library for convenience.

To install the RadioHead library follow the steps:

#### Confirm where is your Sketches folder ####
![ArduinoIDE_Preferences.png](https://bitbucket.org/repo/9Lg6Rx/images/2461214519-ArduinoIDE_Preferences.png)

*Menu "File" -> "Preferences"*

#### Locate the Talk2 Library ####
![ArduinoIDE_LibLocation.png](https://bitbucket.org/repo/9Lg6Rx/images/3981450619-ArduinoIDE_LibLocation.png)

#### "Copy" the RadioHead from the ZIP inside the "Talk2\extras" ####
![ArduinoIDE_RHInstall.png](https://bitbucket.org/repo/9Lg6Rx/images/2770945013-ArduinoIDE_RHInstall.png)

#### "Paste" the RadioHead folder inside the Libraries ####
![ArduinoIDE_RHInstalled.png](https://bitbucket.org/repo/9Lg6Rx/images/1416790794-ArduinoIDE_RHInstalled.png)

#### Restart the Arduino IDE ####

#### Check Libraries are Present ####
![ArduinoIDE_LibsInstalled.png](https://bitbucket.org/repo/9Lg6Rx/images/1480042379-ArduinoIDE_LibsInstalled.png)