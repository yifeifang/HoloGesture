# 1.  Introduction

# 2.  Install

# 3.  Getting Started

## Pre-requests
Our code is fully written in C++ and build with Visual Studio 2019 Community.  

Our code is build targetting **x64** platform depends on   
**Azure kinect sensor SDK**  
**Azure kinect body tracking SDK**  
**OpenCV 4.5.0**

**You can choose to setup those dependency on your own from the official repositorys or you can choose to use our tested and prebuild package. If you would like to use our prebuild package please skip to [Installation section](#Installation)**

You can get Azure kinect sensor SDK from here
```
https://github.com/microsoft/Azure-Kinect-Sensor-SDK
```
Azure kinect body tracking SDK from here
```
https://docs.microsoft.com/en-us/azure/kinect-dk/body-sdk-download
```
OpenCV from here
```
https://docs.opencv.org/master/d5/de5/tutorial_py_setup_in_windows.html
```
## Installation
Assumed Azure Kinect DK hardware is already setup according to instruction here  
```
https://docs.microsoft.com/en-us/azure/kinect-dk/set-up-azure-kinect-dk
```
Download HoloGesture.zip file downloaded from this github page. Unzip and you will get a folder called **HoloGesture** with following layout
```
└── HoloGesture
    ├── HoloGesture.exe
    ├── VC_redist.x64.exe
    ├── depthengine_2_0.dll
    ├── dnn_model_2_0.onnx
    ├── k4a.dll
    ├── k4abt.dll
    ├── onnxruntime.dll
    └── opencv_world450.dll
```

Please run `VC_redist.x64.exe` to install visual c++ denpendency then run `HoloGesture.exe`. A terminal should pop-up and logging some necessery information.

Plesae refer to [Usage section](#Usage) for how to navigate in 3D with gestures
# 4.  Usage
After Launching `HoloGesture.exe` you can start to play around with gestures. Currently all the gesture presets are refer to absolute coordinate frame of Azure Kinect. Relative coordinate frame feature would be added in future releases.

**Supported Gestures**
1.  Display Desktop  
Right hand forward then backword will trigger a display desktop. Do it again will bring your desktop back.
![Display Desktop](https://github.com/yifeifang/HoloGesture/blob/gesture_object/gifs/desktop.gif)

2.  Multitask  
Right hand wave from right to left or from left to right to enable multitask view. Do it again goes back to your original screen.
![Multi-Task](https://github.com/yifeifang/HoloGesture/blob/gesture_object/gifs/multitask.gif)

3.  Volumn Adjustment  
Left hand come from far left and stop in approximately 30 degree to the left from center. Then moving up and down would adjust system volume accordingly. When finished just horizontally move away your hand.
![Volume](https://github.com/yifeifang/HoloGesture/blob/gesture_object/gifs/volume.gif)

4.  Cortana  
Left hand stay at lower left until conversation with Cortana done. Move away would close cortana window.
![Cortana](https://github.com/yifeifang/HoloGesture/blob/gesture_object/gifs/cortana.gif)

# 5.  Roadmap

# 6.  FAQs

# 7.  Contact

# 8.  Acknowledgements
