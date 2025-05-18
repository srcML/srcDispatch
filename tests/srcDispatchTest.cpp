#include "srcDispatchTest.hpp"

const std::string OK = "[\033[32m+\033[0m]";
const std::string INFO = "[\033[33m*\033[0m]";
const std::string ERR = "[\033[31m-\033[0m]";

void Print_Ok(std::string msg) {
    std::cout << OK << " " << msg << std::endl;
};
void Print_Info(std::string msg) {
    std::cout << INFO << " " << msg << std::endl;
};
void Print_Err(std::string msg) {
    std::cout << ERR << " " << msg << std::endl;
    exit(1); // quit testing program when an error occurs
};

std::string StringToSrcML(std::string str, const char* fileName, LANG lang){
    struct srcml_archive* archive;
    struct srcml_unit* unit;
    
    size_t size = 0;
    char* ch;

    archive = srcml_archive_create();
    srcml_archive_enable_option(archive, SRCML_OPTION_POSITION);
    srcml_archive_write_open_memory(archive, &ch, &size);

    unit = srcml_unit_create(archive);
    /*
            SRCML_LANG_MODES
        SRCML_LANGUAGE_NONE   0
        SRCML_LANGUAGE_C      "C"
        SRCML_LANGUAGE_CXX    "C++"
        SRCML_LANGUAGE_CSHARP "C#"
        SRCML_LANGUAGE_JAVA   "Java"
        SRCML_LANGUAGE_XML    "xml"
    */
    switch(lang) {
        case LANG::C:
            srcml_unit_set_language(unit, SRCML_LANGUAGE_C);
        break;
        case LANG::CXX:
            srcml_unit_set_language(unit, SRCML_LANGUAGE_CXX);
        break;
        
        case LANG::CSHARP:
            srcml_unit_set_language(unit, SRCML_LANGUAGE_CSHARP);
        break;
        
        case LANG::JAVA:
            srcml_unit_set_language(unit, SRCML_LANGUAGE_JAVA);
        break;
    }

    srcml_unit_set_filename(unit, fileName);

    srcml_unit_parse_memory(unit, str.c_str(), str.size());
    srcml_archive_write_unit(archive, unit);
    
    srcml_unit_free(unit);
    srcml_archive_close(archive);
    srcml_archive_free(archive);

    // ensure the final char is null-terminator
    ch[size-1] = 0;

    // Copy the buffer content to a string so we
    // can deallocate the buffer created by:
    // `srcml_archive_write_open_memory`
    std::string output;
    output.append(ch,size);

    srcml_memory_free(ch);
    
    return output;
}