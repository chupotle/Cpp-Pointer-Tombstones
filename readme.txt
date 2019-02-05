Timothy Chu
Assignment 5 - Tombstones

To run, execute:
"make"
and run each of the foo files by executing:
"./foo#"

otherwise just compile the *.cpp files as usual with:
"g++ foo#.cpp"

The purpose of this project was to create "tombstones", a library package made to catch dangling references and memory leaks in a C++ program.
This program tells the user ome basic info about the tombstone that triggered the error, such as if it was freed, the reference count, and what operation.