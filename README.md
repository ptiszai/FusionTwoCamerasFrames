### FusionTwoCamerasFrames
Fusion (blender) are frames of color video and frames of infra video in real-time .<br> 
Image processing to be useing OpenCV ! Version is 3.1.0.<br>
### Installation
Microsoft Visual Studio Community C++ 2015, Update 3.<br>
Open "FusionTwoCamerasFrames.sln".
### Dependencies:
Provided you already have OpenCV 3.1.0 x64 package is completely, if none, than ... 
this to be downloaded with NutGet.<br>
In Solution Explorer selected with mouse the "WireDetecor" project, then right click, after selected "Manage NuGet Packages...".<br>
Search OpenCV in online.<br>
Instruction, see the picture: "doc/opencv.png"<br><br>
![Opencv](doc/opencv.png)

Auto created "packages" directory in the base (root) directory: .\packages\opencvdefault.3.1.0\*
### Build
You compile the project.<br>
If it is ready, in the .\x64\Release\ directory created "FusionTwoCamerasFrames.exe" executed application.
### Running program.
You find *avi video files (2 pieces) in the Google Drive:
https://drive.google.com/drive/folders/0B9rmVYV9c3x8TUstbzBYN1I2MTQ?usp=sharing <br>
Name of video list:<br>
proba1Color.mp4  : 2  Mb<br>
proba1Ir.mp4 : 2 Mb<br>
You download, after them copy to .\x64\Release\ directory.<br>
Running is "FusionTwoCamerasFrames.exe".<br>
### Description
Keys:<br>
' ', space key: toggle command: start/stop frames of video.<br>
'q', quit  key: exit program.<br>

### Binary release files:
Google Drive: https://drive.google.com/drive/folders/0B9rmVYV9c3x8TUstbzBYN1I2MTQ?usp=sharing <br>
FusionTwoCamerasFrames-binary.zip<br>
vc_redist.x64.exe<br>

### Result:
From 5 mins 10 secs in the video: https://www.youtube.com/watch?v=uoD2yhA_56o&feature=share

### Source:
Dual camera system. Card is RaspBerry Pi 3<br>
Color camera is 5 megapixels<br>
Infra camera is FLIR Lepton: https://www.sparkfun.com/products/13233

Hardware picture:
![Rpb Pi3twice Camera2](doc/RpbPi3twiceCamera2.jpg)










