# HE<sup>3</sup>DB: An Efficient and Elastic Encrypted Database via Arithmetic-and-Logic Fully Homomorphic Encryption
This code is the implmentation of the paper HE<sup>3</sup>DB: An Efficient and Elastic Encrypted Database via Arithmetic-and-Logic Fully Homomorphic Encryption.
## Requirements

```
git 
gcc >= 10
cmake >= 3.16
GMP 6.2.0
```

## Building HE<sup>3</sup>DB
You can build the HE<sup>3</sup>DB (out-of-source) for your machine by executing the following commands:
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DSEAL_THROW_ON_TRANSPARENT_CIPHERTEXT=OFF
make
```
After the build completes, the following examples can be found in 
the `build/bin/` directory.

If you have Docker on your system, this will do above on docker.
```
docker build -t he3db .
docker run -i -t he3db
cd bin
```
After the build completes, the following examples can be found in 
the docker container.

## Examples
### Homomorphic Comparison
- codes `test/comparison_test.cpp`  
- output binary `build/bin/comparison_test` 
- This demo shows the homomorphic comparison in HE<sup>3</sup>DB. 

### Ciphertext Conversion
- codes `test/conversion_test.cpp`
- output binary `build/bin/conversion_test`
- This demo shows the homomorphic ciphertext conversion in HE<sup>3</sup>DB.

### Query Evaluation
- codes `test/query.cpp`
- output binary `build/bin/query`
- This demo shows the evaluation of tpc-h Q6 over a 16 rows of encrypted database.