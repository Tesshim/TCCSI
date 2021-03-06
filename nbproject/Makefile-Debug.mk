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
	${OBJECTDIR}/_ext/4a940c2/ExactMethod.o \
	${OBJECTDIR}/_ext/4a940c2/SMSP.o \
	${OBJECTDIR}/_ext/4a940c2/SMSPSD.o \
	${OBJECTDIR}/_ext/4a940c2/SMSPSDMO.o \
	${OBJECTDIR}/_ext/4a940c2/SchedulingInstance.o \
	${OBJECTDIR}/_ext/4a940c2/Utils.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-L/arquivos/Professor/Pesquisa/Gurobi/gurobi702/linux64/lib/

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/singlemachineexact

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/singlemachineexact: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/singlemachineexact ${OBJECTFILES} ${LDLIBSOPTIONS} -m64 -g -std=c++11 -I /arquivos/Professor/Pesquisa/Gurobi/gurobi/702/linux64/include/ -L /arquivos/Professor/Pesquisa/Gurobi/gurobi/702/linux64/lib/ -lgurobi_g++5.2 -lgurobi70 -lm

${OBJECTDIR}/_ext/4a940c2/ExactMethod.o: /home/marcelo/NetBeansProjects/SingleMachineExact/ExactMethod.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/ExactMethod.o /home/marcelo/NetBeansProjects/SingleMachineExact/ExactMethod.cpp

${OBJECTDIR}/_ext/4a940c2/SMSP.o: /home/marcelo/NetBeansProjects/SingleMachineExact/SMSP.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/SMSP.o /home/marcelo/NetBeansProjects/SingleMachineExact/SMSP.cpp

${OBJECTDIR}/_ext/4a940c2/SMSPSD.o: /home/marcelo/NetBeansProjects/SingleMachineExact/SMSPSD.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/SMSPSD.o /home/marcelo/NetBeansProjects/SingleMachineExact/SMSPSD.cpp

${OBJECTDIR}/_ext/4a940c2/SMSPSDMO.o: /home/marcelo/NetBeansProjects/SingleMachineExact/SMSPSDMO.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/SMSPSDMO.o /home/marcelo/NetBeansProjects/SingleMachineExact/SMSPSDMO.cpp

${OBJECTDIR}/_ext/4a940c2/SchedulingInstance.o: /home/marcelo/NetBeansProjects/SingleMachineExact/SchedulingInstance.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/SchedulingInstance.o /home/marcelo/NetBeansProjects/SingleMachineExact/SchedulingInstance.cpp

${OBJECTDIR}/_ext/4a940c2/Utils.o: /home/marcelo/NetBeansProjects/SingleMachineExact/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/4a940c2
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/4a940c2/Utils.o /home/marcelo/NetBeansProjects/SingleMachineExact/Utils.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/singlemachineexact

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
