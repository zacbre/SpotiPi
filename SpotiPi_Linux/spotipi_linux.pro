TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    utils.cpp

HEADERS += \
    main.h \
    curl/typecheck-gcc.h \
    curl/stdcheaders.h \
    curl/multi.h \
    curl/mprintf.h \
    curl/easy.h \
    curl/curlver.h \
    curl/curlrules.h \
    curl/curlbuild.h.in \
    curl/curlbuild.h \
    curl/curl.h

LIBS += -lcurl

OTHER_FILES +=

