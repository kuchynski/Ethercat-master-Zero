
INCLUDE_DIRS =	-I./core \
				-I./osal \
				-I./osal/Linux
SRC_FILES = ./main.cpp \
			./core/CScheduler.cpp \
			./core/CSlave.cpp \
			./core/CBus.cpp \
			./core/CDatagram.cpp \
			./core/CFrame.cpp \
			./core/CModule.cpp \
			./core/CModuleScanBus.cpp \
			\
			./osal/CTimer.cpp \
			./osal/CNetworkAdapter.cpp \
			\
			./osal/Linux/CLinuxTimer.cpp \
			./osal/Linux/CLinuxNetworkAdapter.cpp

master0: $(SRC_FILES)
	g++ -std=c++11 -o master0 $^ $(INCLUDE_DIRS)
#-Wall
clean: rm -f *.o
