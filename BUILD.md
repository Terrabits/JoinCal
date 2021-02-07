# Build Instructions for Join Cal

## Requirements

-   Qt 5.2.1 built with Visual C++ 2010 x86
-   Python (2 or 3)
-   Windows Installer XML (WIX) 3.11+

**Note that the this project and RsaToolbox will not compile with newer versions of Qt and/or Visual C++ without a moderate effort to update.**

## Build

Open `JoinCal.pro` in  Qt Creator.

### Build and Deploy RsaToolbox

Next, build subproject `RsaToolbox`.

**Unfortunately, `RsaToolbox` requires a manual build step.** Run the `RsaToolbox/install.py` script to "deploy" it to the `RsaToolbox/Install` folder.

### Build All

Once `RsaToolbox` is built, the entire project can be [re]built without incident.

## Distribute

The installer for Join Cal was built with WIX. Installer files are located in `Installer/`.
.

### Binary Files

All binary files must be manually copied to `[ProgramFiles]/Rohde-Schwarz/Join Cal/`. A list of the required files can be found in `Product.wxs`:

From the `release` build:

-   `R&S Join Cal.exe`

From the Qt installation:

-   `Platforms/qwindows.dll`
-   `D3DCompiler_43.dll`
-   `icudt51.dll`
-   `icuin51.dll`
-   `icuu51.dll`
-   `libEGL.dll`
-   `libGLESv2.dll`
-   `Qt5Core.dll`
-   `Qt5Gui.dll`
-   `Qt5Network.dll`
-   `Qt5Widgets.dll`

From R&S VISA:

-   `RsVisa32.dll`

From Microsoft Visual C++ 2010 (VC++ Runtime Redistributable):

-   `msvcp100.dll`
-   `msvcr100.dll`

From the command line, **and assuming the WIX bins are in path**, execute the following to create the installer:

```shell
cd path/to/JoinCal
candle Product.wxs -ext WixUtilExtension -ext WixUIExtension -out Product.wixobj
light Product.wixobj -spdb -ext WixUtilExtension -ext WixUIExtension -out "R&S Join Cal <version>.msi"
```
