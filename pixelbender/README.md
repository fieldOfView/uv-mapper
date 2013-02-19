UV Mapper Tools: Pixel Bender
=============================
http://fieldofview.com/projects/uv-mapper

The UV Mapper Tools were created to provide an alternative to classic projection mapping animation and calibration techniques, but the tools can also be used for other purposes such as lens corrections for video. 

An Adobe Pixel Bender kernel which can be used to apply UV Maps to video content in Adobe After Effects. Given sufficiently capable hardware, the Pixel Bender kernel is executed on the GPU with realtime performance. Pixel Bender support requires After Effects CS4 or newer.

Adobe has abandoned Pixel Bender in After Effects CS6. Fortunately there's a third party plugin available that converts Pixel Bender kernels to GLSL on the fly so you can still use them in After Effects, with hardware acceleration:
http://aescripts.com/pixel-bender-kernel-accelerator/

Note that the After Effects composition must be set to use 16 bit per channel colordepth, or imported UV maps will loose their 16 bit precision and the results will look pixelated. 

Installation
------------
* Copy the uv-mapper.pbk file to [your profile folder]/Documents/Adobe/Pixel Bender
* Restart Adobe After Effects


License
-------
The UV Mapper Tools sources are released under the Modified BSD License.

In a nutshell, this means you are free to use the software and its source code in your projects. If you use the source code in another software project, you are required to add attribution to the author(s). You are encouraged to share the your source code under a similar fashion, but it is not a requirement.

If you use the software regularly, you may consider making a donation so I can afford to continue making these plugins and making them available. If you use the source code in a commercial application, and/or bundle such software with a hardware product, I would love to receive a copy or unit. But again this is not a requirement.