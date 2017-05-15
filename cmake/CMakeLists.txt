#############################################################
### YOU SHOULD EDIT THE BELOW WITH YOUR DETAILS AND FILES ###
#############################################################

#######################
# Build instructions: #
#######################

# 1. open "VS2015 x64 Native Tools Command Prompt" as administrator, and change dir to source-code and CMakeList.txt directory.
# 2. mkdir build64 & pushd build64
# 3. cmake -G "Visual Studio 14 2015 Win64" ..
# 4. popd
# 5. cmake --build build64 --config Release

set(user1 oribarel)
set(user2 eyalwaserman)

set(id1 "305135956")
set(id2 "305210189")


# All source files needed to compile your exe
set (EXE_SOURCE_FILES
	BoardClass.h
	GameManager.h
	IBattleshipGameAlgo.h
	Ship.h
	Utils.h
	BattleshipGame.cpp
	BoardClass.cpp
	GameManager.cpp
	Ship.cpp
	Utils.cpp
)
# All source files needed to compile your naive algorithm's .dll
set (NAIVE_ALGO_SOURCE_FILES
	BattleshipNaivePlayer.h
	BoardClass.h
	IBattleshipGameAlgo.h
	Ship.h
	Utils.h
	BattleshipNaivePlayer.cpp
	BoardClass.cpp
	Ship.cpp
	Utils.cpp
)
# All source files needed to compile your read-from-file algorithm's .dll
set (FILE_ALGO_SOURCE_FILES
	BattleshipPlayerFromFile.h
	BoardClass.h
	IBattleshipGameAlgo.h
	Ship.h
	Utils.h
	BattleshipPlayerFromFile.cpp
	BoardClass.cpp
	Ship.cpp
	Utils.cpp
)
# All source files needed to compile your smart algorithm's .dll
set (SMART_ALGO_SOURCE_FILES
	Battleship_MAP_player.h
	BoardClass.h
	IBattleshipGameAlgo.h
	Ship.h
	Utils.h
	Battleship_MAP_player.cpp
	BoardClass.cpp
	Ship.cpp
	Utils.cpp
)

##################################
### DO NOT CHANGE FROM HERE ON ###
##################################

message("user: ${user1}, id: ${id1}")

# Set exe build parameters
set(EXE_NAME "ex2.${user1}")
add_executable( ${EXE_NAME}	${EXE_SOURCE_FILES})
set_property(TARGET ${EXE_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${EXE_NAME} PROPERTY OUTPUT_NAME "${user1}")

# Set naive algorithm's .dll build parameters
set(ALG1_NAME "ex2.${user1}.naive")
add_library(${ALG1_NAME} SHARED ${NAIVE_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG1_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG1_NAME} PROPERTY OUTPUT_NAME "${user1}.naive")
target_compile_definitions(${ALG1_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now

# Set read-from-file algorithm's .dll build parameters
set(ALG2_NAME "ex2.${user1}.file")
add_library(${ALG2_NAME} SHARED ${FILE_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG2_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG2_NAME} PROPERTY OUTPUT_NAME "${user1}.file")
target_compile_definitions(${ALG2_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now

# Set smart algorithm's .dll build parameters
set(ALG3_NAME "ex2.${user1}.smart")
add_library(${ALG3_NAME} SHARED ${SMART_ALGO_SOURCE_FILES})
set_property(TARGET ${ALG3_NAME} PROPERTY FOLDER "ex2/${user1}")
set_property(TARGET ${ALG3_NAME} PROPERTY OUTPUT_NAME "${user1}.smart")
target_compile_definitions(${ALG3_NAME} PRIVATE -DALGO_EXPORTS) # preprocessor flag saying we are the one compiling the header now