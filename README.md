# **Flappy Bird** in Vertical Orientation. **Nintendo 3DS**

![Game Banner](/assets/banner_page.png)  

Yet another "**Flappy Bird**" mobile game homebrew recreation for Nintendo 3DS (old/new)  
Made in **vertical orientation**, like the original game. I didn't find any port for 3DS in this format, so I decided to make it by myself. This is my first homebrew game ever, and first project in C too. I hope you will have fun with it.  
  
[GBAtemp thread](https://gbatemp.net/threads/flappy-bird-in-vertical-orientation.672726)  
[Game on Itch.io](https://millkeny.itch.io/flappy-bird-for-3ds)

## Game

*   **Tap bottom screen** or **press D-PAD or B** to flap between the pipes.
*   Game's speed is increasing for each score point.
*   For every 25 points you will earn a new medal - **bronze, silver, gold and platina**. And for platina (99 score) you will unlock the **SuperBird**!
*   Your best score is save in game and stored at path: SD:/config/millkeny/flappy.dat. You also can clear your save data from in-game menu.
*   Night mode depends on real time on your device. It will be turned on between 21:00 and 06:00.
    
## Screenshots

![Screenshot 1](/assets/1.png)  
![Screenshot 2](/assets/2.png)  
![Screenshot 3](/assets/3.png)  
![Screenshot 4](/assets/4.png)

## How to Install

*   3DSX - Copy .3dsx from [releases page](https://github.com/MillKeny/flappy/releases) to 3ds folder on your SD card and run it via Homebrew Launcher or emulator.
*   CIA - Install the .cia or scan the QR Code from [releases page](https://github.com/MillKeny/flappy/releases) with the CIA manager of your choice.
*   Or you can build the project by yourself: Install [devkitPro](https://github.com/devkitPro/installer/releases) and "make" the project. Search internet for more.

## Special Thanks

*   [devkitPro](https://github.com/devkitPro) - For all tools, libraries, docs and etc.
*   [BlyZeDev](https://github.com/BlyZeDev) - For SD filesystem and audio logic from his [T-Rex-Game-3DS](https://github.com/BlyZeDev/T-Rex-Game-3DS).
*   Dong Nguyen, .GEARS - for this legendary game and original assets.
