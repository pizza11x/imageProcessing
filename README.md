# exercisesElim

This repository includes the main image processing algorithms

# Preliminary steps
- Open Terminal
- Install brew: /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)" 
- Install Opencv: brew install opencv
- Install pkgconfig: brew install pkgconfig

# To compile
- alias cv='g++ $(pkg-config --cflags --libs opencv) -std=c++11'
- cv name_file.cpp -o name_that_u_want

# To run
- ./name_that_u_want
