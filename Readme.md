# OF SKETCH REPO



**Classes :**

Contains Reusable classes I have created and Build upon. Currently these need to be imported into projects using them. 

via > solution Explorer > Properties > C/C++ > General > Additional Include Directories

**Sketches :**

This is where the sketches live 

There is a rough naming convention 

00_ indicates a simple / base sketch 

01_ is the start of an idea / build on the simple sketch 

02_ /and so on are subsequent builds on the initial 01_idea

**Templates :** 

These are very simple "Sketches" to be copied to start new projects 

@TODO : actually turn these into project templates that work in a smarter way



****

### 00_particle_template

 <img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/00_particle_template.PNG?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager

Functionality

* Generates a cell based grid -> each cell then becomes it's own Block Manager ( polymorphed Particle System )
* Block Manager Simple spawns "blocks" within cell space provided - kills blocks on edge detection. Block Manager has a block limit
* keypress 's' to force spawn
* keypress 't' to make blocks seek hard coded location

GUI : 

* None

### 01_particle_grid_base

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/01_particle_grid_base.PNG?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager

Functionality

* Generates a cell based grid -> each cell then becomes it's own Block Manager ( polymorphed Particle System )
* Block Manager Simple spawns "blocks" within cell space provided - kills blocks on edge detection. Block Manager has a block limit
* Block Manager Grid Spawns "blocks" within cell space provided, Grid of Blocks is calculated off passed vals; colResolution / rowResolution
* keypress 'a' to grid spawn hardcoded inputs
* keypress 's' to force spawn
* keypress 't' to make blocks seek hard coded location
* keypress 'g' to make blocks seek hard coded location

GUI : 

* None

### 02_particle_grid_ink_illustrations_prep

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/02_particle_grid_ink_illustration_prep.PNG?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager

Functionality

* Generates a cell based grid -> each cell then becomes it's own Block Manager ( polymorphed Particle System )
* Block Manager Simple spawns "blocks" within cell space provided - kills blocks on edge detection. Block Manager has a block limit
* Block Manager Grid Spawns "blocks" within cell space provided, Grid of Blocks is calculated off passed vals; colResolution / rowResolution
* Block Home Seeking is enabled each update 
* Repel Force controlled by mouse dragged
* keypress 'a' to grid spawn hardcoded inputs
* keypress 's' to force spawn
* keypress 't' to make blocks seek hard coded location
* keypress 'g' to make blocks seek hard coded location

GUI : 

* Yes for Particles / Blocks /Forces

### 03_particle_grid_ink_illustrations 

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/02_particle_grid_ink_illustration.PNG?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager

Functionality

* Generates a cell based grid -> each cell then becomes it's own Block Manager ( polymorphed Particle System )
* Block Manager Grid Spawns "blocks" within cell space provided, Grid of Blocks is calculated off passed vals; colResolution / rowResolution
* Block Home Seeking is enabled each update 
* Repel / Attract Force controlled by mouse moved
* ImageManager Class imports x amount of .png files from provided path + draws to screen
* keypress 'a' to grid spawn hardcoded inputs
* keypress 's' to force spawn
* keypress 't' to make blocks seek hard coded location
* keypress 'g' to make blocks seek hard coded location
* keypress 1-4 /7-0 to save gui files
* keypress q-r / u-p to load gui files
* keypress "enter" to save screen shot

GUI : 

* Yes for Particles / Blocks /Forces

### Home_IP_Cam_viewer

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/Home_IP_Cam_Viewer.PNG?raw=true" width="200" height="200">

External Classes

* /

Functionality

* uses ofxIPVideoGrabber + IpVideoGrabber settings to load in cameras
  * bin / data / cams.json 
  * cams must have encryption disabled for the desired viewing profile
* Displays cams defined in settings - either in single mode or grid mode - along with relevant information
* keypress 's' load single cam
* keypress 'g' load grid mode

GUI : 

* /

### 00_cv_color_palette - wip

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/00_cv_color_palette_01.PNG?raw=true" width="200" height="200">
<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/00_cv_color_palette_02.PNG?raw=true" width="200" height="200">

External Classes

* /

Functionality

* INPUT MODE : use the mouse to highlight a colour view via viewfinder, click mouse to save colour to palette. displayed to left of screen
* OUTPUT MODE : basic draw of contour finder current doesn't do all colours in palette - only last
* keypress 's' to switch mode
* keypress 'c' to clear palette

GUI : 

* cv control & general



### 00_simple_image_imports

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/00_simple_image_imports.PNG?raw=true" width="200" height="200">

External Classes

* /

Functionality

