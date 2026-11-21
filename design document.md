# Genre

    Terminal ASCII Game Roguelike

Mutliplayer?

Mutliple rooms per level (think binding of isaac or zelda)

All rooms use a fixed char** array regardless of how much space is used, so that the room can be easily serialized and deserialized.
    There are two layers that make up the rooms: the "main" layer and the "sub" layer. The main layer is where the player moves around, while the sub layer is used for things like background elements or obstacles.
        The second layer does not take up even the unused space; it will contain an array of structs that contain the x and y position and type of each element in the sub layer. This allows for more efficient memory usage and easier randomization.

Abilities:
    Players can chooes a class to play as, each with their own unique abilities and playstyle. Classes can include:
        - Warrior: High health and melee damage, but slower movement speed.
        - Mage: Can cast powerful spells, but has low health and is vulnerable to attacks.
        - Rogue: Fast and agile, with the ability to dodge attacks and deal critical hits.
    Located at the bottom of the screen will be a hotbar with slots and shortcuts for abilities

Text Window System:
    Modular text window system that follows these rules
        - Fixed width and variable height, with minimum height of two lines and a maximum height of half the screen height.
        - Text windows are anchored at the top right of the screen
        - All text is padded by a single space on the left and right sides
        - If the text window is too small to fit the text, it will be truncated and an ellipsis will be added to the end of the last line.

╔════════════════════════════════════════╗
║ Kobold : Halt Traveler!                ║
║          How dare you enter my domain? ║
║          Know you the price of...  ◈◆︎║
╚════════════════════════════════════════╝
