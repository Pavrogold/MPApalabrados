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
	${OBJECTDIR}/src/bag.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/move.o \
	${OBJECTDIR}/src/player.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../local/lib/liblanguage.a ../local/lib/libansiterminal.a ../local/lib/libwordlist.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3: ../local/lib/liblanguage.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3: ../local/lib/libansiterminal.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3: ../local/lib/libwordlist.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/mpapalabrados3 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/bag.o: src/bag.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -I../local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bag.o src/bag.cpp

${OBJECTDIR}/src/main.o: src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -I../local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/move.o: src/move.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -I../local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/move.o src/move.cpp

${OBJECTDIR}/src/player.o: src/player.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -Iinclude -I../local/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/player.o src/player.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
