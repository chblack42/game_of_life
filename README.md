How to run:
 
Open the game_of_life.sln (VS2022)
 
Set "game_of_life" as the starting project.
 
Then run the program (F5) with the desired configuration.
 
How to test:
Made a quick way to test various configurations
 
In the `impl/main/h` there is a #define for `TEST`, just set that to true and run the application. If an assert is hit then the test fails.
 
Information about this solution:
 
- This solution tries to break up each section to more manageable squares (that are the power of 2 in width/height) that I call civilizations. Civilizations only stay in memory if they have something living in it. The size of the civilization can be edited by adjusting the `CIVLIZATION_SIZE` variable in the 'life/civilization.h` file.
 
- I have made use of templating to specialize how each square hold its memory, currently it supports `std::vector<boo>`, `std::bitset<>`, `std::unordered_map<coordinate,bool>`. This also allows for extensibility as the idea was to abstract away most of the logic into common functions and only care about the container when getting or setting lifes.
 
- My ideal solution would have involved swapping containers out when the population got too large or small where a different container would benefit more from that size.
