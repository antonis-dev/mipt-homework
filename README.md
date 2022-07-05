## MIPT homework project

mipt-homework - it is a collection of C++ homework projects

## Build instructions
To build you will need:

* cmake >= 3.16
* gcc >= 4.6

```
mkdir build && cd build
cmake ..
make 
``` 

## Execution
```
cd hwc_twoq_cache
./hwc_twoq_cache <cache_size> <el_num> [elements] 
```

## Regression tests
```
cd hwc_twoq_cache
./regression_tests
```