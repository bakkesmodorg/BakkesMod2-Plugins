#include "PythonPlugin.h"
#include "wrapped.h"
#include <boost/python.hpp>
#include "utils/parser.h"
#include "utils/io.h"

unusedbutneededtocompile abc;
const string PY_PATH = "./bakkesmod/py/";

#ifdef _AI_BUILD
BAKKESMOD_PLUGIN(PythonPlugin, "Python plugin", "0.1", PLUGINTYPE_BOTAI)
#else
BAKKESMOD_PLUGIN(PythonPlugin, "Python plugin", "0.1", 0)
#endif

extern std::string parse_python_exception();

object main_module;
dict main_namespace;

dict global_namespace;
dict local_namespace;

void PythonPlugin::reinit_python() {
	try {
		//global_namespace.clear();
		//olppplocal_namespace.clear();//

		// Copy builtins to new global namespace
		global_namespace = extract<dict>(main_module.attr("__dict__"));
		global_namespace["bakpy"] = boost::python::ptr(_bakPy.get());
		global_namespace["cvarManager"] = boost::python::ptr(cvarManager.get());
		global_namespace["gameWrapper"] = boost::python::ptr(gameWrapper.get());
	}
	catch (const error_already_set&) {
		string err = parse_python_exception();
		cvarManager->log("Python threw error: " + err);
	}
}

void PythonPlugin::onLoad()
{
	_bakPy = make_unique<BakPy>(BakPy(gameWrapper, cvarManager));
	cvarManager->registerNotifier("py_exec", [this, &cm = this->cvarManager, &gw = this->gameWrapper](vector<string> params) {
		if (params.size() < 2) {
			cm->log("usage: " + params.at(0) + " filename.py [params]");
			return;
		}
		string path = this->getSafeFileName(params.at(1));
		if (path.empty())
		{
			cm->log("Could not find file " + params.at(1));
			return;
		}
		try {
			_executes++;
			global_namespace["script"] = exec_file(str(path), global_namespace, global_namespace);
		}
		catch (const error_already_set&) {
			string err = parse_python_exception();
			cm->log("Python threw error: " + err);
		}
	});

	cvarManager->registerNotifier("py_run", [this, &cm = this->cvarManager, &gw = this->gameWrapper](vector<string> params) {
		if (params.size() < 2) {
			cm->log("usage: " + params.at(0) + " script");
			return;
		}
		string script = params.at(1);
		try {
			object result = exec(boost::python::str(script), global_namespace, global_namespace);
			cm->log(extract<std::string>(str(result)));
		}
		catch (const error_already_set&) {
			string err = parse_python_exception();
			cm->log("Python threw error: " + err);
		}
});

#ifdef TICK_BUILD
	enableTickable = make_shared<bool>(true);
	cvarManager->registerCvar("py_tickable_enabled", "1", "Enables tickable execution", true, true, 0, true, 1).bindTo(enableTickable);

	cvarManager->registerNotifier("py_tickable", [this, &cm = this->cvarManager, &gw = this->gameWrapper](vector<string> params) {
		if (params.size() < 2) {
			cm->log("usage: " + params.at(0) + " filename");
			return;
		}
		reinit_python(); //This doesn't work well enough I think
		string path = PY_PATH + params.at(1);
		if (!file_exists(path) && !string_ends_with(path, ".py")) {
			path += ".py";
		}
		if (!file_exists(path)) {
			cvarManager->log("Python script " + path + " does not exist!");
			return;
		}
		try {
			exec_file(str(path), global_namespace, global_namespace);
		}
		catch (const error_already_set&) {
			string err = parse_python_exception();
			cvarManager->log("Python threw error: " + err);
		}
	});

	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", bind(&PythonPlugin::on_tick, this, _1));

#endif

	try {
		Py_SetPath(wstring(L"./bakkesmod/libs/python36.zip").c_str());
		
		PyImport_AppendInittab("bakkesmod", &PyInit_bakkesmod);
		Py_Initialize();
		PyInit_bakkesmod();
		//PyObject* bmObj = 
		
		//initbakkesmod();
		main_module
			= object(handle<>(borrowed(PyImport_AddModule("__main__"))));

		main_namespace = extract<dict>(main_module.attr("__dict__"));
		main_namespace["bakpy"] = boost::python::ptr(_bakPy.get());
		main_namespace["cvarManager"] = boost::python::ptr(cvarManager.get());
		main_namespace["gameWrapper"] = boost::python::ptr(gameWrapper.get());
		reinit_python();
	}
	catch (const error_already_set&) {
		string err = parse_python_exception();
		cvarManager->log("Python threw error: " + err);
	}
	catch (const exception& ex) {
		cvarManager->log("Error: " + string(ex.what()));
	}
	catch (...) {
		cvarManager->log("UNKNOWN ERROR LOADING PYTHON PLUGIN");
	}

}

