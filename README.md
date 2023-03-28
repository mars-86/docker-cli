<h1 align="center">Docker CLI for Windows</h1>

## Get docker cli
At the moment the docker cli to download is a beta version  
You can download it from releases page https://github.com/mars-86/docker-cli/releases

## Overview
Use docker on windows from command line.

## Installation
#### Prerequisites
##### Windows subsystem for linux
- turn windows features on or off
- enable windows subsystem for linux
- enable virtual machine platform
- wsl --update
- wsl --set-default-version 2

##### Mingw
You will need to install mingw if you download the docker-cli version without libraries.

#### Install docker CLI
Just run install.bat and it will install docker cli on user's root folder
![Docker CLI for Windows](https://github.com/mars-86/static-assets/raw/main/images/docker-cli/docker-cli-002.jpg)

## Usage
Run docker command from console.
![Docker CLI for Windows](https://github.com/mars-86/static-assets/raw/main/images/docker-cli/docker-cli-001.jpg)

## Building for source
- Install mingw https://www.mingw-w64.org/
- Add bin folder to path
- Clone the repository
- Run compile.bat

It should generate a bin folder with an executable file inside every module and install.bat on root folder

### :warning: Antivirus
You may have problems due to antivirus, specifically McAfee.

### Troubleshooting
- The operation could not be started because a required feature is not installed.  
    -> enable virtual machine platform (https://github.com/mars-86/docker-cli/tree/main#prerequisites).
- Daemon does not starts on init or stops unexpectedly  
    -> go to %USERPROFILE%\docker-cli\daemon\ and run dockerd.exe manually.
- WSL stops instances after windows sleeps (https://github.com/microsoft/WSL/issues/8763)  
    -> go to edit plan settings and change all the options in "choose what the power buttons do" to hibernate,  
    also go to "change when the computer sleeps" and set the options under "put the computer to sleep" to never.

## License
MIT
