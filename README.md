# sumuri-recon-lab-qt (Silicon)
Repository for the RECON LAB macOS application developers in C++ using QT. This is for the *Silicon* version of the application.

### Building Qt from source
The application needs to be compiled using the no-framework version of Qt. This means that it needs to be compiled from source using the `no-framework` flag. The following steps will walk you through building Qt from source.

- Create a Qt account if you don't already have one by going to https://login.qt.io/register and filling out the form.
-  Login to the Qt customer portal and go to the Downloads page at https://account.qt.io/s/downloads
- Select *Qt* from the Product drop-down menu and then select *Qt 6.2.4* from the Version drop-down.
- Select the *Qt Source Package, Full Framework with UNIX style line endings* option and download the file named *qt-everywhere-src-6.2.4.tar.xz*.
- Extract the file into a location where you want to install Qt. The folder with all the source files will be named *qt-everywhere-src-6.2.4*
- CD into the folder and run the following commands to configure Qt:
```
./configure -no-framework -prefix $PWD/qtbase
```
- Run the following command to build and install Qt:
```
cmake --build . --parallel
```
- Add the Qt version to Creator by following the steps in this article: https://doc.qt.io/qtcreator/creator-project-qmake.html#setting-up-new-qt-versions You can name the kit *Qt 6.2.4 (no-framework)*
- Open the project file `RECON_ITR.pro` and select the *Qt 6.2.4 (no-framework)* kit and click the *Configure Project* button. This is only required the first time you open the project.
- Build the project.


## Building the application
- Install Git Large File Storage (LFS) by following the instructions at https://git-lfs.com/
- Install the RECON LAB dependencies from the [RECON LAB Manual](https://sumuri.com/recon-lab-manual/).
- Install *openssl* via Homebrew: `brew install openssl`
- Install *dlib* via Homebrew: `brew install dlib`
- Comment out the following line in the file `/opt/homebrew/Cellar/dlib/19.24_1/include/dlib/config.h`:
```
#define DLIB_NO_GUI_SUPPORT
```
- Replace `stat64` with `stat` in the following file `/opt/homebrew/include/dlib/dir_nav/dir_nav_kernel_2.h`
- Install *sleuthkit* via Homebrew: `brew install sleuthkit`
- Install hivex using Mac Ports: `sudo port install hivex`. If you don't have Mac Ports then you can build and install from source by downloading from https://github.com/digitalocean/hivex/tree/do/trusty

- Build the project.

## Running the application
- Get the serial number of the USB drive you'll be using. You can get this by running the following command in Terminal: `system_profiler SPUSBDataType` and looking for the serial number of the USB drive you'll be using.
- Generate a license file using the serial number and copy it to the root of the USB drive.
- Run the application and click *OK* on the dialog that appears indicating that the license file is missing.
- When the main screen opens, click the *Load License* button and select the license file you generated in the previous step. 

## Preparing the application for release
To prepate the apllication for release, you will need to update the app bundle by running `bundle.sh` script. You will also need the private key and certificate for the Developer ID Application certificate. You can get this from the Apple Developer portal.

- CD into the build folder and run the `bundle.sh` command and pass in the path to the Qt bin folder to create an app bundle:
```
./bundle.sh /Applications/Qt/Qt6.2.4/bin
```
The script can also be run with the `-code-sign-only` flag to skip the bundling process and only sign the app bundle and dylibs. This is useful if you need to re-sign after making changes to the app bundle.

```
./bundle.sh -code-sign-only
```

*Note*: If you get an error about `(file system sandbox blocked open())` You will need to allow full disk access to the tool and run the following commands. You may need to restart the terminal session.

```
sudo chmod -R 755 /path/to/QtFolder
sudo chown -R $(whoami) /path/to/QtFolder
```

If you see errors like the following, those can be ignored. They are probably caused by the existing plugins that were compiled on the provisioned Sumuri machines. They don't affect the process and we can investigate them later.
```
ERROR: no file at "/usr/lib/libm_tv.1.dylib"
ERROR: no file at "/usr/lib/libm_utorrent.1.dylib"
ERROR: no file at "/usr/lib/libm_safari.1.dylib"
...
```
