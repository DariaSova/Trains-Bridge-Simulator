#Trains-Bridge Simulator in C
##A simulation of an automated control system for a single lane bridge

The simulation of the trains approaching the bridge from two different directions is done by using threads.


**Rules:**

* Only one train is on the bridge at any given time: trains do not overtake each other, trains cross the bridge in the order they arrive (subject to the requirement below)
trains headed East have priority over trains going West
* If there are trains waiting headed both East and West then two of the East bound trains should be allowed to cross for every West bound train allowed to cross


**Running:**

* **Input File format:**
  The input files have a simple format. Each line contains information about a single train. The files end with a blank line.
  The first character on each line is one of the following four characters: 'e', 'E', 'w', or 'W'
  The first two letters specify a train that is going East, the second two letters specify a train headed West.
  Immediately following the letter specifying the direction is an integer that indicates the length of the train. There is no space between the direction character and the length.
  The following file specifies three trains, two headed East and one headed West:
  E10 w6 E3

* **Compilation:**
  Clone the project and run `make` which will compile the C code and then run `./assign2 number_of_trains your_input_file.txt'

* **Testing:**
  If you want to run tests in the `/tests` folder, just run `tests/testing` from the command line. `testing` is the shell script that will run the program with four provided sample input files and create new files with the output in the same folder followed by comparing those files with the solution files. If the program runs correctly you will see the output of the file comparing in the format similar to:
  "4d3
  < #0works
  8d7
  < #1works
  9d8
  < #2works
  6d5
  < #3works
  13d12
  < #4works" _This_ _output_ _indicates_ _that_ _the_ _code_ _passes_ _the_ _testi_.
