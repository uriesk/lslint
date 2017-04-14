call "C:\Program Files (x86)\Microsoft Visual studio 14.0\VC\vcvarsall.bat" x86
set PATH=%PATH%;C:\flexandbison
nmake /F NMakefile
mkdir binary\windows32\
move lslint.exe binary\windows32\
nmake /F NMakefile clean
