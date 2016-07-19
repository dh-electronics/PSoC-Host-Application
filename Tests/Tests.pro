TARGET = tests
TEMPLATE = app
CONFIG -= qt

HEADERS = 

SOURCES = \  
    main.cpp

INCLUDEPATH += $$PWD/../C-API/include

LIBS += -L$$POCO/lib -lPocoFoundation -L$$OUT_PWD/../C-API/ -lc-api -L../DHCOM_HAL -lDHCOM_HAL

DEPENDPATH += $$PWD/../C-API
