# Tapping Annotator
# Experiment Interface
# (c) George Christodoulides 2016-2018

# This program or module is free software: you can redistribute it
# and/or modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version. It is provided
# for educational purposes and is distributed in the hope that it will
# be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
# the GNU General Public License for more details.

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = experiment-interface
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        MainWindow.cpp \
    ParticipantIdentifactionForm.cpp \
    WorkingMemoryTestForm.cpp \
    TappingTestForm.cpp \
    FinalQuestionnaireForm.cpp \
    WMTestSceneWidget.cpp \
    WMTestDataEntryWidget.cpp \
    QSleep.cpp \
    EndExperimentForm.cpp

HEADERS  += MainWindow.h \
    ParticipantIdentifactionForm.h \
    WorkingMemoryTestForm.h \
    TappingTestForm.h \
    FinalQuestionnaireForm.h \
    WMTestSceneWidget.h \
    WMTestDataEntryWidget.h \
    QSleep.h \
    EndExperimentForm.h

FORMS    += MainWindow.ui \
    ParticipantIdentifactionForm.ui \
    WorkingMemoryTestForm.ui \
    TonePerceptionTestForm.ui \
    TappingTestForm.ui \
    FinalQuestionnaireForm.ui \
    WMTestDataEntryWidget.ui \
    EndExperimentForm.ui

RESOURCES += \
    expe.qrc
