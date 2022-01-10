running to do / features: 

Texture Cut out modes

0. regular cutman + live video texture facetracking is being performed on

1. feeding cutman different texture from live video feed ( single )
    * An image from a directory of images ( single w/ button for nxt image )
    * An image from a directory of images ( image updated on every update )
    * A frame from a different video

2. feeding cutman a different texture per ( landmark )
    * same options as above but a different image / input / texture for each landmark
    * function takes an array of textures and applies them to the landmarks

3. simple poly of landmark - filled with block colour

toggles : ( these per landmark? - we want different effects per land mark)
* live_feed
* video_feed
* image_static
* image_update
* simple poly 

* toggle single / multi 

Particle Cut Out Modes

1. add cut partle man back in 
( a particle man per landmark - on update we feed it with the cut texture from the texture cut man mode)
( particle options -> match to 03_facetracker sketrch functionality )

toggles : 
* enable spawning
* enable_random spawning
* enable gravity
* gravity direction / speed etc... 
