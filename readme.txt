Montana Tech Miner - game project
assigned by J. Pach for some SE322 class
================================================================================

GAME IDEA (what they told me to make)

its a platformer but the point isnt just to get to the end
the miner guy has to collect all the ore on the map
but hes got stamina and every action costs stamina
jumping costs stamina, mining costs stamina
there is food on the map that gives stamina back
the whole point is to collect ALL ore using as LITTLE stamina as possible
because at the end your remaining stamina gets converted to score
so you gotta plan your route, dont jump if you dont have to
dont miss ore or youll backtrack and waste stamina

think liero but you are a miner
the ENTIRE screen is solid earth
you spawn in a small circular clearing, like you just broke through into a pocket
a few natural caves are scattered around the map (also circular clearings)
everything else is solid and you have to dig through it

the map is basically a grid of squares
each square is either earth, ore, food, or empty air
when you dig a square it just becomes empty
the squares have textures on them so it looks like real dirt and rock
not just colored rectangles
when a square disappears the texture disappears with it
thats the whole trick, its simpler than it looks

controls they wanted:
- arrow keys to move
- space to jump
- E to mine when standing near ore

map is one screen, no scrolling
ore is buried in the earth, food is in the caves
when all ore is collected the game ends and shows your score

thats basically it, pretty simple

================================================================================

WHAT I ACTUALLY DID

got the raylib tutorial working first, the scarfy animation thing
adapted it for the miner sprite

player moves left and right, flips direction
jumping works, gravity works
should be:
n ores on the map, m foods
stamina bar at the top, starts at 100
jump costs X stamina
mining costs Y stamina
food restores Z stamina
score = stamina * 10 at the end
win screen shows up when all n ores collected

everything is in main.c, i know its messy but it works
globals at the top because i was going to refactor later but didnt get to it

================================================================================

KNOWN ISSUES / STUFF I DIDNT FINISH

animation is broken
the walk sprite i tried to generate with AI was garbage, legs dont alternate
so right now its using the idle sprite for everything including walking
it technically cycles through the 2 idle frames when moving which looks dumb
(look at the idle spritesheet in assets (thomas), thats whats being used for walking too)
someone needs to either:
    - get a proper walk spritesheet with 6-8 frames and real leg alternation
    - hook up separate textures for idle/walk/jump states
    - the code has frameIndex and numFrames already so shouldnt be hard
      just swap the texture and update numFrames

collision is a bit janky sometimes
if you fall too fast you can clip through a square, didnt have time to fix

no sounds at all

no main menu, just starts immediately

no way to restart without closing the window

the tile textures are just colored rectangles right now
earth is brown, ore is gold, food is green
someone needs to replace those with actual tile sprites
each tile is a fixed square size so any square texture will drop right in

the miner sprite is facing left by default, flips right when going right
that part works fine actually

================================================================================

REQUIREMENTS (original, roughly)

single screen underground map, entirely filled with earth
player spawns in a small circular cleared area
two or more additional cave pockets scattered in the map
ore nodes buried in earth, collected by pressing E when adjacent
food items in cave pockets, collected by walking through them
stamina resource displayed as a bar, starts at fixed value
jumping costs small stamina, digging costs larger stamina
food restores stamina
game ends when all ore collected, score based on remaining stamina
implemented in C using raylib

================================================================================

stuff i would have done if i stayed:

split into multiple files, main.c is getting long
proper animation state machine, idle/walk/jump/dig as separate states
actual dig animation, not just a snap to idle
sounds, raylib has audio support its not hard
a proper spritesheet with json atlas for the sprite frames
maybe a second level
proper tile sprites instead of colored rectangles

anyway good luck whoever gets this

the liero-style map is more interesting than a regular platformer i think
but the implementation is rough, lots to improve
the stamina routing puzzle is a good idea even if the map needs work