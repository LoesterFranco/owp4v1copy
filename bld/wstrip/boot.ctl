set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/bat/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

[ BLOCK <1> clean ]
#==================
    echo rm -f -r <PROJDIR>/<OBJDIR>
    rm -f -r <PROJDIR>/<OBJDIR>

[ BLOCK <1> boot ]
#=================
    echo Building the wstrip bootstrap
    mkdir <PROJDIR>/<OBJDIR>
    cdsay <PROJDIR>/<OBJDIR>
    wmake -h -f ../bootmake
    <CPCMD> strip.exe <DEVDIR>/build/bin/wstrip
    cdsay <PROJDIR>
