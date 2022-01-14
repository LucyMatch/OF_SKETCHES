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
* simple poly ( graphic class )

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

Particle new Spawn mode
* circle burst spawn. origin is landmark center andc i.e. eyes burst out - radial and travel in spoawned direction. 


Extra Graphics

1. create the graphic class
this class is given a location and draws  a variaty of poly shapes with different effects 
is it actually an extension of base cut? are these just manually created cut types ?

graphic class is used for landmark particle cut option as descibed above 

but also a graphic manager is created per landmark ( the same way as Pmen ). this manager adds extra graphics behind the primary particle / landmark cut. so i.e. we could have a blue circle behind the right eye .... 

Misc quick punch list 
* cut particle toggle - to just draw the cut filled with colour - no texture
* add more landmarks support
* LocalMediaManager Class
* bg options - using localMediaManager class
* toggle for applying varying gravityy at spawn only
* dont apply update varying gravity within rrange -> only used fixed value


Larger List
* layers + draw order
* switch case Blendmode options
* universal controls - a class for utility events
    * fade alpha in / out on btn press
    * toggle all spawning
    * toggle all random spawning
    * toggle all varying gravity
    * clear all fbos

