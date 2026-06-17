# pewpew-asteroids

This is a stripped down version of the original asteroids game,
with my own logic added in.

## Rules

- `W`, `A`, `S`, `D` to move.
- Ship always points towards mouse position.
- Click left mouse button to shoot.
- If you fire a shot and it does not hit any asteroid,
  you won't be able to shoot anymore until the ammo hits the edge
  of the window and gets reloaded (this was intentional).

I wanted to give the game a Valorant + Aimlabs feel,
and hence, the controls.

## Playing the Game

If you're on Linux, download [this binary](./asteroids-linux),
mark it executable (if not already marked), and execute it.

If you're on Windows, download [this executable](./asteroids-windows.exe)
and run it.

## Building the Project Manually

If you're on Linux and want to manually compile the code,
comment out the part of code in [momstaller.sh](./momstaller.sh)
intended for Windows installation, and run the script.
The final binary should be available both in the `build/`
directory, and the root of the project.

## Some Rant About This Project

This was my first time making a project using C++. I'm so glad
that I made this project first, instead of directly jumping into the
mega project that I had planned to work on. I finally get the hang of
writing C++ code and using CMake. I don't know much about these yet,
but based on however much I've learned, it's super fun working on a C/C++
project. You have explicit control over everything and decide how entities
get created, occupy memory, and get destroyed. I finally saw a real world
application of unique pointers.

---

### My Komments About LLMs and Building Application Designs

LLMs are getting insanely good at designing stuff. I had initially
hard coded the dimensions of the rectangles meant to be
put behind the display text and things were looking pretty off, based
on the length of the text written in them. I knew that the way to fix
it would be to make the box width dynamic based on the length of the
text, but was feeling too lazy to do it manually. I wrote the prompt well
and gave Gemini my entire `main.cpp` file. It refactored the rectangle
logic, made their dimensions dynamic, and even added color to the text!
It took a matter of few seconds to do what it would've taken me twenty
minutes or so. If you're someone who knows only how to work with
front-end applications / application designing, you're cooked fr. 💀

---

## References

I initially started out with the [Asteroids Tutorial][asteroidsbyandrewhamel1],
by Andrew Hamel, but it became increasingly difficult to follow his
insanely sped up tutorials, right from the second video.
This was when I decided to take ideas from the videos and write
my own logic instead.

Videos:
[Part 1][asteroidsbyandrewhamel1],
[Part 2][asteroidsbyandrewhamel2],
[Part 3][asteroidsbyandrewhamel3],
[Part 4][asteroidsbyandrewhamel4].

[asteroidsbyandrewhamel1]: https://youtu.be/abIlGCx_Yq8
[asteroidsbyandrewhamel2]: https://youtu.be/ClRtJs_ooVk
[asteroidsbyandrewhamel3]: https://youtu.be/60n46KbSvUg
[asteroidsbyandrewhamel4]: https://youtu.be/jkmT101cMVY
