# CBOS - Crypto Benchmark oder so

## Project Description
CBOS is a benchmark utility created to measure the performance of various cryptographic algorithms. It provides an independent and easy-to-use tool that can be utilized on different platforms. 

This tool was developed within the DevToSCA project. The official website can be found at https://www.devtosca.de. For more information, please visit the (german) website of the Federal Ministry for Education and Research (BMBF) at https://www.forschung-it-sicherheit-kommunikationssysteme.de/projekte/devtosca.

## How to Install and Use CBOS
1. Clone the repository using Git:
   ```bash
   git clone https://git.inf.h-brs.de/dev2sca/external-tools/cbos.git
   ```
   `cd` to the cbos directory then run `make` to compile the source code. Afterwards you can find the executables in the [out]() directory.

## Benchmarking your own Cryptographic library

1. Create a file for your crypto library in [Libraries](Libraries/). This file will serve as a central location for your code that you want to benchmark. For example, if your library is called "myCryptolib," you can create a file named "MyCryptoLib".

2. If your cryptographic library is hosted in a separate repository, you can add it as a submodule to your benchmarking project. This step is optional.

3. To develop your custom benchmarking code you can start by creating your own benchmarking code for evaluating the performance of your cryptographic library. 

Please refere to the example template in [Template](Template/) for guidance.

## Example Output
This output is an example showcasing the benchmarking results of AES-128 in ECB mode using the Botan and OpenSSL libraries.

```text
[Botan 3.2.0] running AES-128 benchmark...
[Botan 3.2.0] 0.358907 seconds for 1000000 iterations, 4096 bytes message
[Botan 3.2.0] Average Bytes/cycle count: 4.911556 
[Botan 3.2.0] Variance: 0.006992
[Botan 3.2.0] Standard Deviation: 0.083618

[OpenSSL 3.2.0 23 Nov 2023] running AES-128-ECB benchmark...
[OpenSSL 3.2.0 23 Nov 2023] 0.668742 seconds for 1000000 iterations, 4096 bytes message
[OpenSSL 3.2.0 23 Nov 2023] Average Bytes/cycle count: 2.679346 
[OpenSSL 3.2.0 23 Nov 2023] Variance: 0.012991
[OpenSSL 3.2.0 23 Nov 2023] Standard Deviation: 0.113978
```

## Acknowledgment
This project is a modified version of the [crypto-benchmark](https://github.com/mumble-voip/crypto-benchmark) project, released under the MIT License.

Original Project: [crypto-benchmark](https://github.com/mumble-voip/crypto-benchmark)

Original License: [MIT License](https://github.com/randombit/botan/blob/master/license.txt)

Several methods utilized in this project are originally implemented in [Dudect](https://github.com/oreparaz/dudect).