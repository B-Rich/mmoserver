#include "script_engine.h"
#include <iostream>
#include <fstream>
#include "anh/event_dispatcher/event_dispatcher.h"

using namespace python;
using namespace std;

script_engine::script_engine(const std::string& base_path)
{
    base_path_ = base_path;
    // initialize the python script_engine
    Py_Initialize();
}
script_engine::~script_engine()
{
    loaded_files_.empty();
}
void script_engine::load(const std::string& filename)
{
    
    try{              
        string str(&getFileInput(filename)[0]);
        if (str.length() > 0)
        {
            bp::str file_str(str);
            loaded_files_.insert(std::make_pair(std::string(fullPath(filename)), file_str));
        }
    }
    catch(...)
    {
        getExceptionFromPy();
    }
}
void script_engine::run(const std::string& filename)
{
    // are you trying to run a file that's not loaded?
    // lets load the file and run it anyway
    if (!isFileLoaded(filename))
        load(filename);
    
    bp::str loaded_file = getLoadedFile(filename);
    try
    {
        // Retrieve the main module
        bp::object main = bp::import("__main__");
        // Retrieve the main module's namespace
        bp::object global(main.attr("__dict__"));
        bp::exec(loaded_file, global, global);
    }
    catch(...)
    {
        getExceptionFromPy();
    }
}
void script_engine::reload(const std::string& filename)
{
    if (isFileLoaded(filename))
    {
        removeFile(filename);
    }
    load(filename);
}
void script_engine::removeFile(const std::string& filename)
{
    auto it = loaded_files_.begin();
    for (; it != loaded_files_.end();)
    {
        if (it->first == fullPath(filename))
        {
            loaded_files_.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}
bool script_engine::isFileLoaded(const std::string& filename)
{
    auto it = std::find_if(loaded_files_.begin(), loaded_files_.end(), [this,&filename](bp_object_map::value_type& file){
        return file.first == fullPath(filename);
    });
    return it != loaded_files_.end();
}
void script_engine::setFullPath(const std::string& filename)
{
    setFullPath(filename, base_path_);
}
void script_engine::setFullPath(const std::string& filename, const std::string& root_path)
{
    full_path_.clear();
    full_path_.append(root_path);
    full_path_.append(filename);
}
bp::str script_engine::getLoadedFile(const std::string& filename)
{
    auto it = std::find_if(loaded_files_.begin(), loaded_files_.end(), [&](bp_object_map::value_type& file){
        return file.first == fullPath(filename);
    });
    if (it != loaded_files_.end())
        return it->second;
    else 
        return bp::str();
}
char* script_engine::fullPath(const std::string& filename)
{
    setFullPath(filename);
    return const_cast<char*>(full_path_.c_str());
}

vector<char> script_engine::getFileInput(const std::string& filename)
{
    vector<char> input;
    ifstream file(fullPath(filename), ios::in | ios::binary);
    if (!file.is_open())
    {
        // set our error message here
        setCantFindFileError();
        input.push_back('\0');
        return input;
    }

    file >> std::noskipws;
    copy(istream_iterator<char>(file), istream_iterator<char>(), back_inserter(input));
    input.push_back('\n');
    input.push_back('\0');
    
    return input;
}
void script_engine::getExceptionFromPy()
{
    PyObject* type, *value, *trace_back;
    PyErr_Fetch(&type, &value, &trace_back);
    // normalize to change value from tuple to string
    PyErr_NormalizeException(&type, &value, &trace_back);
    //value contains error message
    //trace_back contains stack snapshot and other information
    //(see python traceback structure)
    try
    {
        if (trace_back) {
            bp::handle<> hTraceback(trace_back);
            bp::object traceback(hTraceback);

            //Extract line number (top entry of call stack)
            // if you want to extract another levels of call stack
            // also process traceback.attr("tb_next") recurently
            py_exception.line_num = bp::extract<std::string> (traceback.attr("tb_lineno"));
            py_exception.file_name = bp::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
            py_exception.func_name = bp::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
        }
        if (value)
        {
            //Extract error message
            bp::handle<> hVal (PyObject_Str(value));
            bp::object err_msg(hVal);
            py_exception.err_msg = bp::extract<std::string>(err_msg);
        }
    } 
    catch(...)
    {
        return;
    }
}
void script_engine::setCantFindFileError()
{
    py_exception.file_name = full_path_;
    py_exception.err_msg = full_path_ + string(": No such file or directory");
}
std::string script_engine::getErrorMessage()
{
    if (py_exception.err_msg.length() > 0)
    {
        std::stringstream ss;
        ss << "Error: " << py_exception.err_msg;
        if (py_exception.line_num.length() >0)
        {
            ss << " On Line: " << py_exception.line_num;
            ss << " in file: " << py_exception.file_name;
            ss << " function: " << py_exception.func_name;
        }
        return ss.str();
    }
    else
    {
        return "Undefined Error";
    }
}