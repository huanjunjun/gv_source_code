# A GPGPU Transparent Virtualization Component for High Performance Computing Clouds #

The GPU Virtualization Service (GVirtuS) presented in this work tries to fill the gap between in-house hosted computing clusters, equipped with GPGPUs devices, and pay-for-use high performance virtual clusters deployed via public or private computing clouds. gVirtuS allows an instanced virtual machine to access GPGPUs in a transparent and hypervisor independent way, with an overhead slightly greater than a real machine/GPGPU setup. The performance of the components of gVirtuS is assessed through a suite of tests in different deployment scenarios, such as providing GPGPU power to cloud computing based HPC clusters and sharing remotely hosted GPGPUs among HPC nodes.

**[Click here to read the official GVirtuS paper.](https://link.springer.com/chapter/10.1007/978-3-642-15277-1_37)**

## How to cite GVirtuS in your scientific papers ##

* Montella, R., Ferraro, C., Kosta, S., Pelliccia, V., & Giunta, G. (2016, December). [Enabling android-based devices to high-end gpgpus](https://link.springer.com/chapter/10.1007%2F978-3-319-49583-5_9). In International Conference on Algorithms and Architectures for Parallel Processing (pp. 118-125). Springer, Cham.

* Mentone, A., Di Luccio, D., Landolfi, L., Kosta, S., & Montella, R. (2019, October). [CUDA virtualization and remoting for GPGPU based acceleration offloading at the edge](https://link.springer.com/chapter/10.1007/978-3-030-34914-1_39). In International Conference on Internet and Distributed Computing Systems (pp. 414-423). Springer, Cham.

* Montella, R., Giunta, G., Laccetti, G., Lapegna, M., Palmieri, C., Ferraro, C., ... & Nikolopoulos, D. S. (2017). [On the virtualization of CUDA based GPU remoting on ARM and X86 machines in the GVirtuS framework](https://link.springer.com/article/10.1007/s10766-016-0462-1). International Journal of Parallel Programming, 45(5), 1142-1163.

* Montella, R., Kosta, S., Oro, D., Vera, J., Fernández, C., Palmieri, C., ... & Laccetti, G. (2017). [Accelerating Linux and Android applications on low‐power devices through remote GPGPU offloading](https://onlinelibrary.wiley.com/doi/full/10.1002/cpe.4286?casa_token=VY2QbDZY7sMAAAAA%3Ah7KGKoE5jr36xowSzG05MeBFG673wawPA3-YIpeZgmjFFt6H7Rdig4e-dK7mkerI1Ov4r6Uxyk97tqE). Concurrency and Computation: Practice and Experience, 29(24), e4286.

* Montella, R., Giunta, G., & Laccetti, G. (2014). [Virtualizing high-end GPGPUs on ARM clusters for the next generation of high performance cloud computing](https://link.springer.com/content/pdf/10.1007/s10586-013-0341-0.pdf). Cluster computing, 17(1), 139-152.

* Laccetti, G., Montella, R., Palmieri, C., & Pelliccia, V. (2013, September). [The high performance internet of things: using GVirtuS to share high-end GPUs with ARM based cluster computing nodes](https://link.springer.com/chapter/10.1007/978-3-642-55224-3_69). In International Conference on Parallel Processing and Applied Mathematics (pp. 734-744). Springer, Berlin, Heidelberg.

* Montella, R., Coviello, G., Giunta, G., Laccetti, G., Isaila, F., & Blas, J. G. (2011, September). [A general-purpose virtualization service for HPC on cloud computing: an application to GPUs](https://link.springer.com/chapter/10.1007/978-3-642-31464-3_75). In International Conference on Parallel Processing and Applied Mathematics (pp. 740-749). Springer, Berlin, Heidelberg.

* Giunta, G., Montella, R., Agrillo, G., Coviello, G. (2010) [A GPGPU Transparent Virtualization Component for High Performance Computing Clouds](https://link.springer.com/chapter/10.1007/978-3-642-15277-1_37). In: Euro-Par 2010 - Parallel Processing. Euro-Par 2010. Lecture Notes in Computer Science, vol 6271. Springer, Berlin, Heidelberg

## GVirtuS applications ##

* Montella, R., Di Luccio, D., Marcellino, L., Galletti, A., Kosta, S., Giunta, G., & Foster, I. (2019). Workflow-based automatic processing for internet of floating things crowdsourced data. Future Generation Computer Systems, 94, 103-119.

* Montella, R., Marcellino, L., Galletti, A., Di Luccio, D., Kosta, S., Laccetti, G., & Giunta, G. (2018). Marine bathymetry processing through GPGPU virtualization in high performance cloud computing. Concurrency and Computation: Practice and Experience, 30(24), e4895.

* Deyannis, D., Tsirbas, R., Vasiliadis, G., Montella, R., Kosta, S., & Ioannidis, S. (2018, June). Enabling gpu-assisted antivirus protection on android devices through edge offloading. In Proceedings of the 1st International Workshop on Edge Systems, Analytics and Networking (pp. 13-18).

* Montella, R., Marcellino, L., Galletti, A., Di Luccio, D., Kosta, S., Laccetti, G., & Giunta, G. (2018). Marine bathymetry processing through GPGPU virtualization in high performance cloud computing. Concurrency and Computation: Practice and Experience, 30(24), e4895.

* Marcellino, L., Montella, R., Kosta, S., Galletti, A., Di Luccio, D., Santopietro, V., ... & Laccetti, G. (2017, September). Using GPGPU accelerated interpolation algorithms for marine bathymetry processing with on-premises and cloud based computational resources. In International Conference on Parallel Processing and Applied Mathematics (pp. 14-24). Springer, Cham.

* Galletti, A., Marcellino, L., Montella, R., Santopietro, V., & Kosta, S. (2017). A virtualized software based on the NVIDIA cuFFT library for image denoising: performance analysis. Procedia computer science, 113, 496-501.

* Di Lauro, R., Lucarelli, F., & Montella, R. (2012, July). SIaaS-sensing instrument as a service using cloud computing to turn physical instrument into ubiquitous service. In 2012 IEEE 10th International Symposium on Parallel and Distributed Processing with Applications (pp. 861-862). IEEE.

* Di Lauro, R., Giannone, F., Ambrosio, L., & Montella, R. (2012, July). Virtualizing general purpose GPUs for high performance cloud computing: an application to a fluid simulator. In 2012 IEEE 10th International Symposium on Parallel and Distributed Processing with Applications (pp. 863-864). IEEE.

# How To install GVirtuS framework and plugins #
## Prerequisites: ##

* **Compilers:** GCC, G++ with C++17 extension (Version 7 or above)

* **CMake:** Version 3.17 or above

* **OS:** CentOS 7.3 or Ubuntu 18.04 (note that those are tested OSes, but GVirtuS could be virtually installed anywhere)

* **CUDA Toolkit:** Version 10.2 or above

Furthermore, those packages are required:

```
build-essential 
autotools-dev 
automake 
git 
libtool 
libxmu-dev 
libxi-dev 
libgl-dev 
libosmesa-dev 
liblog4cplus-dev
```

The required packages can be installed with the following commands:

Ubuntu:

```
sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev libosmesa-dev git liblog4cplus-dev
```

CentOS:

```
sudo yum install centos-release-scl
sudo yum install devtoolset-8-gcc
scl enable devtoolset-8 bash
```

Now we can install GVirtuS.

## Installation: ##

1) `git clone` the **GVirtuS** main repository: 

```   
git clone https://github.com/gvirtus/GVirtuS.git 
```

2) Compile and install **GVirtuS** using `cmake`:

```
cd GVirutS
mkdir build
cd build
cmake ..
make
make install
```    

By default **GVirtuS** will be installed in `${HOME}/GVirtuS`. To override this behavior **export the GVIRTUS_HOME variable BEFORE RUNNING CMAKE**, i.e.:

```
export GVIRTUS_HOME=/Your/GVirtuS/Path 
```

`GVIRTUS_HOME` should be exported if **GVirtuS** is desired to be installed in a different path.

If everything worked properly, **GVirtuS** is now installed. This step **must** be performed on both the remote and client machines.

## Running GVirtuS: ##
### Backend machine (physical GPU and Cuda required) ###

These steps are aimed to the machine where the CUDA executables will be executed. 

GVirtuS can be run in both local _(for testing purposes)_ or remote setups.

GVirtuS backend configuration file `$GVIRTUS_HOME/etc/properties.json` should be modified if the default port `9999` is occupied or the machine is remote, changing the localhost IP with the IP of the machine:

```
{
  "communicator": [
    {
      "endpoint": {
        "suite": "tcp/ip",
        "protocol": "tcp",
        "server_address": "127.0.0.1",
        "port": "9999"
      },
      "plugins": [
        "cudart",
        "cublas",
        "curand",
        "cudnn"
      ]
    }
  ],
  "secure_application": false
}
```

Currently supported transmission means are:
* **TCP/IP**: 
``` 
"suite": "tcp/ip",
"protocol": "tcp", 
```
* **RDMA over Infiniband**
``` 
"suite": "infiniband-rdma",
"protocol": "ib", 
```

To run `gvirtus-backend` server application, perform the following command:

```
LD_LIBRARY_PATH=${GVIRTUS_HOME}/lib:${LD_LIBRARY_PATH} $GVIRTUS_HOME/bin/gvirtus-backend ${GVIRTUS_HOME}/etc/properties.json
```
The terminal should now prompt a similar message:

```
INFO - ? - GVirtuS backend version
INFO - ? - Configuration: /home/m.aponte/GVirtuS_fork/etc/properties.json
INFO - ? - Up and running
```

If everything of the above worked correctly, `gvirtus-backend` is now running, waiting for requests.

### Frontend machine (No GPU or Cuda required) ###

These steps are aimed to the client machine that cannot perform CUDA operations.

GVirtuS frontend configuration file `$GVIRTUS_HOME/etc/properties.json` should be modified if the default port `9999` is occupied or the machine is remote, changing the localhost IP with the IP of the remote machine:

```
{
  "communicator": [
    {
      "endpoint": {
        "suite": "tcp/ip",
        "protocol": "tcp",
        "server_address": "127.0.0.1",
        "port": "9999"
      },
      "plugins": [
        "cudart",
        "cublas",
        "curand",
        "cudnn"
      ]
    }
  ],
  "secure_application": false
}
```

**Note that In the local configuration, GVirtuS Backend and Frontend share the same configuration files.**

Optionally, a different configuration file could be set:

```
export GVIRTUS_CONFIG=$HOME/dev/properties.json
```

Now we have to compile our CUDA application.

If `nvcc` is being used, **be sure to compile using shared libraries**:

```
export EXTRA_NVCCFLAGS="--cudart=shared"
```

Now compile the CUDA application. A potential `nvcc` command could be:
```
nvcc example.cu -o example --cudart=shared
```

In order to tell your applications to use GVirtuS libraries, export the **dynamic GVirtuS library** with the following command. **THIS STEP IS FUNDAMENTAL**:

```
export LD_LIBRARY_PATH=${GVIRTUS_HOME}/lib:${GVIRTUS_HOME}/lib/frontend:${LD_LIBRARY_PATH}
```

`ldd`command could be useful to check whether the CUDA application is using GVirtuS libraries.

If everything is correctly set up, the cuda application can be eventually run through GVirtuS, in a transparent fashion:

```
./example
```

If `GVIRTUS_LOGLEVEL` environment variable is set on `DEBUG_LOG_LEVEL`, debug logs on terminal are expected on both backend and frontend applications. 

## Logging ##

In order to change the logging level, the `GVIRTUS_LOGLEVEL` environment variable should be defined as follows:

```
export GVIRTUS_LOGLEVEL=<loglevel>
```

The `<loglevel>` value is defined as follows:
```
OFF_LOG_LEVEL     = 60000

FATAL_LOG_LEVEL   = 50000

ERROR_LOG_LEVEL   = 40000

WARN_LOG_LEVEL    = 30000

INFO_LOG_LEVEL    = 20000

DEBUG_LOG_LEVEL   = 10000

TRACE_LOG_LEVEL   = 0
```
