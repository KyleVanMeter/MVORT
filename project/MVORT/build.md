# Build Instructions
This project relies on Qt5 (5.10.1), and the Open Asset Import Library (assimp 5.0.0-rc1), Eigen3, and OpenMP

Things to look out for before running qmake on the object file are the file paths to assimp in MVORT.pro, and the library itself being dynamically linkable via LD_LIBRARY_PATH, or some ld config in /etc/ld.so.conf.d/
    
