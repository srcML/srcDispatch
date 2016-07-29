#include <srcSAXEventDispatch.hpp>
#include <srcSAXHandler.hpp>
#include <unordered_map>
#include <unordered_set>
#include <DeclTypePolicy.hpp>
#include <srcSAXHandler.hpp>
#include <srcSAXEventDispatch.hpp>
#include <FunctionCallPolicy.hpp>
int main(int argc, char** filename){
	CallPolicy calldata;
    srcSAXController control(filename[1]);
    srcSAXEventDispatch::srcSAXEventDispatcher<CallPolicy> handler {&calldata};
    control.parse(&handler); //Start parsing
}