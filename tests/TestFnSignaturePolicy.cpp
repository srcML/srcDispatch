#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <srcSAXHandler.hpp>
#include <srcSAXEventDispatch.hpp>
#include <FunctionSignaturePolicy.hpp>
int main(int argc, char** filename){
    FunctionSignaturePolicy sigdata;
    srcSAXController control(filename[1]);
    srcSAXEventDispatch::srcSAXEventDispatcher<FunctionSignaturePolicy> handler {&sigdata};
    control.parse(&handler); //Start parsing
}