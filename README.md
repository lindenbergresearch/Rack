# Rack for Apple Silicon (arm64)

This is an unofficial release of VCV Rack ported to Apple Silicon Architecture based on *arm64*.
It basically relates on v1.1.6 but I added some optimizations and experimental graphics extensions.

These version needs at least an **M1 based Mac** and macOS **Big Sur**.


## Update 01/06/2021

I updated the sources und pushed all changes to github. I am sorry for the bad git comments and noisy reformatting, 
I was a bit busy and lazy... you may have to figure out yourself. Don't hesitate to ask me :-)


**WARNING:** *This is an experimental version, some features may not work proper or remain unfinished. 
Make backups and use with care.*


Here are some of the changes included in this version:

- **Logger:** A TRACE log-level has been added, default global level is set to: DEBUG. Logging is done to console **and** 
  to filesystem now (for later analysing...). Colors and formatting have been changed for better readability.
- SVGs are buffered now as raster-graphics (oversampled for quality), which is eating more 
  RAM but increases speed on zooming dramatically especially on slow systems.
- It uses now an own adapted version of blendisch containing a lot of UI tweaks and changes. 
  Also the overall font has been changed to: Asimov and setup a bit bigger.
- CPU and GPU monitoring has been extended: Now it measures also the step-time and draw-time. 
  Design has been changed and the sum of all modules usage is displayed on the right side.
- The cable drawing has been changed and a bit humanized and the colors changed to look a bit more funky baby.
- v-sync could be enabled via settings.json

For more details please look at the git commits or contact me.

In general all UI and graphics tweaks and changes could also run on Intel machines and on Linux and Windows. 


## 1. Caution
---

__This is an experimental version of Rack, use it 'as is' and on your own risk. Save your patches and backup files.__


## 2. General information
---

Please note the following points before starting:

- this is an experimental release mainly to explore the possibilities of the Apple architecture 
- Apple Silicon based Mac's are able to run the current version of VCV Rack fine while using Rosetta 2. The only drawback is that the performance is not optimized for that platform, despite it runs very fast. For production usage I recommend using an official build: [VCV website](https://vcvrack.com/)  
- all online functions have been **disabled** (Update, Login etc.)
- no current available plugins build for the official release will work with this version, because they are not binary compatible. You have them compile yourself or use a prebuild binary.
- to avoid clashes this version uses a different folder to store data and plugins: __~/Documents/Rack.arm64__
- this version is using v-sync with a framerate limiter to improve graphics. You can setup this in the __settings-v1.json__.
- bugs and other issues can be reported via the issues section of GitHub

## 3.  Using a precompiled release
---

You can download a precompiled binary version from the __Releases__ section of GitHub. There are also some precompiled plugins matching this version.

**Important**: After downloading Apple set's a _quarantine_ flag to the App, because it's unsigned code. This results in a message that it is damaged and should be moved to the trash. It's not damaged! Use the following steps to remove the quarantine flag from the App:

Workaround:

Open __Terminal__ App and paste the following lines:

```
cd ~/Download
xattr -dr com.apple.quarantine ./Rack.app
```

Now you should run Rack without any issue.

---

## 4. Build Rack yourself

__TODO, stay tuned.__


# License 

__All licenses and copyrights kept from the official release, see LICENSE, LICENSE-dist, LICENSE-GPLv3.__

All changes and improvements made on this version are licensed by MIT:

Copyright 2020 Patrick Lindenberg / Lindenberg Research

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

## Official sites:

*Rack* is the engine for the VCV open-source virtual modular synthesizer.

- [VCV website](https://vcvrack.com/)
- [Manual](https://vcvrack.com/manual/index.html)
- [Building](https://vcvrack.com/manual/Building.html)
- [Communities](https://vcvrack.com/manual/Communities.html)
- [Licenses](LICENSE.md)
