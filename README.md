# Un mare di altruismo

"Un mare di altruismo" is a project for Fisica dei Sistemi Complessi exam in Bononia University. You can read our report in *docs/* folder.

![Execution](/docs/FSC.png?raw=true "Execution")

## Installation

Use your package manager to install dependencies listed below.

```bash
sudo apt-get -y install cmake
sudo apt-get -y install python3
sudo apt-get -y install libgtkmm-3.0-dev
sudo apt-get -y install libomp-dev
sudo apt-get install libboost-all-dev
sudo apt-get install python-tk
```

Then download submodules dependencies.

```bash
git submodule update --init --recursive
```

## Usage

```bash
$ # Compile with
$ ./setup.sh -build 
$ # Execute with
$ ./a.out
$ # Clean your tree
$ ./setup.sh -clean
```
## License
[MIT](https://choosealicense.com/licenses/mit/)