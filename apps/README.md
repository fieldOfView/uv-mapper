UV Mapper Tools: Apps
=====================
http://fieldofview.com/projects/uv-mapper

The UV Mapper Tools were created to provide an alternative to classic projection mapping animation and calibration techniques, but the tools can also be used for other purposes such as lens corrections for video. 

The main tools suite uses Cinder (http://www.libcinder.org) and supports Mac OS X and Windows.

#### UV Player
The UV Player is used apply a UV map to deform a source video. Videos can be played back in real-time (or as near to real time as hardware allows), and they can be exported to a new video. The UV Player currently uses Quicktime to display video, so playback is limited by the codecs supported by that framework. The same limitation applies for exporting video. 

#### UV Tool
The UV Tool is used to create and edit UV maps for use with the UV Player. It is geared towards creating calibrated projections on arbitrary geometry, using a structured light scan of the projected surface.

Compiling
---------
In order to compile the tools from the source, you need a working fork of the libCinder project (including boost and the Quicktime SDK for Windows). Please refer to this page about installing libCinder:
http://libcinder.org/docs/welcome/GitSetup.html

If you want to be sure you are working with the same branch and version of the libCinder framework that is used to create the official binaries for the project, you may want to check out this fork of the Cinder project:
https://github.com/fieldofview/cinder

License
-------
The UV Mapper Tools sources are released under the Modified BSD License.

In a nutshell, this means you are free to use the software and its source code in your projects. If you use the source code in another software project, you are required to add attribution to the author(s). You are encouraged to share the your source code under a similar fashion, but it is not a requirement.

If you use the software regularly, you may consider making a donation so I can afford to continue making these plugins and making them available. If you use the source code in a commercial application, and/or bundle such software with a hardware product, I would love to receive a copy or unit. But again this is not a requirement.