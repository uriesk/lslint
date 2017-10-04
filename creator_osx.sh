curl -O https://bitbucket.org/api/1.0/repositories/Sei_Lisa/kwdb/raw/default/outputs/builtins.txt
export DEBUG=""
make
mkdir -p binary/osx/
cp lslint binary/osx/
make clean
