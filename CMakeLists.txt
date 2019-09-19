CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(isoftToken VERSION 0.1.0)

FIND_PACKAGE(eosio.cdt)


#ADD_CONTRACT(
#    isoftToken isoftToken 
#    ${CMAKE_CURRENT_SOURCE_DIR}/isoft.token.cpp
#)

# ------------


SET(CONTRACT_NAME "isoftToken")
SET(MAIN_SRC_NAME "isoft.token")

# build wasm
ADD_EXECUTABLE(
    ${CONTRACT_NAME}.wasm
    ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC_NAME}.cpp
)

# generate abi
ADD_CUSTOM_COMMAND(
    TARGET ${CONTRACT_NAME}.wasm
    POST_BUILD
    COMMENT "Generating ABI"
    COMMAND ${ABIGEN} ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_SRC_NAME}.cpp -output=${CONTRACT_NAME}.abi.tmp -contract ${CONTRACT_NAME} -- -DBOOST_DISABLE_ASSERTS
    COMMAND sh ../run_sed.sh ${CONTRACT_NAME}.abi.tmp ${CONTRACT_NAME}.abi
    COMMAND rm ${CONTRACT_NAME}.abi.tmp
    COMMAND python3 ../set_recardian.py ${CONTRACT_NAME}.abi
)