void PythonPlugin::onUnload()
{
	Py_Finalize();
}

namespace boost {
	namespace python {
		bool hasattr(object o, const char* name) {
			return PyObject_HasAttrString(o.ptr(), name);
		}
	}
}


#ifdef TICK_BUILD
void PythonPlugin::on_tick(string ignoredParam)
{
	if (*enableTickable && hasattr(main_module, "on_tick")) {
		auto tick_func = main_module.attr("on_tick");
		tick_func();
	}
}
#endif

std::string parse_python_exception() {
	PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
	// Fetch the exception info from the Python C API  
	PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

	// Fallback error  
	std::string ret("Unfetchable Python error");
	// If the fetch got a type pointer, parse the type into the exception string  
	if (type_ptr != NULL) {
		handle<> h_type(type_ptr);
		str type_pstr(h_type);
		// Extract the string from the boost::python object  
		extract<std::string> e_type_pstr(type_pstr);
		// If a valid string extraction is available, use it   
		//  otherwise use fallback  
		if (e_type_pstr.check())
			ret = e_type_pstr();
		else
			ret = "Unknown exception type";
	}
	// Do the same for the exception value (the stringification of the exception)  
	if (value_ptr != NULL) {
		handle<> h_val(value_ptr);
		str a(h_val);
		extract<std::string> returned(a);
		if (returned.check())
			ret += ": " + returned();
		else
			ret += std::string(": Unparseable Python error: ");
	}
	// Parse lines from the traceback using the Python traceback module  
	if (traceback_ptr != NULL) {
		handle<> h_tb(traceback_ptr);
		// Load the traceback module and the format_tb function  
		object tb(import("traceback"));
		object fmt_tb(tb.attr("format_tb"));
		// Call format_tb to get a list of traceback strings  
		object tb_list(fmt_tb(h_tb));
		// Join the traceback strings into a single string  
		object tb_str(str("\n").join(tb_list));
		// Extract the string, check the extraction, and fallback in necessary  
		extract<std::string> returned(tb_str);
		if (returned.check())
			ret += ": " + returned();
		else
			ret += std::string(": Unparseable Python traceback");
	}
	return ret;
}

BakPy::BakPy(std::shared_ptr<GameWrapper> gw, std::shared_ptr<CVarManagerWrapper> cw) : gameWrapper(gw), cvarManager(cw)
{
}

void BakPy::set_timeout(string methodname, float time)
{
	gameWrapper->SetTimeout([this, methodname](GameWrapper* gw) {
		if (hasattr(main_module, methodname.c_str())) {
			try {
				auto callback_func = main_module.attr(str(methodname));
				callback_func();
			}
			catch (const error_already_set&) {
				string err = parse_python_exception();
				cvarManager->log("(callback) Python threw error: " + err);
			}
		}
		else {
			cvarManager->log("Callback function " + methodname + " does not exist!");
		}
	}, time);

}

string PythonPlugin::getSafeFileName(string initialName)
{
	string path = PY_PATH + initialName;
	if (!file_exists(path) && !string_ends_with(path, ".py")) 
	{
		path += ".py";
	}
	if (!file_exists(path)) {
		return "";
	}
	return path;
}

