<h1 align="center">Docker CLI for Windows</h1>

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
    -> enable virtual machine platform (intallation prerequisites).
- Daemon does not starts on init
    -> go to %USERPROFILE%\docker-cli\daemon\ and run dockerd.exe manually.

## License
MIT