* Loads assets from a directory via specified path
* draws all assets to screen
* TBD ( assetsRandomDraw() )
* TBD ( turning this into an external class )

GUI : 

* general & img alpha

### 01_particle_cutouts 

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/01_particle_cutouts.PNG?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager

Functionality

* ImageParticleManager
  * loads image assets from provided directory
  * Assigns the textures to particles randomly when spawning them
  * simple spawn (seems broken)
  * grid spawn
  * kills / erases on leaving bound
* Generates a cell based grid -> each cell then becomes it's own ImageParticleManager( polymorphed Particle System )
* Repel / Attract Force controlled by mouse moved
* keypress 'a' to re init grid spawn ( uses gui vals to set p grid)
* keypress 's' to force spawn ( not working)
* keypress 'x' to re-init cell grid of particle managers
* keypress 1-4 /7-0 to save gui files
* keypress q-r / u-p to load gui files
* keypress "enter" to save screen shot

GUI : 

* Yes for Particles / Blocks /Forces / Grid controls

### 02_particle_cutouts 

<img src="https://github.com/LucyMatch/OF_SKETCHES/blob/master/screen_grabs/02_particle_cutouts.png?raw=true" width="200" height="200">

External Classes

* Forces
* Particle
* ParticleManager
* ImageHandler

Functionality

* Image Handler
  * loads image sets from directory
* ImageParticleManager
  * constructor takes array of tex
  * sizing control
  * Image control - uniform - mixed
  * grid spawn
  * Image set spawn
  * Simple spawn
  * varying gravity
  * kills / erases on leaving bound
* Pman now draws to properly blended FBO - so there can be trail control w/out effecting bg
* Generates a cell based grid -> each cell then becomes it's own ImageParticleManager( polymorphed Particle System )
* Repel / Attract Force controlled by mouse moved
* keypress 'd' to spawn a particle per unique img in image set
* keypress 'a' to re init grid spawn ( uses gui vals to set p grid)
* keypress 's' to force spawn 
* keypress 'x' to re-init cell grid of particle managers
* keypress 1-4 /7-0 to save gui files
* keypress q-r / u-p to load gui files
* keypress "enter" to save screen shot

GUI : 

* Yes for Particles / Blocks /Forces / Grid controls

ISSUES :

* you fucked up all the blend modes
* all controls could be better streamlined! its like a very complex instrument you forget how to play....

### 00_Image_Cut 

External Classes

* ImageHandler

Functionality

* utility for cutting static images and exporting out the assets
* there is an "input mode" where you can use the mouse to select where different cuts exist
* cut shapes are configurable
* key press for export / next image etc

GUI : 

* 

Potential

* tidy up with proper on screen info and could turn into utility offered on github!



### 00_video_cut

External Classes

* ImageHandler

Functionality

* utility for cutting  videos /webcam live feed
* same functionality as image-cut - mouse controlled "holepunch" with configrable shapes
* key controlled export

GUI : 

* 

Potential

* tidy up with proper on screen info and could turn into utility offered on github!



### 03_particle_cutouts_live

External Classes

* 

Functionality

* 00_video_cut + 02_particle_cut_outs
* live / static video with "cuts" that act as particles - > spawn positions etc... 

GUI : 

* 

Potential

* 



### 01_facetracker_two_cut

External Classes

* 

Functionality

* uses facetracker2 addon
* Utility for exporting cuts based on facetracking with the ability to set timed intervals for exporting out cuts
* only deals with eyes + mouths
* live or input videos

GUI : 

* 

Potential

* tidy up with proper on screen info and could turn into utility offered on github!



### 03_particle_cutouts_facetracker_two - wip

External Classes

* Cuts.h
* VideoHandler.h / .cpp
* Forces / Particle / Pmanager (?)

Functionality ( plan / dump )

* 01_facetracker_two_cut + 03_particle_cutouts_live / 02_particle_cuts
* add alternate shapes for "face cuts" 
  * think circles around the eyes but tracking
* get baseline particle spawn / functionality going
* THEN THE GOAL IS TO CLEAN THIS SHIT UP!!!!!!
* eventually do mouth inner cut / other features?

GUI : 

* 

Potential

* 

Current approach...

* A particle manager per "poly cut"
  * meaning a particle manager per facial feature ( landmark )
  * all particles belonging to that manager will have the same "texture"
  * the option to spawn randomly / at feature location / debug ( display cut at location)
  * *** option to do some really cool variations from the manager - spawn varying sizes, spawn varying shapes of cuts, spawn different colour effects per particle
  * *** I believe this means we want the ability to control individual particle features from the manager... so no static particle features - but static per manager

