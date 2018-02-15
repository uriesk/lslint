@echo off
rem :: VS2015
IF EXIST "C:\Program Files (x86)\Microsoft Visual studio 14.0\VC\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual studio 14.0\VC\vcvarsall.bat" x86
) else (
    rem :: VS2017
    rem :: NOTE: The instructions contained in the NMakefile are no longer necessary with this method.
    rem :: Simply run the script, as long as C:\flexandbison exists.
    pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\"
    call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
    popd
)
set PATH=%PATH%;C:\flexandbison
nmake /F NMakefile
mkdir binary\windows32\
move lslint.exe binary\windows32\
nmake /F NMakefile clean
