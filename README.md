# GP2-Minecraft
This is a project where I recreate a small aspect of the survival mode of Minecraft using the Overlord Engine made for students at DAE. want to create a small aspect of the survival mode of Minecraft. 

## Mechanics

### World

A randomly generated infinite world. The world is separated in to multiple chunks. Chunks far away
from the player get unloaded. When the player moves in to new territory, new chunks get loaded.  
The whole world exists out of a blend between the plains biome and the forest biome of Minecraft.
This means there are lots of trees but also grass and flowers.  
All the surface blocks are grass - under the grass layer there is a layer of dirt blocks - under the dirt
there is a layer of stone blocks - until the last layer which is bedrock.

### Player

The player can run around and jump.  
When looking at a block in range, the player can start breaking a block. After a certain amount of time
the block breaks and drops an item.  
A player can pick up an item by running over the item. This item appears in the hot bar.  
The player can switch items in the hot bar.  
When looking at a block in range and while holding an item, a player can place a block.

### NPC

There will be a sheep NPC that will randomly spawn on some chunks. Every x seconds the sheep will
start moving in a random direction. When there is a block in front of the sheep it will jump.  
When a player looks at a sheep in range, the player can hit the sheep. A sheep has a certain amount
of health. When the health reaches zero, the sheep dies and it drops a wool block.  
When the sheep is hit, it will run in a random direction with a higher speed then normal walking.
