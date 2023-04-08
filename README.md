lambda
====

Mixed use of term recall and direction recall, unified scoring, unified sorting search engine

# deps
* turbo
* libtext

# build

```shell
git clone https://github.com/gottingen/lambda.git
cd lambda
conda env create -f conda/environment.yaml
conda activate lambda-dev
mkdir build
cd build
cmake ..
make
```

# design

[system design doc](https://github.com/gottingen/search-book)
[engine design doc]()