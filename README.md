# RFMCaptureTheFlag

A simple multiplayer capture the flag first person shooter made for a job test within a 7 days period (around 12 hours of work time in total). GAS was used for attibutes.


NOTES:

- I’ve  implemented a simple lobby setup with seamless travel enabled, so it is necessary to launch the game in standalone mode (if this is not possible, please open the map M_Shooter_CTF directly)
- The flag is represented as the white glowing mesh on the center of the map
- The chosen Multiplayer type was listen server  
- A simple UI for displaying the game score, player health, match events
- Multiple spawn points per team
- Team colors
- Implemented GAS for player health, gameplay effects, and abilities ( jump and shoot)
- The player carrying the flag has reduced speed
- Implemented an extra gameplay feature where when a point is scored, an random gameplay effect is applied to all players. Debuff for the scoring team and a buff for the non scoring team.
The effects are simple, just increase or decrease walk speed and jump height, but new effects can be easily added to the pool to be chosen.

