#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/c3d16e37/Fl_Table_Copy.o \
	${OBJECTDIR}/_ext/c3d16e37/SimpleTable.o \
	${OBJECTDIR}/_ext/c3d16e37/TwoTables.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`fltk-config --cxxflags` 
CXXFLAGS=`fltk-config --cxxflags` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk lib/debug/libsimpletable.a

lib/debug/libsimpletable.a: ${OBJECTFILES}
	${MKDIR} -p lib/debug
	${RM} lib/debug/libsimpletable.a
	${AR} -rv lib/debug/libsimpletable.a ${OBJECTFILES} 
	$(RANLIB) lib/debug/libsimpletable.a

${OBJECTDIR}/_ext/c3d16e37/Fl_Table_Copy.o: /home/max/cpp/fltk/SimpleTable/src/Fl_Table_Copy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/c3d16e37
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/max/cpp/fltk/SimpleTable -I/home/max/cpp/My_1_5 -I/home/max/cpp/fltk/SimpleTable/src -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c3d16e37/Fl_Table_Copy.o /home/max/cpp/fltk/SimpleTable/src/Fl_Table_Copy.cpp

${OBJECTDIR}/_ext/c3d16e37/SimpleTable.o: /home/max/cpp/fltk/SimpleTable/src/SimpleTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/c3d16e37
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/max/cpp/fltk/SimpleTable -I/home/max/cpp/My_1_5 -I/home/max/cpp/fltk/SimpleTable/src -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c3d16e37/SimpleTable.o /home/max/cpp/fltk/SimpleTable/src/SimpleTable.cpp

${OBJECTDIR}/_ext/c3d16e37/TwoTables.o: /home/max/cpp/fltk/SimpleTable/src/TwoTables.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/c3d16e37
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/home/max/cpp/fltk/SimpleTable -I/home/max/cpp/My_1_5 -I/home/max/cpp/fltk/SimpleTable/src -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/c3d16e37/TwoTables.o /home/max/cpp/fltk/SimpleTable/src/TwoTables.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} lib/debug/libsimpletable.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
