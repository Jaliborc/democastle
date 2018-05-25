# Demo Castle
My first experiment working with 3D graphics in C++. Done using Irrlicht Engine.
Repo only includes the source code files, as I've lost the media files since then.

[![](http://jaliborc.com/images/other/democastle.jpg)](https://www.youtube.com/watch?v=oKCe8hE4r5k)

[Video preview](https://www.youtube.com/watch?v=oKCe8hE4r5k)

### Code
* *common.h:* the only header (simple project, no need for a more complex header structure)
* *main.c:* responsible for starting the engine, the map, each room, and delegating events to the "zone" API
* *zones.h and zones.c:* an API to define independent “zones” - areas with different rules in the castle - so that new experiments can be easily added to the environment
* *tennis.c:* the tennis game, shown in the teaser video
