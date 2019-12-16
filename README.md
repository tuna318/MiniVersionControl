# MiniVersionControl

## 1. Folder structure
- __helper__: Store all support libraries 
- __server__: Store server code
- __th-cmd__: Store th-cmd and client code
- __storage__: Storage user's repositories

## 2. Settup path for global running
open`.bashrc` </br>
```
nano ~/.bashrc
```
then add path to executable files:
```
export PATH=$PATH:$HOME/path/to/MiniVersionControl/th-cmd
export PATH=$PATH:$HOME/path/to/MiniVersionControl/server
```
save the file then run:
```
source ~/.bashrc
```

## 3. Compile and run
In the main folder run:
```
make all
```
run server:
```
th-server
```
run app
```
th-app
```
commit:
```
th-commit -m "message"
```
pull:
```
th-pull
```
push:
```
th-push
```
