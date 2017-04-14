call setenv /Release /x86 /xp
set PATH=%PATH%;C:\flexandbison
nmake /F NMakefile
mkdir binary\windows\
move lslint.exe binary\windows\
nmake /F NMakefile clean
