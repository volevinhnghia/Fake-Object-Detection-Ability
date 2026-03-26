# Technical Quick Test - Fake-Object-Detection-Ability

Quick technical test for detecting ability with real and fake object

## Setup instructions

In foulder /All/Game/Interactive we have BP_SpawnVolume 

![alt text](<Screenshot 2026-03-26 162728.png>)

With this SpawnVolume, we only need to put it in anywhere in level and do following set up

![alt text](<Screenshot 2026-03-26 171839.png>)

Allowed Character Class: Choose main character BP

Spawnable Classes: Pict all object want to spawn inside this volume and how many for each

Correct Class: What is correct object should collect all

Spawn once: spawn once or every time character go in volume

Countdown Time: How long for player collect all object they required to collect

Level Demo: /Game/ThirdPerson/Lvl_ThirdPerson

We move in middle of the map, that will spawn all object need to pick up. We should avoid all fake object and need correct real one. 

## Controls
WASD: Normal moving

Mouse: Camera Rotation

E: Interact

Q: Use Reveal skill for know what is real object
## Feature overview
We have 2 type of interactive object

![alt text](<Screenshot 2026-03-26 163635-1.png>)

For BP_InteractFake, it will trigger GE_DmgPickUp, and not reveal when we use skill reveal

![alt text](<Screenshot 2026-03-26 163843.png>)

For BP_InteractReal, it will reveal when we use skill reveal

![alt text](<Screenshot 2026-03-26 164031.png>)

When we press Q, that will reveal all BP_InteractReal, and slowly draining player mana

![alt text](<Screenshot 2026-03-26 164130-1.png>)

## System Overview

The game system mainly using Gameplay Ability System from Unreal Engine 5.

*   Gameplay Attribute: BaseAttributeSet

*   Gameplay Ability: GA_Interact, GA_Reveal

*   Gameplay Effect: GE_CostMana, GE_DamageHealth

SpawnVolume: Will spawn object inside the zone and start countdown timer through CountdownSubsystem. When collect all right object, volume will destroy. If player die or time out, game will restart.

InteractiveBaseObject: Will interact with GA_Interact through interface, and trigger gameplay effect depending what type of Interactive Object.

## Improvement after

*   Add gameplay cue for play VFX, SFX when interact with object.

*   Improve how GA_Reveal interact with object outline, currently just change collision "hidden in game" for confirm GA_Reveal work.

*   Change how object random spawn for prevent overlap object.