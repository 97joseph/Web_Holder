# First, as you can see, any line that begins with # is a comment.
#
# Second, make sure your makefile is actually called "makefile" or
# "Makefile". Don't call it by any other name, or with any suffix
# like Makefile.txt. It won't work.

# In the below, the line with a colon specifies a target to compile,
# and its dependencies are listed after the colon. It means if anything in
# the dependency list is changed, that target will be recompiled.
# The next line is the command to compile that target (or indeed to do
# anything).
# Note that the second line MUST BEGIN WITH A TAB (NOT SPACES).

# Actually, this is not a good example of using makefiles.
# See the one in Assignment 1 for really using separate compilation.

All: all
all: main

main: main.cpp
	g++ main.cpp -o main
